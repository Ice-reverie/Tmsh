#ifndef _GUI_Dialog_API_H_
#define _GUI_Dialog_API_H_

#include <QtCore/QtGlobal>


#if defined(GUIDialog_API)
#define GUIDialogAPI Q_DECL_EXPORT
#else
#define GUIDialogAPI Q_DECL_IMPORT
#endif

#endif
