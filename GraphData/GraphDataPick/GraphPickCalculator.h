/*****************************************************************//**
 * @file    GraphPickCalculator.h
 * @brief   拾取数据计算生成类。
 *
 * @author  ChengHaotian (yeguangbaozi@foxmail.com)
 * @date    2024-07-19
 *********************************************************************/

#ifndef __GraphPickCalculator_H__
#define __GraphPickCalculator_H__

#include <QObject>

#include "GraphDataPickAPI.h"
#include "GraphDataPickInfo.h"

// 前置声明
class vtkCell;
class vtkIdList;
class vtkIntArray;
class vtkUnstructuredGrid;

namespace Graph
{
    class GraphPickData;
    /**
     * @brief   拾取数据计算生成类。
     * @author  ChengHaotian (yeguangbaozi@foxmail.com)
     * @date    2024-07-19
     */
    class GraphDataPickAPI GraphPickCalculator : public QObject
    {
        Q_OBJECT;
    public:
        /**
         * @brief   构造函数。
         * @param   pickedInfo：拾取信息
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-07-19
         */
        GraphPickCalculator(GraphPickData* pickedInfo);

        /**
         * @brief   析构函数。
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-07-19
         */
        ~GraphPickCalculator() = default;

        /**
         * @brief   根据拾取类型与相关信息进行计算，获取实际需要高亮数据。
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-07-19
         */
        void calculate();

    private:
        /**
         * @brief    单独拾取模式数据生成
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-03-31
         */
        void bySingle();
        /**
         * @brief    框选拾取模式数据生成
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-03-31
         */
        void byAreaPick();
        /**
         * @brief    单独拾取几何
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-03-31
         */
        void bySingleGeometry();
        /**
         * @brief    框选拾取几何
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-03-31
         */
        void byAreaGeometry();

    private:
        /**
         * @brief   需要计算的拾取数据。
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-07-19
         */
        Graph::GraphPickData *m_pickedData{ nullptr };

    };
}

#endif // __PICKEDDATACALCULATOR_H__
