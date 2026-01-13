/**********************************************************************
 * @file   GUITmshSettings.h
 * @brief  Tmsh尺寸信息设置
 * @date   2025-12-22
 *********************************************************************/
#ifndef GUITMSHSETTINGS_H
#define GUITMSHSETTINGS_H

#include "FITK_Kernel/FITKCore/FITKAbstractGUIDialog.h"
#include "FITKTmshExeDriverAPI.h"

#include "GUIDetailedParameters.h"
#include "ui_GUIDetailedParameters.h"

#include <QWidget>

namespace Ui { class GUITmshSettings; }

namespace Interface
{
    class VirtualShape;
    class FITKAbstractMesherDriver;
}

namespace GUI
{
    class FITKTmshExeDriverAPI GUITmshSettings : public Core::FITKDialog
    {
        Q_OBJECT
    public:
        GUITmshSettings(QWidget* parent = nullptr);
        GUITmshSettings(Interface::FITKAbstractMesherDriver* driver, QWidget* parent = nullptr);
        ~GUITmshSettings();

        void isEnablePickModel(bool isEnable);
        QList<Interface::VirtualShape> getPickModelVirtualShape();

    private:
        void init();
        void pickFinishedOper(int pickType, int objectId, QHash<QString, void*> objInfo);

    private slots:
        void on_pushButton_OK_clicked();
        void on_pushButton_Cancel_clicked();
        void on_pushButton_Pick_clicked();
        void on_comboBox_Type_currentIndexChanged(int index);
        void on_pushButton_Detailed_clicked();

    private:
        Ui::GUITmshSettings* _ui{};
        bool _isEnablePickModel{ true };
        Interface::FITKAbstractMesherDriver* _driver{};
        QList<Interface::VirtualShape> _virtualTopos{};
        GUITmshSettings* _detailedDlg{};
    };
}

#endif

