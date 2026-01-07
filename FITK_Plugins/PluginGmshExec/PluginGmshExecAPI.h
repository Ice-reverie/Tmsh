 
#ifndef _PLUGIN_GMSH_EXEC_API_H_
#define _PLUGIN_GMSH_EXEC_API_H_

#include <QtCore/QtGlobal>


#if defined(PluginGmshExec_API)
#define PluginGmshExecAPI Q_DECL_EXPORT
#else
#define PluginGmshExecAPI Q_DECL_IMPORT
#endif

#endif
