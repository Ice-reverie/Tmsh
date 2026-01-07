/**********************************************************************
 * @file   OperGmshGlobalSize.h
 * @brief  Gmsh网格尺寸操作器
 * @author guqingtao (15598887859@163.com)
 * @date   2025-06-05
 *********************************************************************/
#ifndef __OPERGMSHGLOBALSIZE____
#define __OPERGMSHGLOBALSIZE____

#include "FITK_Kernel/FITKCore/FITKActionOperator.h"
#include "FITK_Kernel/FITKCore/FITKOperatorRepo.h"
#include "PluginGmshExecAPI.h"

namespace ModelOper
{
    class  PluginGmshExecAPI OperGmshGlobalSize :public Core::FITKActionOperator
    {
        Q_OBJECT;
    public:
        /**
         * @brief    Gmsh网格尺寸操作器
         * @return   void
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-06-06
         */
        explicit OperGmshGlobalSize() = default;
        virtual ~OperGmshGlobalSize() = default;

        /**
         * @brief 执行界面操作
         * @return true     执行成功
         * @return false     执行失败
         * @author guqingtao (15598887859@163.com)
         * @date 2025-06-06
         */
        virtual bool execGUI() override;
        /**
         * @brief 执行读入操作
         * @return true     执行成功
         * @return false     执行失败
         * @author guqingtao (15598887859@163.com)
         * @date 2025-06-06
         */
        virtual bool execProfession() override;

    private:
        bool updateGlobalSize();

    };
}
#endif