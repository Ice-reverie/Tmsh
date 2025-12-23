/**
 *
 * @file FITKInterfaceGeometryAPI.h
 * @brief 几何接口声明，导出api接口
 * @author LiBaojun (libaojunqd@foxmail.com)
 * @date 2024-07-12
 *
 */
#ifndef _FITKInterfaceGeometry_API_H_
#define _FITKInterfaceGeometry_API_H_

#include <QtCore/QtGlobal>


#if defined(FITKInterfaceGeometry_API)
#define FITKInterfaceGeometryAPI Q_DECL_EXPORT
#else
#define FITKInterfaceGeometryAPI Q_DECL_IMPORT
#endif


#ifndef FITKInterfaceGeometryVersion
#define FITKInterfaceGeometryVersion "1.0.0"
#endif

#endif
