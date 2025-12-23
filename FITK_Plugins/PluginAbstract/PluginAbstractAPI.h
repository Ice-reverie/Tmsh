/**
 * @file   GUIFrameAPI.h
 * @brief  接口宏声明
 * @author libaojun
 * @date   2024-03-19
 */
#ifndef __PLUGIN_ABSTRACT__H__
#define __PLUGIN_ABSTRACT__H__

#include <QtCore/QtGlobal>

#ifdef PluginAbstract_API
#define PluginAbstractAPI Q_DECL_EXPORT
#else
#define PluginAbstractAPI Q_DECL_IMPORT
#endif

#endif // __GUIFRAMEAPI_H__