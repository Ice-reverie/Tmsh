#ifndef _GUI_Widget_API_H_
#define _GUI_Widget_API_H_

#include <QtCore/QtGlobal>


#if defined(GUIWidget_API)
#define GUIWidgetAPI Q_DECL_EXPORT
#else
#define GUIWidgetAPI Q_DECL_IMPORT
#endif

#endif
