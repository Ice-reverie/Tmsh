/**
 *
 * @file FITKAppFrameworkAPI.h
 * @brief 声明接口
 * @author LiBaojun (libaojunqd@foxmail.com)
 * @date 2024-03-07
 *
 */
#ifndef _FITKAPPFRAMEWORK_API_H_
#define _FITKAPPFRAMEWORK_API_H_

#include <QtCore/QtGlobal>


#if defined(FITKAppFramework_API)
#define FITKAppFrameworkAPI Q_DECL_EXPORT
#else
#define FITKAppFrameworkAPI Q_DECL_IMPORT
#endif

#ifndef FITKAppFrameworkVersion
#define FITKAppFrameworkVersion "1.0.7"
#endif

#endif
