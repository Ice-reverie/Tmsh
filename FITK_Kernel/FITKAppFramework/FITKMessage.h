/**
 *
 * @file FITKMessage.h
 * @brief  信息输出接口声明
 * @author LiBaojun (libaojunqd@foxmail.com)
 * @date 2024-03-18
 *
 */
#ifndef ___FITKMESSAGE_H___
#define ___FITKMESSAGE_H___

#include "FITKAppFrameworkAPI.h"

namespace AppFrame
{
    /**
     * @brief 输出正常信息
     * @param[i]  message        提示信息
     * @author LiBaojun (libaojunqd@foxmail.com)
     * @date 2024-03-18
     */
    void FITKAppFrameworkAPI FITKMessageNormal(const QString& message);
    /**
     * @brief 输出警告信息
     * @param[i]  message        提示信息
     * @author LiBaojun (libaojunqd@foxmail.com)
     * @date 2024-03-18
     */
    void FITKAppFrameworkAPI FITKMessageWarning(const QString& message);
    /**
     * @brief 输出错误信息
     * @param[i]  message        提示信息
     * @author LiBaojun (libaojunqd@foxmail.com)
     * @date 2024-03-18
     */
    void FITKAppFrameworkAPI FITKMessageError(const QString& message);

    /**
     * @brief 输出信息
     * @param[i]  message        提示信息
     * @author LiBaojun (libaojunqd@foxmail.com)
     * @date 2024-03-18
     */
    void FITKAppFrameworkAPI FITKMessageInfo(const QString& message);


}



#endif
