/**********************************************************************
 * @file   OperGeometryTransformation.h
 * @brief  特征变换操作器
 * @author guqingtao (15598887859@163.com)
 * @date   2025-04-03
 *********************************************************************/

#ifndef __OPERATORS_GEOMETRY_TRANSFORMATION_____
#define __OPERATORS_GEOMETRY_TRANSFORMATION_____

#include "FITK_Kernel/FITKCore/FITKActionOperator.h"
#include "FITK_Kernel/FITKCore/FITKOperatorRepo.h"
#include "PluginGeoAbstractAPI.h"

namespace ModelOper
{
    class  PluginGeoAbstractAPI OperGeometryTransformation :public Core::FITKActionOperator
    {
        Q_OBJECT;
    public:
        /**
         * @brief    特征变换操作器
         * @return   void
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-04-03
         */
        explicit OperGeometryTransformation() = default;
        virtual ~OperGeometryTransformation() = default;

        /**
         * @brief 执行界面操作
         * @return true     执行成功
         * @return false     执行失败
         * @author guqingtao (15598887859@163.com)
         * @date 2025-04-03
         */
        virtual bool execGUI() override;
        /**
         * @brief 执行读入操作
         * @return true     执行成功
         * @return false     执行失败
         * @author guqingtao (15598887859@163.com)
         * @date 2025-04-03
         */
        virtual bool execProfession() override;

    private:
        bool createGeometryMirror();
        bool createGeometryRecPattern();
        bool createGeometryCirPattern();

    };
}
#endif