#ifndef GUIDETAILEDPARAMETERS_H
#define GUIDETAILEDPARAMETERS_H

#include <QWidget>

#include "FITK_Kernel/FITKCore/FITKAbstractGUIDialog.h"
#include "FITKTmshExeDriverAPI.h"
#include "GUITmshSettings.h"
#include "ui_GUITmshSettings.h"

namespace Ui {
class GUIDetailedParameters;
}

namespace Interface
{
    class VirtualShape;
    class FITKAbstractMesherDriver;
}

namespace GUI
{
    class FITKTmshExeDriverAPI GUIDetailedParameters : public Core::FITKDialog
    {
        Q_OBJECT
    public:
        GUIDetailedParameters(QWidget* parent = nullptr);
        GUIDetailedParameters(Interface::FITKAbstractMesherDriver* driver, QWidget* parent = nullptr);
        ~GUIDetailedParameters();

    private:
        void init();

    private slots:
        void on_pushButton_OK_clicked();

    private:
        Ui::GUIDetailedParameters* _ui{};
        Interface::FITKAbstractMesherDriver* _driver{};
        QList<Interface::VirtualShape> _virtualTopos{};
        GUIDetailedParameters* _detailedDlg{};
    };
}

#endif  // GUIDETAILEDPARAMETERS_H

