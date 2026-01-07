/**********************************************************************
 * @file   FITKTmshExeDriverAPI.h
 * @brief  外部接口声明
 * @date   2025-12-22
 *********************************************************************/
#ifndef _FITKTMSHEXEDRIVER_API_H_
#define _FITKTMSHEXEDRIVER_API_H_

#include <QtCore/QtGlobal>

#if defined(FITKTmshExeDriver_API)
#define FITKTmshExeDriverAPI Q_DECL_EXPORT
#else
#define FITKTmshExeDriverAPI Q_DECL_IMPORT
#endif

#ifndef FITKTMSHEXEDRIVERVERSION
#define FITKTMSHEXEDRIVERVERSION "1.0.0"
#endif

#endif // _FITKTMSHEXEDRIVER_API_H_

