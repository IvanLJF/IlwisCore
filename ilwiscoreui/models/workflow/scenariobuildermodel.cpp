#include <future>
#include <QUuid>

#include "kernel.h"
#include "scenariobuildermodel.h"
#include "scenariodesignermodel.h"


ScenarioBuilderModel::ScenarioBuilderModel(QObject *parent) : QObject(parent)
{

}

ModelDesigner *ScenarioBuilderModel::create()
{
    return new ModelDesigner();
}
