#ifndef CREATEPROJECTEDCOORDINATESYSTEM_H
#define CREATEPROJECTEDCOORDINATESYSTEM_H


namespace Ilwis {
namespace BaseOperations {
class CreateProjectedCoordinateSystem :  public OperationImplementation
{
public:
    CreateProjectedCoordinateSystem();

    CreateProjectedCoordinateSystem(quint64 metaid, const Ilwis::OperationExpression &expr);

    bool execute(ExecutionContext *ctx, SymbolTable& symTable);
    static Ilwis::OperationImplementation *create(quint64 metaid,const Ilwis::OperationExpression& expr);
    Ilwis::OperationImplementation::State prepare(ExecutionContext *ctx,const SymbolTable&);

    static quint64 createMetadata();

    NEW_OPERATION(CreateProjectedCoordinateSystem);

private:
    IProjection _projection;
    IEllipsoid _ellipsoid;
    std::map<QString, double> _parms;


};
}
}

#endif // CREATEPROJECTEDCOORDINATESYSTEM_H
