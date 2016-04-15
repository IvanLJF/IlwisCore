#include "kernel.h"
#include "ilwisdata.h"
#include "mastercatalog.h"
#include "workflow.h"
#include "operationmetadata.h"
#include "workflowmodel.h"
#include "symboltable.h"
#include "commandhandler.h"
#include "featurecoverage.h"
#include "operationcatalogmodel.h"
#include "../workflowerrormodel.h"
#include "ilwiscontext.h"
#include "ilwistypes.h"

using namespace Ilwis;
using namespace boost;

WorkflowModel::WorkflowModel()
{
}

WorkflowModel::~WorkflowModel()
{
    for(auto *node : _nodeProps)
        delete node;
    for(auto *edge : _edgeProps)
        delete edge;
}

WorkflowModel::WorkflowModel(const Ilwis::Resource &source, QObject *parent) : OperationModel(source, parent)
{
    _workflow.prepare(source);
    connect(this, &WorkflowModel::sendMessage, kernel(), &Kernel::acceptMessage);
    connect(kernel(), &Kernel::sendMessage, this, &WorkflowModel::acceptMessage);
}

QStringList WorkflowModel::assignConstantInputData(QString inputData, int operationIndex) {
    QStringList inputParameters = inputData.split('|');
    OVertex vertex = operationIndex;
    QStringList* parameterEntrySet = new QStringList();

    for (int i = 0; i < inputParameters.length(); ++i) {
        QString value = inputParameters[i];
        if ( value == "")
            continue;
        if(_workflow->hasInputAssignment(vertex,i)){
            SPAssignedInputData constantInput = _workflow->getAssignedInputData({vertex, i});
            bool oldValueFilled = constantInput->value.size() != 0;

            if (constantInput->value.isEmpty() && value.trimmed().size() > 0) {
                int parameterIndex = _workflow->getWorkflowParameterIndex(vertex, i);
                parameterEntrySet->push_back(QString::number(parameterIndex) + "|remove");
                --_inputParameterCount;
            }

            if (value.trimmed().isEmpty()) {
                value = value.trimmed();
            }

            constantInput->value = value;

            if (value.isEmpty() && oldValueFilled) {
                int parameterIndex = _workflow->getWorkflowParameterIndex(vertex, i);
                parameterEntrySet->push_back(QString::number(parameterIndex) + "|insert");
                ++_inputParameterCount;
            }
        }
    }
    return *parameterEntrySet;
}

void WorkflowModel::assignConditionInputData(QString inputData, QStringList ids)
{
    // TODO: return a parameterEntrySet like method above
    _workflow->assignConditionInputData(ids[0].toInt(), ids[1].toInt(), inputData.split('|'));
}

QStringList WorkflowModel::addOperation(const QString &id)
{
    QStringList* parameterEntrySet = new QStringList();
    bool ok;
    quint64 opid = id.toULongLong(&ok);
    Resource res = mastercatalog()->id2Resource(opid);

    if ( ok && res.isValid()){
        OVertex v = _workflow->addOperation({res});
        IOperationMetaData meta = _workflow->getOperationMetadata(v);
        std::vector<SPOperationParameter> inputs = meta->getInputParameters();
        for (int i = 0 ; i < inputs.size() ; i++) {
            _workflow->assignInputData(v, i);
            ++_inputParameterCount;
            int parameterIndex = _workflow->getWorkflowParameterIndex(v, i);
            parameterEntrySet->push_back(QString::number(parameterIndex) + "|insert");
        }
    }else {
       kernel()->issues()->log(QString(TR("Invalid operation id used in workflow %1")).arg(name()));
    }
    return *parameterEntrySet;
}

QStringList WorkflowModel::addFlow(int vertexFrom, int vertexTo, const QVariantMap& flowpoints, int rectFrom, int rectTo)
{
    QStringList* parameterEntrySet = new QStringList();
    if ( vertexFrom >= 0 && vertexTo >= 0 && flowpoints.size() == 2) {
        try {
            const OVertex& fromOperationVertex = vertexFrom;
            const OVertex& toOperationVertex = vertexTo;
            int outParamIndex = flowpoints["fromParameterIndex"].toInt();
            int inParamIndex = flowpoints["toParameterIndex"].toInt();

            int parameterIndex = _workflow->getWorkflowParameterIndex(toOperationVertex, inParamIndex);
            parameterEntrySet->push_back(QString::number(parameterIndex) + "|remove");
            --_inputParameterCount;

            EdgeProperties flowPoperties(
                outParamIndex, inParamIndex,
                rectFrom, rectTo
            );

            _workflow->addOperationFlow(fromOperationVertex,toOperationVertex,flowPoperties);
        } catch (std::out_of_range e) {
           qDebug() << "False operation";
        }
    }
    return *parameterEntrySet;
}

