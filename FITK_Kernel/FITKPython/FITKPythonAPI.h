/**
 * 
 * @file FITKPythonAPI.h
 * @brief 导入导出库声明
 * @author BaGuijun (baguijun@163.com)
 * @date 2024-02-26
 * 
 */
#ifndef _FITKPYTHON_API_H_
#define _FITKPYTHON_API_H_

#include <QtCore/QtGlobal>


#if defined(FITKPython_API)
#define FITKPythonAPI Q_DECL_EXPORT
#else
#define FITKPythonAPI Q_DECL_IMPORT
#endif

#ifndef FITKPythonVersion
#define FITKPythonVersion "1.0.0"   
#endif

#endif
