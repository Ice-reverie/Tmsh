 /**
 *
 * @file FITKInterfaceModelAPI.h
 * @brief API接口声明
 * @author LiBaojun (libaojunqd@foxmail.com)
 * @date 2024-02-27
 *
 */
#ifndef _FITKINTERFACEMODEL_API_H_
#define _FITKINTERFACEMODEL_API_H_

#include <QtCore/QtGlobal>


#if defined(FITKInterfaceModel_API)
#define FITKInerfaceModelAPI Q_DECL_EXPORT
#else
#define FITKInerfaceModelAPI Q_DECL_IMPORT
#endif

#ifndef FITKInterfaceModelVersion
#define FITKInterfaceModelVersion "0.9.1"
#endif

#endif
