/**
 *
 * @file FITKInterfaceIOAPI.h
 * @brief  声明接口
 * @author LiBaojun (libaojunqd@foxmail.com)
 * @date 2024-03-08
 *
 */
#ifndef _FITKINTERFACE_IO_API_H_
#define _FITKINTERFACE_IO_API_H_

#include <QtCore/QtGlobal>


#if defined(FITKInterfaceIO_API)
#define FITKInterfaceIOAPI Q_DECL_EXPORT
#else
#define FITKInterfaceIOAPI Q_DECL_IMPORT
#endif

#ifndef FITKInterfaceIOVersion
#define FITKInterfaceIOVersion "1.0.1"
#endif

#endif
