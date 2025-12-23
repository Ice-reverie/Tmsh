/**********************************************************************
 * @file   GraphDataProviderBase.h
 * @brief  渲染管理基类
 * @author BaGuijun (baguijun@163.com)
 * @date   2025-03-12
 *********************************************************************/
#ifndef GraphDataProviderBase_H
#define GraphDataProviderBase_H

#include "GraphDataProviderAPI.h"

#include <QObject>

namespace Core {
    class FITKAbstractDataObject;
}

namespace Comp {
    class FITKGraph3DWindowVTK;
}

namespace Interface {
    class FITKAbsGeoCommand;
}

namespace Graph
{
    class GraphObjectBase;
    /**
     * @brief  渲染管理基类
     * @author BaGuijun (baguijun@163.com)
     * @date   2025-03-12
     */
    class GraphDataProviderAPI GraphDataProviderBase : public QObject
    {
        Q_OBJECT;
    public:
        /**
         * @brief    构造函数
         * @param[i] graphWidget                           渲染界面
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-03-12
         */
        GraphDataProviderBase(Comp::FITKGraph3DWindowVTK* graphWidget);
        /**
         * @brief    析构函数
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-03-12
         */
        ~GraphDataProviderBase();
    protected:
        /**
         * @brief    获取渲染界面
         * @return   Comp::FITKGraph3DWindowVTK*            渲染界面
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-03-12
         */
        Comp::FITKGraph3DWindowVTK* getGraphWidget();
        /**
         * @brief    获取渲染对象
         * @param[i] KeyName               适配器key
         * @param[i] objDict               渲染对象管理链表
         * @param[i] dataObj               数据对象
         * @return   GraphObjectBase*      渲染对象
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-03-12
         */
        GraphObjectBase* getGraphObject(QString KeyName, QHash<int, GraphObjectBase*>& objDict, Core::FITKAbstractDataObject* dataObj);
        /**
         * @brief    获取几何渲染对象
         * @param[i] keyName               渲染适配器key
         * @param[i] objDict               渲染对象管理链表
         * @param[i] geoCom                几何数据对象
         * @return   GraphObjectBase*      渲染对象
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-03-31
         */
        GraphObjectBase* getGeoGraphObject(QString keyName, QHash<int, GraphObjectBase*>& objDict, Interface::FITKAbsGeoCommand* geoCom);
    private:
        /**
         * @brief  渲染界面
         * @author BaGuijun (baguijun@163.com)
         * @date   2025-03-12
         */
        Comp::FITKGraph3DWindowVTK* _graphWidget = nullptr;
    };
}

#endif