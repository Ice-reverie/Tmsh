/**********************************************************************
 * @file   FITKCompareTool.h
 * @brief  对比工具
 * @author BaGuijun (baguijun@163.com)
 * @date   2025-04-14
 *********************************************************************/
#ifndef _FITKCompareTool_H
#define _FITKCompareTool_H

#include "FITKVTKAlgorithmAPI.h"
#include <QObject>

class vtkDataSet;

/**
 * @brief  对比工具
 * @author BaGuijun (baguijun@163.com)
 * @date   2025-04-14
 */
class FITKVTKALGORITHMAPI FITKCompareTool :public QObject
{
    Q_OBJECT;
public:
    /**
     * @brief    构造函数
     * @author   BaGuijun (baguijun@163.com)
     * @date     2025-04-14
     */
    FITKCompareTool() = default;
    /**
     * @brief    析构函数
     * @author   BaGuijun (baguijun@163.com)
     * @date     2025-04-14
     */
    ~FITKCompareTool() = default;
    /**
     * @brief    从点、单元角度对比两个VTK数据集是否一致
     * @param[i] data1                          数据集1
     * @param[i] data2                          数据集2
     * @return   bool                           是否一致
     * @author   BaGuijun (baguijun@163.com)
     * @date     2025-04-14
     */
    static bool CompareVTKData(vtkDataSet* data1, vtkDataSet* data2);
    /**
     * @brief    从点角度对比两个VTK数据集是否一致
     * @param[i] data1                          数据集1
     * @param[i] data2                          数据集2
     * @return   bool                           是否一致
     * @author   BaGuijun (baguijun@163.com)
     * @date     2025-04-14
     */
    static bool ComparePoints(vtkDataSet* data1, vtkDataSet* data2);
    /**
     * @brief    从单元角度对比两个VTK数据集是否一致
     * @param[i] data1                          数据集1
     * @param[i] data2                          数据集2
     * @return   bool                           是否一致
     * @author   BaGuijun (baguijun@163.com)
     * @date     2025-04-14
     */
    static bool CompareCells(vtkDataSet* data1, vtkDataSet* data2);
};

#endif