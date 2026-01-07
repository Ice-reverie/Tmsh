#include "GraphEventOperator.h"

namespace EventOper
{
    void GraphEventOperator::updateAllGraph()
    {

    }

    void GraphEventOperator::updateGraph(int dataObjId, bool forceUpdate)
    {
        Q_UNUSED(dataObjId);
        Q_UNUSED(forceUpdate);
    }

    void GraphEventOperator::updateColor(int dataObjId, QColor color, QVariant otherPar)
    {
        Q_UNUSED(dataObjId);
        Q_UNUSED(otherPar);
        Q_UNUSED(color);
    }

    void GraphEventOperator::updateColor(int dataObjId, QColor color, bool forceUpdate)
    {
        Q_UNUSED(dataObjId);
        Q_UNUSED(color);
        Q_UNUSED(forceUpdate);
    }

    Graph::GraphObjectBase * GraphEventOperator::getModelGraphObjectByDataId(int dataObjId)
    {
        Q_UNUSED(dataObjId);
        return nullptr;
    }

    void GraphEventOperator::updateGraphRecursively(int dataObjId, int refLayer, bool recDown)
    {
        Q_UNUSED(dataObjId);
        Q_UNUSED(refLayer);
        Q_UNUSED(recDown);
    }

    void GraphEventOperator::reRender()
    {

    }

    void GraphEventOperator::fitView()
    {

    }
}
