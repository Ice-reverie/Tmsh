/**
 *
 * @file FITKAdaptorAPI.h
 * @brief 声明接口
 * @author LiBaojun (libaojunqd@foxmail.com)
 * @date 2024-03-07
 *
 */
#ifndef _FITKADAPTOR_API_H_
#define _FITKADAPTOR_API_H_

#include <QtCore/QtGlobal>


#if defined(FITKAdaptor_API)
#define FITKAdaptorAPI Q_DECL_EXPORT
#else
#define FITKAdaptorAPI Q_DECL_IMPORT
#endif


#ifndef FITKAdaptorVersion
#define FITKAdaptorVersion "0.5.0"
#endif

#endif
