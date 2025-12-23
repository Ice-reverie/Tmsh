 
#ifndef _PLUGIN_TMSH_EXEC_API_H_
#define _PLUGIN_TMSH_EXEC_API_H_

#include <QtCore/QtGlobal>

#if defined(PluginTmshExec_API)
#define PluginTmshExecAPI Q_DECL_EXPORT
#else
#define PluginTmshExecAPI Q_DECL_IMPORT
#endif

#endif

