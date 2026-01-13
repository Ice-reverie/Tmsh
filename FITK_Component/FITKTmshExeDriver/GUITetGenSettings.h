/**********************************************************************
 * @file   GUITetGenSettings.h
 * @brief  TetGen Settings尺寸信息设置
 * @date   2026-1-12
 *********************************************************************/
#ifndef GUITETGENSETTINGS_H
#define GUITETGENSETTINGS_H

#include "FITK_Kernel/FITKCore/FITKAbstractGUIDialog.h"
#include "FITKTmshExeDriverAPI.h"

#include "GUITetGenHelp.h"
#include "ui_GUITetGenHelp.h"

#include <QWidget>

namespace Ui { class GUITetGenSettings; }

namespace Interface
{
    class VirtualShape;
    class FITKAbstractMesherDriver;
}

namespace GUI
{
    class FITKTmshExeDriverAPI GUITetGenSettings : public Core::FITKDialog
    {
        Q_OBJECT
    public:
        GUITetGenSettings(QWidget* parent = nullptr);
        GUITetGenSettings(Interface::FITKAbstractMesherDriver* driver, QWidget* parent = nullptr);
        ~GUITetGenSettings();

    private:
        void init();

    private slots:
        void on_pushButton_OK_clicked();
        void on_pushButton_Cancel_clicked();
        void on_pushButton_Help_clicked();
        void on_pushButton_Browse_clicked();

    private:
        Ui::GUITetGenSettings* _ui{};
        Interface::FITKAbstractMesherDriver* _driver{};
        QList<Interface::VirtualShape> _virtualTopos{};
        GUITetGenSettings* _detailedDlg{};
    };
}

#endif // GUITETGENSETTINGS_H