bool WorkflowModel::hasValueDefined(int operationIndex, int parameterIndex){
    try {
        const OVertex& operationVertex = operationIndex;
        return _workflow->hasValueDefined(operationVertex, parameterIndex);
    } catch (std::out_of_range e) {
       return false;
    }
}

/**
 * Returns the number of input parameters of an operations, this is the same amount as can be seen in the run/workflow form.
 * @param operationIndex the operation who's input parameter count to get.
 * @return The number of input parameters
 */
int WorkflowModel::operationInputParameterCount(int operationIndex){
    const OVertex& operationVertex = operationIndex;

    QList<SPAssignedInputData> list = _workflow->getAssignedInputData(operationVertex);

    int inParameterCount = list.length();

    for(int i=0;i<list.size();++i){
        if(list[i]->value != ""){
            --inParameterCount;
        }
    }

    return inParameterCount;
}

/**
 * Returns the number of output parameters of an operations, this is the same amount as can be seen in the run/workflow form.
 * @param operationIndex the operation who's output parameter count to get.
 * @return The number of output parameters
 */
int WorkflowModel::operationOutputParameterCount(int operationIndex){
    const OVertex& operationVertex = operationIndex;

    QStringList operationOutputs = _workflow->getOutputTerms(operationVertex);

    std::vector<quint16> assignedOuputs = _workflow->getAssignedPouts(operationVertex);

    return operationOutputs.length()-assignedOuputs.size();
}

/**
 * Returns the values of an operation which have already been defined (a flow has been drawn to it)
 * @param operationIndex the operation to check
 * @return a string of fields which have been defined, seperated by |
 */

QStringList WorkflowModel::implicitIndexes(int operationIndex){
    try {
        const OVertex& operationVertex = operationIndex;
        return _workflow->implicitIndexes(operationVertex);
    } catch (std::out_of_range e) {
       return QStringList();
    }
}

QStringList WorkflowModel::deleteOperation(int index)
{
    QStringList* parameterEntrySet = new QStringList();
    try {

        if ( index < _workflow->getOperationCount()){
            const OVertex& operationVertex = index;

            // In edges
            std::pair<InEdgeIterator,InEdgeIterator> inIterators = _workflow->getInEdges(operationVertex);
            for (auto &iter = inIterators.first; iter < inIterators.second; ++iter) {
                _workflow->removeOperationFlow(*iter);
            }

            // Out edges
            std::pair<OutEdgeIterator,OutEdgeIterator> outIterators = _workflow->getOutEdges(operationVertex);
            for (auto &iter = outIterators.first; iter < outIterators.second; ++iter) {
                _workflow->removeOperationFlow(*iter);
            }

            for (int parameterIndex: *_workflow->getWorkflowParameterIndex(operationVertex)) {
                parameterEntrySet->push_front(QString::number(parameterIndex) + "|remove");
                --_inputParameterCount;
            }

            _workflow->removeOperation(operationVertex);

        } else {
            qDebug() << "There are no operations";
        }
        return *parameterEntrySet;
    } catch (std::out_of_range e) {
        qDebug() << "False operation";
        return *(new QStringList());
    }
}

QStringList WorkflowModel::deleteFlow(int vertexFrom, int vertexTo, int parameterFrom, int parameterto)
{
    QStringList* parameterEntrySet = new QStringList();
    OVertex sourceNode = vertexFrom;
    boost::graph_traits<WorkflowGraph>::out_edge_iterator ei, ei_end;
    for (boost::tie(ei,ei_end) = _workflow->getOutEdges(sourceNode); ei != ei_end; ++ei) {

        OVertex targetNode = _workflow->getTargetOperationNode(*ei);

        NodeProperties npNode = _workflow->nodeProperties(targetNode);
        NodeProperties npTarget = _workflow->nodeProperties(vertexTo);

        if(npNode._operationid == npTarget._operationid)
        {
            EdgeProperties ep = _workflow->edgeProperties(*ei);

            if(ep._outputParameterIndex == parameterFrom && ep._inputParameterIndex == parameterto)
            {
                _workflow->removeOperationFlow(*ei);
                int parameterIndex = _workflow->getWorkflowParameterIndex(targetNode, parameterto);
                parameterEntrySet->push_back(QString::number(parameterIndex) + "|insert");
                ++_inputParameterCount;
            }
        }
    }
    return *parameterEntrySet;
}

