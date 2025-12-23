 
#ifndef _PLUGIN_GEO_OCC_API_H_
#define _PLUGIN_GEO_OCC_API_H_

#include <QtCore/QtGlobal>


#if defined(PluginGeoOCC_API)
#define PluginGeoOCCAPI Q_DECL_EXPORT
#else
#define PluginGeoOCCAPI Q_DECL_IMPORT
#endif

#endif
