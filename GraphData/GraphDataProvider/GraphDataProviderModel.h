/**********************************************************************
 * @file   GraphDataProviderModel.h
 * @brief  渲染模型管理
 * @author BaGuijun (baguijun@163.com)
 * @date   2025-03-12
 *********************************************************************/
#ifndef GraphDataProviderModel_H
#define GraphDataProviderModel_H

#include "GraphDataProviderAPI.h"
#include "GraphDataProviderBase.h"

#include <QObject>
#include <QHash>

namespace Comp {
    class FITKGraph3DWindowVTK;
}

namespace Graph
{
    class GraphObjectBase;
    /**
     * @brief  渲染模型管理
     * @author BaGuijun (baguijun@163.com)
     * @date   2025-03-12
     */
    class GraphDataProviderAPI GraphDataProviderModel : public GraphDataProviderBase
    {
        Q_OBJECT;
    public:
        /**
         * @brief    构造函数
         * @param[i] graphWidget                           渲染界面
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-03-12
         */
        GraphDataProviderModel(Comp::FITKGraph3DWindowVTK* graphWidget);
        /**
         * @brief    析构函数
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-03-12
         */
        ~GraphDataProviderModel();
        /**
         * @brief    获取几何渲染对象
         * @param[i] objId                     数据id
         * @return   GraphObjectBase*          渲染对象
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-03-12
         */
        GraphObjectBase* getGeoModelGraph(int objId);
        /**
         * @brief    获取网格渲染对象
         * @param[i] objId                     数据id
         * @return   GraphObjectBase*          渲染对象
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-04-10
         */
        GraphObjectBase* getMeshKernelGraph(int objId);
        /**
         * @brief    设置几何点是否可拾取
         * @param[i] isPick               是否可视化
         * @param[i] objID                对象id(-1默认是全部对象)
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-03-13
         */
        void setGeoModelVertexPick(bool isPick, int objID = -1);
        /**
         * @brief    设置几何线是否可拾取
         * @param[i] isPick               是否可视化
         * @param[i] objID                对象id(-1默认是全部对象)
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-03-13
         */
        void setGeoModelCurverPick(bool isPick, int objID = -1);
        /**
         * @brief    设置几何面是否可拾取
         * @param[i] isPick               是否可视化
         * @param[i] objID                对象id(-1默认是全部对象)
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-03-13
         */
        void setGeoModelFacePick(bool isPick, int objID = -1);
        /**
         * @brief    设置几何体是否可拾取
         * @param[i] isPick               是否可视化
         * @param[i] objID                对象id(-1默认是全部对象)
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-03-13
         */
        void setGeoModelSolidPick(bool isPick, int objID = -1);
    private:
        /**
         * @brief  几何渲染对象链表
         * @author BaGuijun (baguijun@163.com)
         * @date   2025-03-12
         */
        QHash<int, GraphObjectBase*> _geoModelObjs = {};
        /**
         * @brief  网格渲染对象链表
         * @author BaGuijun (baguijun@163.com)
         * @date   2025-03-12
         */
        QHash<int, GraphObjectBase*> _meshKernelObjs = {};
    };
}

#endif