/**
 * Returns the nodes of the workflow
 */
QQmlListProperty<NodePropObject> WorkflowModel::getNodes()
{
    if ( !_workflow.isValid())
        return QQmlListProperty<NodePropObject>();

    if ( _nodeProps.size() != 0) {
        for(auto *node : _nodeProps)
            delete node;
        _nodeProps.clear();
    }

    std::pair<WorkflowVertexIterator, WorkflowVertexIterator> nodeIterators = _workflow->getNodeIterators();
    for (auto &iter = nodeIterators.first; iter < nodeIterators.second; ++iter) {
        NodePropObject *nodeProp = new NodePropObject();
        nodeProp->setProps(_workflow->nodeProperties(*iter), *iter);
        _nodeProps.append(std::move(nodeProp));
    }
    return  QQmlListProperty<NodePropObject>(this, _nodeProps);
}

///**
// * Returns the edges of the node
// */
QQmlListProperty<EdgePropObject> WorkflowModel::getEdges()
{
    if ( _edgeProps.size() != 0) {
        for(auto *edge : _edgeProps)
            delete edge;
        _edgeProps.clear();
    }

    std::pair<WorkflowVertexIterator, WorkflowVertexIterator> nodeIterators = _workflow->getNodeIterators();
    for (auto &iter = nodeIterators.first; iter < nodeIterators.second; ++iter) {
        std::pair<OutEdgeIterator,OutEdgeIterator> edgeIterators = _workflow->getOutEdges(*iter);
        for (auto &iter2 = edgeIterators.first; iter2 < edgeIterators.second; ++iter2) {
            EdgePropObject *edgeProp = new EdgePropObject();
            edgeProp->setProps(_workflow->edgeProperties(*iter2), *iter, _workflow->getTargetOperationNode(*iter2));
            _edgeProps.append(std::move(edgeProp));
        }
    }
    return  QQmlListProperty<EdgePropObject>(this, _edgeProps);
}

void WorkflowModel::addOperationToContainer(quint16 containerIndex, quint16 operationId)
{
    OVertex v = operationId;
   _workflow->addOperationToContainer(containerIndex,v);
}

void WorkflowModel::removeOperationFromContainer(quint16 containerIndex, quint16 operationId)
{
    OVertex v = operationId;
    _workflow->removeOperationFromContainer(containerIndex,v);
}

QStringList WorkflowModel::getAsignedValuesByItemID(int itemId)
{
    QStringList* results = new QStringList();
    OVertex v = itemId;

    // Add edge values to fill empty spots
    auto edgeIterators = _workflow->getInEdges(v);
    for (auto &iter = edgeIterators.first; iter < edgeIterators.second; ++iter) {
        results->insert(_workflow->edgeProperties(*iter)._inputParameterIndex, "");
    }

    // Constant values
    QList<InputAssignment> inputs = _workflow->getInputAssignments(itemId);
    for (const InputAssignment &input : inputs) {
        results->insert(input.second, _workflow->getAssignedInputData(input)->value);
    }

    return *results;
}

void WorkflowModel::store(const QStringList &coordinates)
{
    try {
        for (int i = 0; i< coordinates.size(); i++) {
            QStringList split = coordinates[i].split('|');
            OVertex v = i;
            NodeProperties props = _workflow->nodeProperties(v);
            props._x = split[0].toDouble();
            props._y = split[1].toDouble();
            _workflow->updateNodeProperties(v, props);
        }


        _workflow->connectTo(_workflow->resource().url(true), QString("workflow"), QString("stream"), Ilwis::IlwisObject::cmOUTPUT);
        _workflow->createTime(Ilwis::Time::now());
        _workflow->store();
    } catch(const ErrorObject&){

    }
}

void WorkflowModel::createMetadata()
{
    _workflow->createMetadata();
}

QVariantMap WorkflowModel::addCondition(int containerId, int operationId)
{
    return _workflow->addCondition(containerId, operationId);
}


QVariantMap WorkflowModel::getOpenConditionParameters(const int containerId, const int conditionId)
{
    Condition condition = _workflow->getCondition(containerId, conditionId);
    QStringList hiddenFields;
    QStringList constantValues;
    QVariantMap resultMap;

    for (const AssignedInputData parameter : condition._inputAssignments) {
        constantValues.push_back(parameter.value);
    }
    resultMap.insert("constantValues", constantValues);

    for (const EdgeProperties edge : condition._edges) {
        hiddenFields.push_back(QString::number(edge._inputParameterIndex));
    }
    resultMap.insert("hiddenFields", hiddenFields);
    resultMap.insert("operationId", condition._operation->id());

    return resultMap;
}

