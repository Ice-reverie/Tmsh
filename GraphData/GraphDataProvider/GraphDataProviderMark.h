/**********************************************************************
 * @file   GraphDataProviderMark.h
 * @brief  渲染标签管理
 * @author BaGuijun (baguijun@163.com)
 * @date   2025-03-12
 *********************************************************************/
#ifndef GraphDataProviderMark_H
#define GraphDataProviderMark_H

#include "GraphDataProviderAPI.h"
#include "GraphDataProviderBase.h"

#include <QObject>

namespace Comp {
    class FITKGraph3DWindowVTK;
}

namespace Graph
{
    /**
     * @brief  渲染标签管理
     * @author BaGuijun (baguijun@163.com)
     * @date   2025-03-12
     */
    class GraphDataProviderAPI GraphDataProviderMark : public GraphDataProviderBase
    {
        Q_OBJECT;
    public:
        /**
         * @brief    构造函数
         * @param[i] graphWidget                           渲染界面
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-03-12
         */
        GraphDataProviderMark(Comp::FITKGraph3DWindowVTK* graphWidget);
        /**
         * @brief    析构函数
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-03-12
         */
        ~GraphDataProviderMark();
    private:
    };
}

#endif