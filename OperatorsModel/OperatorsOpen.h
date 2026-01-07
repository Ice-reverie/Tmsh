/**********************************************************************
 * @file   OperatorsOpen.h
 * @brief  创建打开工程文件事件响应
 * @author liuzhonghua (liuzhonghuaszch@163.com)
 * @date   2024-09-25
 *********************************************************************/
#ifndef _OPERATORSOPEN_H
#define _OPERATORSOPEN_H

#include "FITK_Kernel/FITKCore/FITKActionOperator.h"
#include "FITK_Kernel/FITKCore/FITKOperatorRepo.h"

namespace ModelOper
{
    /**
     * @brief  打开工程文件操作器
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date   2025-04-11
     */
    class OperatorsOpen :public Core::FITKActionOperator
    {
        Q_OBJECT;
    public:
        /**
         * @brief    Construct a new Operators Open object
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-04-11
         */
        explicit OperatorsOpen() = default;
        /**
         * @brief    Destroy the Operators Open object
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-04-11
         */
        virtual ~OperatorsOpen() = default;
        /**
         * @brief    执行界面操作
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-04-11
         */
        virtual bool execGUI() override;
        /**
         * @brief    执行读入操作
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-04-11
         */
        virtual bool execProfession() override;

    private slots:
        /**
         * @brief  读取线程结束执行该槽函数
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-04-11
         */
        void ioThreadFinishedSlot();
    };

    // 按钮注册相关操作
    Register2FITKOperatorRepo(actionOpen, OperatorsOpen);
}

#endif