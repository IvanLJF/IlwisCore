#include "raster.h"
#include "table.h"
#include "featurecoverage.h"
#include "feature.h"
#include "featureiterator.h"
#include "raster.h"
#include "symboltable.h"
#include "ilwisoperation.h"
#include "mastercatalog.h"
#include "setattributetabel.h"

using namespace Ilwis;
using namespace BaseOperations;

REGISTER_OPERATION(SetAttributeTable)

SetAttributeTable::SetAttributeTable()
{
}


SetAttributeTable::SetAttributeTable(quint64 metaid, const Ilwis::OperationExpression &expr) :
    OperationImplementation(metaid, expr)
{
}

bool SetAttributeTable::execute(ExecutionContext *ctx, SymbolTable& symTable)
{
    if (_prepState == sNOTPREPARED)
        if((_prepState = prepare(ctx, symTable)) != sPREPARED)
            return false;

    if ( _inputCoverage->ilwisType() == itRASTER){
        IRasterCoverage raster = _inputCoverage.as<RasterCoverage>();
//        ColumnDefinition coldef = _inputTable->columndefinition(_primaryKey);
//        coldef.name(COVERAGEKEYCOLUMN);
//        _inputTable->addColumn(coldef);
        raster->setAttributes(_inputTable);
    }
    return true;
}

Ilwis::OperationImplementation *SetAttributeTable::create(quint64 metaid, const Ilwis::OperationExpression &expr)
{
    return new SetAttributeTable(metaid, expr);
}

Ilwis::OperationImplementation::State SetAttributeTable::prepare(ExecutionContext *, const SymbolTable &)
{

    OperationHelper::check([&] ()->bool { return _inputTable.prepare(_expression.input<QString>(0), itTABLE); },
    {ERR_COULD_NOT_LOAD_2,_expression.input<QString>(0), "" } );

    OperationHelper::check([&] ()->bool { return _inputCoverage.prepare(_expression.input<QString>(1), itCOVERAGE); },
    {ERR_COULD_NOT_LOAD_2,_expression.input<QString>(1), "" } );

    _primaryKey = _expression.input<QString>(2);

    int index = _inputTable->columnIndex(_primaryKey);
    if ( index == iUNDEF){
        kernel()->issues()->log(TR("Table doesnt contain column ") + _primaryKey);
        return sPREPAREFAILED;
    }
    if ( _inputCoverage->ilwisType() == itRASTER){
        ColumnDefinition coldef = _inputTable->columndefinition(index);
        IRasterCoverage raster = _inputCoverage.as<RasterCoverage>();
        if ( coldef.datadef().domain() != raster->datadef().domain()){
            kernel()->issues()->log(TR("Key column doesnt match raster domain ") + _primaryKey);
            return sPREPAREFAILED;
        }
    }


    return sPREPARED;
}

quint64 SetAttributeTable::createMetadata()
{
    OperationResource operation({"ilwis://operations/setsttributetable"});
    operation.setSyntax("setattributetable(inputtable, coverage, primarykey)");
    operation.setDescription(TR("assigns the input table as attribute data to coverage, note that for features is is a (kind of) copy as they dont have real attribute tables"));
    operation.setInParameterCount({3});
    operation.addInParameter(0,itTABLE , TR("input table"),TR("Table with at least one column suitable as primary key, For features this column must hold the indexes for the features to be coupled"));
    operation.addInParameter(0,itCOVERAGE , TR("coverage"),TR("For rasters the domain must be the same as the primary key, features are coupled either by index or by index number in the key column"));
    operation.addInParameter(0,itSTRING , TR("key column"),TR("Column that serves as primary key for connecting the table to the coverage"));
    operation.setOutParameterCount({0});
    operation.setKeywords("coverage, selection");

    mastercatalog()->addItems({operation});
    return operation.id();

}
