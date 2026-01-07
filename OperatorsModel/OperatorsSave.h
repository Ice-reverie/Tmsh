/**********************************************************************
 * @file   OperatorsSave.h
 * @brief  保存工程文件操作器
 * @author liuzhonghua (liuzhonghuaszch@163.com)
 * @date   2025-04-11
 *********************************************************************/
#ifndef _OPERATORSSAVE_H
#define _OPERATORSSAVE_H

#include "FITK_Kernel/FITKCore/FITKActionOperator.h"
#include "FITK_Kernel/FITKCore/FITKOperatorRepo.h"

namespace ModelOper
{
    /**
     * @brief  保存工程文件操作器
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date   2025-04-11
     */
    class OperatorsSave :public Core::FITKActionOperator
    {
        Q_OBJECT
    public:
        /**
         * @brief  Construct a new Operators Save object
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-04-11
         */
        explicit OperatorsSave() = default;
        /**
         * @brief  Destroy the Operators Save object
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-04-11
         */
        virtual ~OperatorsSave() = default;
        /**
         * @brief  执行界面操作
         * @return true  执行成功
         * @return false  执行失败
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-04-11
         */
        virtual bool execGUI() override;
        /**
         * @brief  执行写出操作
         * @return true  执行成功
         * @return false  执行失败
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-04-11
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
    Register2FITKOperatorRepo(actionSave, OperatorsSave);
    Register2FITKOperatorRepo(actionSaveAs, OperatorsSave);
}

#endif