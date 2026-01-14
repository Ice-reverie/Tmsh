#include "GUITetGenSettings.h"
#include "ui_GUITetGenSettings.h"

#include "FITK_Interface/FITKInterfaceMeshGen/FITKMeshGenInterface.h"
#include "FITK_Interface/FITKInterfaceMeshGen/FITKGlobalMeshSizeInfo.h"
#include "FITK_Interface/FITKInterfaceMeshGen/FITKGlobalMeshGenerateAlgorithmInfo.h"
#include "FITK_Interface/FITKInterfaceMeshGen/FITKAbstractMesherDriver.h"
#include "FITKTmshGlobalMeshGenerateAlgorithmInfo.h"

#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKSignalTransfer.h"
#include "FITK_Kernel/FITKCore/FITKDataRepo.h"

#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoCommand.h"

#include "GUITetGenHelp.h"

#include <QMessageBox>
#include <QFileDialog>
#include <QApplication>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>
#include <QDebug>
#include <QDir>

namespace GUI
{
    namespace
    {
        constexpr const char* kMesherKey = "TmshExec";
    }

    GUITetGenSettings::GUITetGenSettings(QWidget* parent/* = nullptr*/) : Core::FITKDialog(parent)
    {
        _ui = new Ui::GUITetGenSettings();
        _ui->setupUi(this);
        setAttribute(Qt::WA_DeleteOnClose);
        Qt::WindowFlags flags = windowFlags();
        flags &= ~Qt::WindowContextHelpButtonHint;
        setWindowFlags(flags);

        QString currentPath = QDir::currentPath();
        _ui->lineEdit_FilePath->setText(currentPath);

        this->init();
    }

    GUITetGenSettings::GUITetGenSettings(Interface::FITKAbstractMesherDriver* driver, QWidget* parent/* = nullptr*/)
        : Core::FITKDialog(parent), _driver(driver)
    {
        _ui = new Ui::GUITetGenSettings();
        _ui->setupUi(this);
        setAttribute(Qt::WA_DeleteOnClose);
        Qt::WindowFlags flags = windowFlags();
        flags &= ~Qt::WindowContextHelpButtonHint;
        setWindowFlags(flags);

        QString currentPath = QDir::currentPath();
        _ui->lineEdit_FilePath->setText(currentPath);

        this->init();
    }

    GUITetGenSettings::~GUITetGenSettings()
    {
        if (_ui)
        {
            delete _ui;
            _ui = nullptr;
        }
        if (_detailedDlg)
        {
            delete _detailedDlg;
            _detailedDlg = nullptr;
        }
        AppFrame::FITKSignalTransfer* signalTransfer = FITKAPP->getSignalTransfer();
        if (!signalTransfer) return;
        emit signalTransfer->setPickableObjTypeSig(-1);
    }


    void GUITetGenSettings::init()
    {

        setWindowTitle(tr("TetGen Settings"));

        Interface::FITKGlobalMeshSizeInfo* meshSizeInfo = Interface::FITKMeshGenInterface::getInstance()->getGlobalMeshSizeInfo(kMesherKey);
        Interface::FITKGlobalMeshGenerateAlgorithmInfo* meshGenerateAlgorithmInfo = Interface::FITKMeshGenInterface::getInstance()->getGlobalMeshGenerateAlgorithmInfo(kMesherKey);
        if (!meshSizeInfo || !meshGenerateAlgorithmInfo) return;

        /*
        // Tmsh 专用算法信息（由 FITKMeshAlgorithmGeneratorTmshExec 创建）
        Tmsh::FITKTmshGlobalMeshGenerateAlgorithmInfo* tmshAlg =
            dynamic_cast<Tmsh::FITKTmshGlobalMeshGenerateAlgorithmInfo*>(meshGenerateAlgorithmInfo);
        if (tmshAlg)
        {
            //_ui->lineEdit_TangencyTolerance->setText(QString::number(tmshAlg->getTangencyTolerance(), 'g', 16));
        }

        */

    }

    void GUITetGenSettings::on_pushButton_OK_clicked()
    {
        Interface::FITKGlobalMeshSizeInfo* meshSizeInfo = Interface::FITKMeshGenInterface::getInstance()->getGlobalMeshSizeInfo(kMesherKey);
        if (meshSizeInfo == nullptr) return;

        Interface::FITKGlobalMeshGenerateAlgorithmInfo* meshGenerateAlgorithmInfo = Interface::FITKMeshGenInterface::getInstance()->getGlobalMeshGenerateAlgorithmInfo(kMesherKey);
        if (meshGenerateAlgorithmInfo == nullptr) return;


        /*
        // 将界面参数写回 Tmsh 算法信息
        Tmsh::FITKTmshGlobalMeshGenerateAlgorithmInfo* tmshAlg =
            dynamic_cast<Tmsh::FITKTmshGlobalMeshGenerateAlgorithmInfo*>(meshGenerateAlgorithmInfo);
        if (tmshAlg)
        {
            QString TetgenSwitchesText = _ui->lineEdit_TetgenSwitches->text().trimmed();
            const char* TetgenSwitchesCStr = TetgenSwitchesText.toUtf8().constData();
            tmshAlg->setTetgenSwitches(TetgenSwitchesCStr);
        }

        */

        QVariant v = QVariant::fromValue(_virtualTopos);
        if (_driver)
        {
            _driver->setValue("virtualTopos", v);
        }

        this->accept();
    }

    void GUITetGenSettings::on_pushButton_Cancel_clicked()
    {
        QList<Interface::VirtualShape> virtualTopos{};
        QVariant v = QVariant::fromValue(virtualTopos);
        if (_driver)
        {
            _driver->setValue("virtualTopos", v);
        }
        this->reject();
    }


    // 实现详细参数按钮点击槽函数
    void GUITetGenSettings::on_pushButton_Help_clicked()
    {
        GUITetGenHelp *_detailedDlg = new GUITetGenHelp;
        _detailedDlg->show();
    }

    void GUITetGenSettings::on_pushButton_Browse_clicked()
    {
        QString filePath = QFileDialog::getOpenFileName(nullptr, "选择文件", _ui->lineEdit_FilePath->text(), "mesh文件 (*.surf.mesh)");
        if(!filePath.isEmpty())
        {
            _ui->lineEdit_FilePath->setText(filePath);
        }
    }
}

