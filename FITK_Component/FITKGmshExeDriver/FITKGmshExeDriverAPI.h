/**********************************************************************
 * @file   FITKGmshExeDriverAPI.h
 * @brief  外部接口声明
 * @author liuzhonghua (liuzhonghuaszch@163.com)
 * @date   2024-11-27
 *********************************************************************/
#ifndef _FITKGmshExeDriver_API_H_
#define _FITKGmshExeDriver_API_H_

#include <QtCore/QtGlobal>


#if defined(FITKGmshExeDriver_API)
#define FITKGmshExeDriverAPI Q_DECL_EXPORT
#else
#define FITKGmshExeDriverAPI Q_DECL_IMPORT
#endif

#ifndef FITKGMSHEXEDRIVERVERSION
#define FITKGMSHEXEDRIVERVERSION "1.0.0"
#endif

#endif // _FITKGmshExeDriver_API_H_
