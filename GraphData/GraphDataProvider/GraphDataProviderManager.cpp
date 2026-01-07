#include "GraphDataProviderManager.h"
#include "GraphDataProviderModel.h"
#include "GraphDataProviderMark.h"

namespace Graph
{
    //静态变量初始化
    GraphDataProviderManager* GraphDataProviderManager::_instance = nullptr;
    QMutex GraphDataProviderManager::m_mutex;

    GraphDataProviderModel * GraphDataProviderManager::getModelProvider(Comp::FITKGraph3DWindowVTK * graphWidget)
    {
        if (_modelProHash.keys().contains(graphWidget)) {
            return _modelProHash.value(graphWidget);
        }

        GraphDataProviderModel* pro = new GraphDataProviderModel(graphWidget);
        _modelProHash.insert(graphWidget, pro);
        return pro;
    }

    GraphDataProviderMark * GraphDataProviderManager::getMarkProvider(Comp::FITKGraph3DWindowVTK * graphWidget)
    {
        if (_markProHash.keys().contains(graphWidget)) {
            return _markProHash.value(graphWidget);
        }

        GraphDataProviderMark* pro = new GraphDataProviderMark(graphWidget);
        _markProHash.insert(graphWidget, pro);
        return pro;
    }

    void GraphDataProviderManager::initialize()
    {

    }

    void GraphDataProviderManager::finalize()
    {
        _modelProHash.clear();
        _markProHash.clear();
    }
}