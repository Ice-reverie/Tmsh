/**
 *
 * @file FITKCoreAPI.h
 * @brief 导入导出库声明
 * @author LiBaojun (libaojunqd@foxmail.com)
 * @date 2024-02-22
 *
 */
#ifndef _FITKCORE_API_H_
#define _FITKCORE_API_H_

#include <QtCore/QtGlobal>


#if defined(FITKCore_API)
#define FITKCoreAPI Q_DECL_EXPORT
#else
#define FITKCoreAPI Q_DECL_IMPORT
#endif

#ifndef FITKCoreVersion
#define FITKCoreVersion "1.0.3"
#endif

#endif


