/**
 * 
 * @file FITKAbstractAppInitializer.h
 * @brief 初始化器抽象类
 * @author libaojun (libaojunqd@foxmail.com)
 * @date 2024-10-23
 * 
 */
#ifndef _FITK_APP_ABSTRACT_INITIALIZER_H___
#define _FITK_APP_ABSTRACT_INITIALIZER_H___
 
#include "FITKAppFrameworkAPI.h"

namespace AppFrame
{
    /**
     * @brief 程序数据初始化器抽象类
     * @author libaojun (libaojunqd@foxmail.com)
     * @date 2024-10-23
     */
    class FITKAppFrameworkAPI FITKAbstractAppInitializer
    {
    public:
        /**
         * @brief Construct a new FITKAbstractAppInitializer object
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-10-23
         */
        explicit FITKAbstractAppInitializer() = default;
        /**
         * @brief Destroy the FITKAbstractAppInitializer object
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-10-23
         */
        virtual ~FITKAbstractAppInitializer() = 0;
        /**
         * @brief 执行初始化操作，初始化失败将直接退出程序
         * @return true  初始化成功
         * @return false  初始化失败
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-10-23
         */
        virtual bool init() = 0;
    
    };
}


#endif
