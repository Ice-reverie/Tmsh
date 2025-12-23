/**
 *
 * @file ActionEventOperator.h
 * @brief action点击事件处理基
 * @author LiBaojun (libaojunqd@foxmail.com)
 * @date 2024-03-25
 *
 */

#ifndef __FITKACTIONOPERATOR_H___
#define __FITKACTIONOPERATOR_H___

#include "FITKAbstractOperator.h"

namespace Core
{
    /**
    * @brief Action点击事件处理器，实现事件的处理逻辑，抽象类
    * @author LiBaojun (libaojunqd@foxmail.com)
    * @date 2024-03-25
    */
    class FITKCoreAPI FITKActionOperator : public FITKAbstractOperator
    {
    public:
        explicit FITKActionOperator() = default;
        virtual ~FITKActionOperator() = 0;


        /**
         * @brief 点击事件处理方法
         * @return true      执行成功
         * @return false     执行失败
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-25
         */
        virtual bool actionTriggered();

        /**
         * @brief 界面逻辑，生成UI交互
         * @return true             执行成功
         * @return false            执行失败
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-25
         */
        virtual bool execGUI();
        /**
         * @brief 业务处理逻辑，在execGUI后执行
         * @return true             执行成功
         * @return false            执行失败
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-25
         */
        virtual bool execProfession();


    protected:
        /**
         * @brief 参数预处理
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-25
         */
        virtual void preArgs();
    };

}



#endif
