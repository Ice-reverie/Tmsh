/**
* @file   DrawerWidgetAPI.h
* @brief  自定义抽屉组件接口文件
* @author XuXinwei
* @version 1.0.0
*/
#ifndef _DRAWER_WIDGET_API_H_
#define _DRAWER_WIDGET_API_H_

#include <QtCore/qglobal.h>

#if defined(FITKWidget_API)
#  define FITKWidgetAPI Q_DECL_EXPORT
#else
#  define FITKWidgetAPI Q_DECL_IMPORT
#endif

#ifndef FITKWIDGETVERSION
#define FITKWIDGETVERSION "1.0.0"
#endif

#endif // _DRAWER_WIDGET_API_H_