/**********************************************************************
 * @file   OperWorkingDir.h
 * @brief  工作路径操作器
 * @author guqingtao (15598887859@163.com)
 * @date   2025-04-07
 *********************************************************************/
#ifndef _OPER_GUI_WORKINGDIR_H___
#define _OPER_GUI_WORKINGDIR_H___

#include "FITK_Kernel/FITKCore/FITKActionOperator.h"
#include "FITK_Kernel/FITKCore/FITKOperatorRepo.h"


namespace GUIOper
{
    /**
     * @brief  工作路径操作器
     * @author guqingtao (15598887859@163.com)
     * @date   2025-04-07
     */
    class OperWorkingDir :public Core::FITKActionOperator
    {
 
    public:
        
        /**
         * @brief    Construct a new Oper Working Dir object
         * @return   void
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-04-07
         */
        explicit OperWorkingDir() = default;
        /**
         * @brief    Destroy the Oper Working Dir object
         * @return   void
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-04-07
         */
        virtual ~OperWorkingDir() = default;
        /**
         * @brief    执行GUI操作程序，选择文件对话框
         * @return   bool
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-04-07
         */
        virtual bool execGUI() override;
 
    };

    // 按钮注册相关操作

    Register2FITKOperatorRepo(actionWorkingDir, OperWorkingDir);
}


#endif
