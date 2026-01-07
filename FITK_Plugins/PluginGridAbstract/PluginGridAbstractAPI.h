/**
 * @file   GUIFrameAPI.h
 * @brief  接口宏声明
 * @author libaojun
 * @date   2024-03-19
 */
#ifndef __PLUGIN_GRID_ABSTRACT__H__
#define __PLUGIN_GRID_ABSTRACT__H__

#include <QtCore/QtGlobal>

#ifdef PluginGridAbstract_API
#define PluginGridAbstractAPI Q_DECL_EXPORT
#else
#define PluginGridAbstractAPI Q_DECL_IMPORT
#endif

#endif // __GUIFRAMEAPI_H__