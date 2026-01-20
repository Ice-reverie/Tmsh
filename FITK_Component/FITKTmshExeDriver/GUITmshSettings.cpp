#include "GUITmshSettings.h"
#include "ui_GUITmshSettings.h"

#include "FITK_Interface/FITKInterfaceMeshGen/FITKMeshGenInterface.h"
#include "FITK_Interface/FITKInterfaceMeshGen/FITKGlobalMeshSizeInfo.h"
#include "FITK_Interface/FITKInterfaceMeshGen/FITKGlobalMeshGenerateAlgorithmInfo.h"
#include "FITK_Interface/FITKInterfaceMeshGen/FITKAbstractMesherDriver.h"
#include "FITKTmshGlobalMeshGenerateAlgorithmInfo.h"

#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKSignalTransfer.h"
#include "FITK_Kernel/FITKCore/FITKDataRepo.h"

#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoCommand.h"

#include "GUIDetailedParameters.h"

#include <QMessageBox>
#include <QDebug>

#define LOW_ORDER 1
#define HIGH_ORDER 2
#define Surface 2
#define Solid 3

namespace GUI
{
    namespace
    {
        constexpr const char* kMesherKey = "TmshExec";
    }

    GUITmshSettings::GUITmshSettings(QWidget* parent/* = nullptr*/) : Core::FITKDialog(parent)
    {
        _ui = new Ui::GUITmshSettings();
        _ui->setupUi(this);
        setAttribute(Qt::WA_DeleteOnClose);
        Qt::WindowFlags flags = windowFlags();
        flags &= ~Qt::WindowContextHelpButtonHint;
        setWindowFlags(flags);
        connect(FITKAPP->getSignalTransfer(), &AppFrame::FITKSignalTransfer::graphObjectPickedSig, this, &GUITmshSettings::pickFinishedOper);
        this->init();
    }

    GUITmshSettings::GUITmshSettings(Interface::FITKAbstractMesherDriver* driver, QWidget* parent/* = nullptr*/)
        : Core::FITKDialog(parent), _driver(driver)
    {
        _ui = new Ui::GUITmshSettings();
        _ui->setupUi(this);
        setAttribute(Qt::WA_DeleteOnClose);
        Qt::WindowFlags flags = windowFlags();
        flags &= ~Qt::WindowContextHelpButtonHint;
        setWindowFlags(flags);
        connect(FITKAPP->getSignalTransfer(), &AppFrame::FITKSignalTransfer::graphObjectPickedSig, this, &GUITmshSettings::pickFinishedOper);
        this->init();
    }

    GUITmshSettings::~GUITmshSettings()
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

    void GUITmshSettings::isEnablePickModel(bool isEnable)
    {
        _isEnablePickModel = isEnable;
        _ui->groupBox_PickModel->setVisible(isEnable);
        this->adjustSize();
    }

    QList<Interface::VirtualShape> GUITmshSettings::getPickModelVirtualShape()
    {
        return _virtualTopos;
    }

    void GUITmshSettings::init()
    {
        _ui->comboBox_Type->addItem(tr("Surface"), Surface);
        _ui->comboBox_Type->addItem(tr("Solid"), Solid);

        _ui->groupBox_PickModel->setVisible(_isEnablePickModel);
        setWindowTitle(tr("Tmsh Settings"));

        Interface::FITKGlobalMeshSizeInfo* meshSizeInfo = Interface::FITKMeshGenInterface::getInstance()->getGlobalMeshSizeInfo(kMesherKey);
        Interface::FITKGlobalMeshGenerateAlgorithmInfo* meshGenerateAlgorithmInfo = Interface::FITKMeshGenInterface::getInstance()->getGlobalMeshGenerateAlgorithmInfo(kMesherKey);
        if (!meshSizeInfo || !meshGenerateAlgorithmInfo) return;

        // Tmsh 专用算法信息（由 FITKMeshAlgorithmGeneratorTmshExec 创建）
        Tmsh::FITKTmshGlobalMeshGenerateAlgorithmInfo* tmshAlg =
            dynamic_cast<Tmsh::FITKTmshGlobalMeshGenerateAlgorithmInfo*>(meshGenerateAlgorithmInfo);
        if (tmshAlg)
        {
            _ui->lineEdit_TangencyTolerance->setText(QString::number(tmshAlg->getTangencyTolerance(), 'g', 16));

            _ui->lineEdit_MinLength->setText(QString::number(tmshAlg->getMinLength(), 'g', 16));
            _ui->lineEdit_MaxLength->setText(QString::number(tmshAlg->getMaxLength(), 'g', 16));
            _ui->lineEdit_TetgenSwitches->setText(QString::number(tmshAlg->getTetgenSwitches(), 'g', 16));
            _ui->lineEdit_NormalTol->setText(QString::number(tmshAlg->getNormalTol(), 'g', 16));
            _ui->lineEdit_MinAngle->setText(QString::number(tmshAlg->getMinAngle(), 'g', 16));
        }
    }

