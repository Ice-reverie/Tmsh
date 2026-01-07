#include "FITKPythonQtScriptingConsole.h"

#include <QKeyEvent>

namespace Python
{
    FITKPythonQtScriptingConsole::FITKPythonQtScriptingConsole(QWidget * parent, const PythonQtObjectPtr & context, Qt::WindowFlags i) :
        PythonQtScriptingConsole(parent, context, i)
    {

    }

    FITKPythonQtScriptingConsole::~FITKPythonQtScriptingConsole()
    {

    }

    void FITKPythonQtScriptingConsole::keyPressEvent(QKeyEvent * event)
    {
        //Ctrl + Z崩溃
        if ((event->modifiers()& Qt::ControlModifier) != 0 && event->key() == Qt::Key_Z){
            return;
        }
        else {
            PythonQtScriptingConsole::keyPressEvent(event);
        }
    }
}