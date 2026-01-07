/**********************************************************************
 * @file   GraphDataPickInfo.h
 * @brief  拾取状态信息
 * @author BaGuijun (baguijun@163.com)
 * @date   2025-03-12
 *********************************************************************/
#ifndef __GraphDataPickInfo_H__
#define __GraphDataPickInfo_H__

#include <QObject>
#include "GraphDataPickAPI.h"

#define PickObjTypeStr "PickObjTypeStr"
#define PickMethodStr "PickMethodStr"
#define AutoClearStr "AutoClearStr"

namespace Graph
{
    /**
     * @brief  拾取对象类型
     * @author BaGuijun (baguijun@163.com)
     * @date   2025-03-12
     */
    enum class PickObjType
    {
        PickNone = 0,                  //关闭
        PickGeoVertex,                 //几何点
        PickGeoCurve,                  //几何线
        PickGeoFace,                   //几何面
        PickGeoSolid,                  //几何体
    };

    /**
     * @brief  拾取方式。
     * @author BaGuijun (baguijun@163.com)
     * @date   2025-03-12
     */
    enum class PickMethod
    {
        PMNone = 0,            // 关闭。           
        PMIndividually,        // 默认拾取方式。（点选+框选+追加拾取）           
        PMSingle,              // 单独数据拾取方式。（点选）         
    };

    /**
     * @brief  拾取方式信息结构体。
     * @author BaGuijun (baguijun@163.com)
     * @date   2025-03-12
     */
    struct GraphDataPickInfoStru
    {
        // 拾取对象类型。
        PickObjType _pickObjType{ PickObjType::PickNone };

        // 拾取方法。
        PickMethod _pickMethod{ PickMethod::PMNone };

        // 拾取空白区域自动清空已拾取数据。
        bool _autoClear = true;
    };

    /**
     * @brief  拾取方法设置
     * @author BaGuijun (baguijun@163.com)
     * @date   2025-03-12
     */
    class GraphDataPickAPI GraphDataPickInfo : public QObject
    {
        Q_OBJECT;
    public:
        /**
         * @brief   清除拾取状态信息。
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-10-31
         */
        static void ClearPickInfo();

        /**
         * @brief   设置拾取方式信息。
         * @param   info：拾取方式信息
         * @param   dataObjId：开启拾取状态的数据ID[缺省]
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-10-31
         */
        static void SetPickInfo(GraphDataPickInfoStru info, int dataObjId = -1);

        /**
         * @brief   获取拾取方式信息。
         * @return  拾取方式信息
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-10-31
         */
        static GraphDataPickInfoStru GetPickInfo();

    private:
        /**
         * @brief   构造函数。
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-10-31
         */
        GraphDataPickInfo() = default;

        /**
         * @brief   析构函数。
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-10-31
         */
        ~GraphDataPickInfo() = default;

    };


}

#endif // __GraphDataPickInfo_H__
