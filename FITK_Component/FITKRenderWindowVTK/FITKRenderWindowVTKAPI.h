/**
 * 
 * @file FITKRenderWindowVTKAPI.h
 * @brief  导入导出接口声明
 * @author BaGuijun (baguijun@163.com)
 * @date 2024-03-08
 * 
 */
#ifndef _FITKRenderWindowVTKAPI_H_
#define _FITKRenderWindowVTKAPI_H_

#include <QtCore/QtGlobal>

#if defined(FITKRenderWindowVTK_API)
#define FITKRenderWindowVTKAPI Q_DECL_EXPORT
#else
#define FITKRenderWindowVTKAPI Q_DECL_IMPORT
#endif

#ifndef FITKRENDERWINDOWVTKVERSION
#define FITKRENDERWINDOWVTKVERSION "1.0.0"  
#endif

#endif // _FITKRenderWindowVTKAPI_H_
