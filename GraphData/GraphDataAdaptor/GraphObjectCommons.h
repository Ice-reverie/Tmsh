/**********************************************************************
 * @file   GraphObjectCommons.h
 * @brief  渲染数据公用参数
 * @author BaGuijun (baguijun@163.com)
 * @date   2025-03-27
 *********************************************************************/
#ifndef _GraphObjectCommons_H
#define _GraphObjectCommons_H

#include <QObject>

namespace Graph
{
    enum class ShapeAbsEnum
    {
        None = 0,
        STA_VERTEX,
        STA_EDGE,
        STA_FACE,
        STA_SOLID,
    };

    enum class ShapeType
    {
        None = 0,
        ModelVertex,
        ModelEdge,
        ModelFace,
        ModelSolid,
    };
    /**
     * @brief  渲染数据公用参数
     * @author BaGuijun (baguijun@163.com)
     * @date   2025-03-27
     */
    class GraphObjectCommons : public QObject
    {
    public:
        /**
         * @brief    构造函数
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-03-27
         */
        GraphObjectCommons() = default;
        /**
         * @brief    析构函数
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-03-27
         */
        ~GraphObjectCommons() = default;
    public:
        /**
         * @brief  渲染颜色数值名称
         * @author BaGuijun (baguijun@163.com)
         * @date   2025-03-27
         */
        static QString _dataColorArrayName;
        /**
         * @brief  点id存储数据名称
         * @author BaGuijun (baguijun@163.com)
         * @date   2025-04-11
         */
        static QString _dataPointIDArrayName;
        /**
         * @brief  单元id存储数据名称
         * @author BaGuijun (baguijun@163.com)
         * @date   2025-04-11
         */
        static QString _dataCellIDArrayName;
        /**
         * @brief  几何点渲染颜色
         * @author BaGuijun (baguijun@163.com)
         * @date   2025-03-27
         */
        static QColor _geoPointDefaultColor;
        /**
         * @brief  几何线渲染颜色
         * @author BaGuijun (baguijun@163.com)
         * @date   2025-03-27
         */
        static QColor _geoCurveDefaultColor;
        /**
         * @brief  几何面渲染颜色
         * @author BaGuijun (baguijun@163.com)
         * @date   2025-03-27
         */
        static QColor _geoFaceDefaultColor;
        /**
         * @brief  几何体渲染颜色
         * @author BaGuijun (baguijun@163.com)
         * @date   2025-03-27
         */
        static QColor _geoSolidDefaultColor;
        /**
         * @brief  网格面默认颜色
         * @author BaGuijun (baguijun@163.com)
         * @date   2025-04-11
         */
        static QColor _meshFaceDefaultColor;
        /**
         * @brief  几何点默认大小
         * @author BaGuijun (baguijun@163.com)
         * @date   2025-03-27
         */
        static double _geoPointDefaultSize;
        /**
         * @brief  几何线默认大小
         * @author BaGuijun (baguijun@163.com)
         * @date   2025-03-27
         */
        static double _geoLineDefaultSize;
        /**
         * @brief  几何渲染偏移参数
         * @author BaGuijun (baguijun@163.com)
         * @date   2025-04-15
         */
        static double _geoGraphOffset;
        /**
         * @brief  2D网格(面网格)渲染偏移参数
         * @author BaGuijun (baguijun@163.com)
         * @date   2025-04-15
         */
        static double _mesh2DGraphOffset;
        /**
         * @brief  3D网格(面网格)渲染偏移参数
         * @author BaGuijun (baguijun@163.com)
         * @date   2025-04-15
         */
        static double _mesh3DGraphOffset;
    };
}

#endif