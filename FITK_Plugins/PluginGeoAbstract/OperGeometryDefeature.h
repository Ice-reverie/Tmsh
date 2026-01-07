/**********************************************************************
 * @file   OperGeometryDefeature.h
 * @brief  特征移除操作器
 * @author guqingtao (15598887859@163.com)
 * @date   2025-04-01
 *********************************************************************/

#ifndef __OPER_GEOMETRY_DEFEATURE_____
#define __OPER_GEOMETRY_DEFEATURE_____

#include "PluginGeoAbstractAPI.h"

#include "FITK_Kernel/FITKCore/FITKActionOperator.h"
#include "FITK_Kernel/FITKCore/FITKOperatorRepo.h"

namespace ModelOper
{
    class PluginGeoAbstractAPI OperGeometryDefeature :public Core::FITKActionOperator
    {
        Q_OBJECT;
    public:
        /**
         * @brief    特征移除操作器
         * @return   void
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-04-01
         */
        explicit OperGeometryDefeature() = default;
        virtual ~OperGeometryDefeature() = default;

        /**
         * @brief 执行界面操作
         * @return true     执行成功
         * @return false     执行失败
         * @author guqingtao (15598887859@163.com)
         * @date 2025-04-01
         */
        virtual bool execGUI() override;
        /**
         * @brief 执行读入操作
         * @return true     执行成功
         * @return false     执行失败
         * @author guqingtao (15598887859@163.com)
         * @date 2025-04-01
         */
        virtual bool execProfession() override;

    private:
        bool createGeometryChamfer();
        bool createGeometryFIllet();
        bool createGeometryDefeature();

    };
}
#endif