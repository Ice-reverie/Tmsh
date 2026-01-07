/**********************************************************************
 * @file   GraphDataProviderManager.h
 * @brief  渲染数据管理器
 * @author BaGuijun (baguijun@163.com)
 * @date   2025-03-12
 *********************************************************************/
#ifndef GraphDataProviderManager_H
#define GraphDataProviderManager_H

#include "GraphDataProviderAPI.h"

#include "FITK_Kernel/FITKCore/FITKCoreMacros.h"

#include <QObject>
#include <QMutex>
#include <QHash>

namespace Comp {
    class FITKGraph3DWindowVTK;
}

namespace Graph
{
    class GraphDataProviderModel;
    class GraphDataProviderMark;
    /**
     * @brief  渲染数据管理器
     * @author BaGuijun (baguijun@163.com)
     * @date   2025-03-12
     */
    class GraphDataProviderAPI GraphDataProviderManager : public QObject
    {
        Q_OBJECT;
        DeclSingleton(GraphDataProviderManager);
    public:
        /**
         * @brief    获取渲染模型管理器
         * @param[i] graphWidget                             渲染界面
         * @return   GraphDataProviderModel*                 渲染模型管理器
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-03-12
         */
        GraphDataProviderModel* getModelProvider(Comp::FITKGraph3DWindowVTK* graphWidget);
        /**
         * @brief    获取渲染模型管理器
         * @param[i] graphWidget                             渲染界面
         * @return   GraphDataProviderMark*                  渲染标签管理器
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-03-12
         */
        GraphDataProviderMark* getMarkProvider(Comp::FITKGraph3DWindowVTK* graphWidget);
    private:
        /**
         * @brief    初始化
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-03-12
         */
        void initialize();
        /**
         * @brief    结束
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-03-12
         */
        void finalize();
    private:
        /**
         * @brief  渲染模型管理表
         * @author BaGuijun (baguijun@163.com)
         * @date   2025-03-12
         */
        QHash<Comp::FITKGraph3DWindowVTK*, GraphDataProviderModel*> _modelProHash = {};
        /**
         * @brief  渲染标签管理表
         * @author BaGuijun (baguijun@163.com)
         * @date   2025-03-12
         */
        QHash<Comp::FITKGraph3DWindowVTK*, GraphDataProviderMark*> _markProHash = {};
    };
}

#endif