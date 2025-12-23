/**
 * 
 * @file FITKCompMessageWidgetAPI.h
 * @brief 导入导出库声明 
 * @author BaGuijun (baguijun@163.com)
 * @date 2024-03-05
 * 
 */
#ifndef _FITKCompMessageWidgetAPI_H_
#define _FITKCompMessageWidgetAPI_H_

#include <QtCore/QtGlobal>

#if defined(FITKCompMessageWidget_API)
#define CompMesWidgetAPI Q_DECL_EXPORT
#else
#define CompMesWidgetAPI Q_DECL_IMPORT
#endif

#ifndef FITKCOMPMESSAGEWIDGETVERSION
#define FITKCOMPMESSAGEWIDGETVERSION "1.0.0"
#endif

#endif
