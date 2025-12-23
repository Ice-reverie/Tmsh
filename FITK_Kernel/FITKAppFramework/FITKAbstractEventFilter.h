/**
 *
 * @file FITKAbstractEventFilter.h
 * @brief 自定义事件处理器抽象类
 * @author LiBaojun (libaojunqd@foxmail.com)
 * @date 2024-04-30
 *
 */
#ifndef __FITKABSTRACTEVENTFILTER_H___
#define __FITKABSTRACTEVENTFILTER_H___

#include "FITKAppFrameworkAPI.h"
#include "FITK_Kernel/FITKCore/FITKAbstractObject.hpp"

namespace AppFrame
{
    /**
     * @brief 自定义事件处理器抽象类
     * @author LiBaojun (libaojunqd@foxmail.com)
     * @date 2024-04-30
     */
    class FITKAppFrameworkAPI FITKAbstractEventFilter : public Core::FITKAbstractObject
    {
    public:
        /**
         * @brief Construct a new FITKAbstractEventFilter object
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-04-30
         */
        explicit FITKAbstractEventFilter() = default;
        /**
         * @brief Destroy the FITKAbstractEventFilter object
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-04-30
         */
        virtual ~FITKAbstractEventFilter() = 0;
        /**
         * @brief 事件过滤器
         * @param[i]  Object  事件发出对象
         * @param[i]  event   事件对象
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-04-30
         */
        virtual void eventFilter(QObject *Object, QEvent *event) = 0;
    };
}

#endif // !__FITKABSTRACTEVENTFILTER_H___