    void GUITmshSettings::pickFinishedOper(int pickType, int objectId, QHash<QString, void*> objInfo)
    {
        Q_UNUSED(pickType);
        if (objInfo.contains("PickError"))
        {
            _virtualTopos.clear();
            _ui->label_10->setText("Picked (0)");
            return;
        }

        bool pickStart = *static_cast<bool*>(objInfo["PickStart"]);
        if (pickStart)
        {
            _virtualTopos.clear();
            _ui->label_10->setText("Picked (0)");
        }

        int cmdID = *static_cast<int*>(objInfo["CmdId"]);
        Interface::FITKAbsGeoCommand* cmd = FITKDATAREPO->getTDataByID<Interface::FITKAbsGeoCommand>(cmdID);
        if (!cmd) return;

        Interface::FITKShapeVirtualTopoManager* virtualTopoManager = nullptr;
        if (_ui->comboBox_Type->currentData().toInt() == Surface)
        {
            virtualTopoManager = cmd->getShapeVirtualTopoManager(Interface::FITKGeoEnum::VTopoShapeType::VSFace);
        }
        else
        {
            virtualTopoManager = cmd->getShapeVirtualTopoManager(Interface::FITKGeoEnum::VTopoShapeType::VSSolid);
        }
        if (!virtualTopoManager) return;

        int index = virtualTopoManager->getIndexByID(objectId);
        Interface::VirtualShape temp(cmdID, objectId, index);
        if (_ui->comboBox_Type->currentData().toInt() == Surface)
        {
            temp.Type = Interface::FITKGeoEnum::VTopoShapeType::VSFace;
        }
        else
        {
            temp.Type = Interface::FITKGeoEnum::VTopoShapeType::VSSolid;
        }

        _virtualTopos.push_back(temp);
        _ui->label_10->setText(QString("Picked (%1)").arg(QString::number(_virtualTopos.size())));
    }

    void GUITmshSettings::on_pushButton_OK_clicked()
    {
        if (_isEnablePickModel && _virtualTopos.size() == 0)
        {
            QMessageBox::warning(this, "warning", tr("Pick up the geometric model."), QMessageBox::Ok);
            return;
        }

        Interface::FITKGlobalMeshSizeInfo* meshSizeInfo = Interface::FITKMeshGenInterface::getInstance()->getGlobalMeshSizeInfo(kMesherKey);
        if (meshSizeInfo == nullptr) return;

        Interface::FITKGlobalMeshGenerateAlgorithmInfo* meshGenerateAlgorithmInfo = Interface::FITKMeshGenInterface::getInstance()->getGlobalMeshGenerateAlgorithmInfo(kMesherKey);
        if (meshGenerateAlgorithmInfo == nullptr) return;

        // 将界面参数写回 Tmsh 算法信息
        Tmsh::FITKTmshGlobalMeshGenerateAlgorithmInfo* tmshAlg =
            dynamic_cast<Tmsh::FITKTmshGlobalMeshGenerateAlgorithmInfo*>(meshGenerateAlgorithmInfo);
        if (tmshAlg)
        {
            tmshAlg->setTangencyTolerance(_ui->lineEdit_TangencyTolerance->text().toDouble());

            tmshAlg->setMaxLength(_ui->lineEdit_MaxLength->text().toDouble());
            tmshAlg->setMinLength(_ui->lineEdit_MinLength->text().toDouble());
            tmshAlg->setMinAngle(_ui->lineEdit_MinAngle->text().toDouble());
            tmshAlg->setNormalTol(_ui->lineEdit_NormalTol ->text().toDouble());

            QString TetgenSwitchesText = _ui->lineEdit_TetgenSwitches->text().trimmed();
            const char* TetgenSwitchesCStr = TetgenSwitchesText.toUtf8().constData();
            tmshAlg->setTetgenSwitches(TetgenSwitchesCStr);
        }

        QVariant v = QVariant::fromValue(_virtualTopos);
        if (_driver)
        {
            _driver->setValue("virtualTopos", v);
        }

        this->accept();
    }

    void GUITmshSettings::on_pushButton_Cancel_clicked()
    {
        QList<Interface::VirtualShape> virtualTopos{};
        QVariant v = QVariant::fromValue(virtualTopos);
        if (_driver)
        {
            _driver->setValue("virtualTopos", v);
        }
        this->reject();
    }

    void GUITmshSettings::on_pushButton_Pick_clicked()
    {
        AppFrame::FITKSignalTransfer* signalTransfer = FITKAPP->getSignalTransfer();
        if (!signalTransfer) return;
        if (_ui->comboBox_Type->currentData().toInt() == Surface)
        {
            emit signalTransfer->setPickableObjTypeSig(3005);
        }
        else
        {
            emit signalTransfer->setPickableObjTypeSig(3006);
        }
    }

    void GUITmshSettings::on_comboBox_Type_currentIndexChanged(int index)
    {
        //_ui->tabWidget->setCurrentIndex(index);
        _virtualTopos.clear();
        _ui->label_10->setText("Picked (0)");
        AppFrame::FITKSignalTransfer* signalTransfer = FITKAPP->getSignalTransfer();
        if (!signalTransfer) return;
        emit signalTransfer->setPickableObjTypeSig(-1);
    }

    // 实现详细参数按钮点击槽函数
    void GUITmshSettings::on_pushButton_Detailed_clicked()
    {
        GUIDetailedParameters *_detailedDlg = new GUIDetailedParameters;
        _detailedDlg->show();
    }
}