QVariantList WorkflowModel::getConditions(int containerId)
{
    ConditionContainer container = _workflow->getContainer(containerId);
    QVariantList results;
    int i = 0;

    for (Condition condition : container.conditions) {
        QVariantMap map;

        map.insert("xId", i);
        map.insert("name", "");

        QString value = condition._operation->resource()["keyword"].toString();
        if (value.contains("condition")){
            map.insert("first", !condition._inputAssignments[0].value.isEmpty());
            map.insert("condition", condition._inputAssignments[1].value);
            map.insert("second", !condition._inputAssignments[2].value.isEmpty());
        } else {
            bool set = true;
            for (AssignedInputData input : condition._inputAssignments){
                if (input.value.isEmpty()) {
                    set = false;
                }
            }
            map.insert("name", condition._operation->name());
            map.insert("set", set);
        }

        results.push_back(map);

        ++i;
    }

    return results;
}

void WorkflowModel::setSelectedOperationId(const QString& id)
{
    _selectedOperation.clear();
    Resource res = mastercatalog()->id2Resource(id.toULongLong());
    if ( res.isValid()){
        IlwisObjectModel *iomodel = new IlwisObjectModel(res,this);
        _selectedOperation.append(iomodel);
    }
}

QVariantList WorkflowModel::propertyList()
{
    QVariantList result;

    QVariantMap workflow;
    workflow["label"] = TR("Workflow(Run) form");
    workflow["form"] = "OperationForms.qml";
    result.append(workflow);
    QVariantMap operation;
    operation["label"] = TR("Selected operation form");
    operation["form"] = "OperationForms.qml";
    result.append(operation);
    QVariantMap metadata;
    metadata["label"] = TR("Metadata selected form");
    metadata["form"] = "workflow/forms/OperationPropForm.qml";
    result.append(metadata);
    QVariantMap script;
    metadata["label"] = TR("Script");
    metadata["form"] = "workflow/forms/WorkflowPythonScript.qml";
    result.append(metadata);

    return result;
}

QString WorkflowModel::generateScript(const QString &type, const QString& parameters)
{
    try {
        QString result;
        if ( type == "python"){
            _expression = OperationExpression::createExpression(_workflow->id(),parameters,true);
            if ( !_expression.isValid())
                return "";
            QUrl url = _workflow->resource().url(true);
            QFileInfo inf(url.toLocalFile());
           // QUrl newName = QUrl::fromLocalFile(inf.path() + "/" + inf.baseName() + ".py");
            QUrl newName = "ilwis://internalcatalog/" + inf.baseName() + ".py";
            _workflow->connectTo(newName,"inmemoryworkflow","python",IlwisObject::cmOUTPUT);
            QVariant value;
            value.setValue(_expression);
            IOOptions opt("expression", value);
            _workflow->store(opt);
            result = _workflow->constConnector(IlwisObject::cmOUTPUT)->getProperty("content").toString();
        }
        return result;
    }
    catch (ErrorObject&){
    }
    return "";
}

void WorkflowModel::gotoStepMode()
{
    QVariantMap parms;
    parms["stepmode"] = true;
    parms["id"] = _workflow->id();
    emit sendMessage("workflow","stepmode", parms);
}

void WorkflowModel::acceptMessage(const QString &type, const QString &subtype, const QVariantMap &parameters)
{
    if ( type == "workflow"){
        bool ok;
        quint64 id = parameters["id"].toLongLong(&ok);
        if ( ok && id == _workflow->id()){ // check if this was meant for this workflow

        }
    }
}

QList<QVariantMap> WorkflowModel::outputCurrentOperation() const
{
    return QList<QVariantMap>();
}

void WorkflowModel::debug(const QString &code)
{
    if ( _workflow.isValid()){
        if ( code == "graph")
            _workflow->debugPrintGraph();
        if ( code == "topologicalsort")
            _workflow->debugPrintToplogicalOrder();
    }
}

 QQmlListProperty<IlwisObjectModel> WorkflowModel::getSelectedOperation()
{
   return QQmlListProperty<IlwisObjectModel>(this, _selectedOperation) ;
}

 QString WorkflowModel::modelType() const
 {
     return "workflowmodel";
 }

