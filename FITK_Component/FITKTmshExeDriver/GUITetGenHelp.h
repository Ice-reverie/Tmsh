#ifndef GUITETGENHELP_H
#define GUITETGENHELP_H

#include <QWidget>

#include "FITK_Kernel/FITKCore/FITKAbstractGUIDialog.h"
#include "FITKTmshExeDriverAPI.h"

#include "GUITetGenSettings.h"
#include "ui_GUITetGenSettings.h"

namespace Ui {
class GUITetGenHelp;
}

namespace Interface
{
    class VirtualShape;
    class FITKAbstractMesherDriver;
}

namespace GUI
{
    class FITKTmshExeDriverAPI GUITetGenHelp : public Core::FITKDialog
    {
        Q_OBJECT
    public:
        GUITetGenHelp(QWidget* parent = nullptr);
        GUITetGenHelp(Interface::FITKAbstractMesherDriver* driver, QWidget* parent = nullptr);
        ~GUITetGenHelp();

    private:
        void init();

    private slots:
        void on_pushButton_Back_clicked();

    private:
        Ui::GUITetGenHelp* _ui{};
        Interface::FITKAbstractMesherDriver* _driver{};
        QList<Interface::VirtualShape> _virtualTopos{};
        GUITetGenHelp* _detailedDlg{};
    };
}

#endif  // GUITETGENHELP_H
