/**********************************************************************
 * @file   OperGeometryBool.h
 * @brief  bool运算操作器
 * @author guqingtao (15598887859@163.com)
 * @date   2025-04-01
 *********************************************************************/

#ifndef __OPERATORS_GEOMETRY_BOOL_____
#define __OPERATORS_GEOMETRY_BOOL_____

#include "FITK_Kernel/FITKCore/FITKActionOperator.h"
#include "FITK_Kernel/FITKCore/FITKOperatorRepo.h"
#include "PluginGeoAbstractAPI.h"

namespace ModelOper
{
    class  PluginGeoAbstractAPI OperGeometryBool :public Core::FITKActionOperator
    {
        Q_OBJECT;
    public:
        /**
         * @brief    bool操作器
         * @return   void
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-04-01
         */
        explicit OperGeometryBool() = default;
        virtual ~OperGeometryBool() = default;

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
        bool createGeometryUnite();
        bool createGeometryIntersect();
        bool createGeometryDifference();
    };

    // 按钮注册相关操作
    //Register2FITKOperatorRepo(actionCreateBox, OperGeometryBox);
}
#endif