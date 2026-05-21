#include "GUIDetailedParameters.h"
#include "ui_GUIDetailedParameters.h"

#include "FITK_Interface/FITKInterfaceMeshGen/FITKMeshGenInterface.h"
#include "FITK_Interface/FITKInterfaceMeshGen/FITKGlobalMeshSizeInfo.h"
#include "FITK_Interface/FITKInterfaceMeshGen/FITKGlobalMeshGenerateAlgorithmInfo.h"
#include "FITK_Interface/FITKInterfaceMeshGen/FITKAbstractMesherDriver.h"
#include "FITKTmshGlobalMeshGenerateAlgorithmInfo.h"

#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKSignalTransfer.h"
#include "FITK_Kernel/FITKCore/FITKDataRepo.h"

#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoCommand.h"

#include "GUITmshSettings.h"

#include <QMessageBox>

#define LOW_ORDER 1
#define HIGH_ORDER 2

namespace GUI
{
    namespace
    {
        constexpr const char* kMesherKey = "TmshExec";
    }

    GUIDetailedParameters::GUIDetailedParameters(QWidget* parent/* = nullptr*/) : Core::FITKDialog(parent)
    {
        _ui = new Ui::GUIDetailedParameters();
        _ui->setupUi(this);
        setAttribute(Qt::WA_DeleteOnClose);
        Qt::WindowFlags flags = windowFlags();
        flags &= ~Qt::WindowContextHelpButtonHint;
        setWindowFlags(flags);

        connect(_ui->pushButton_OK, &QPushButton::clicked, this, &GUIDetailedParameters::on_pushButton_OK_clicked);

        this->init();
    }

    GUIDetailedParameters::GUIDetailedParameters(Interface::FITKAbstractMesherDriver* driver, QWidget* parent/* = nullptr*/)
        : Core::FITKDialog(parent), _driver(driver)
    {
        _ui = new Ui::GUIDetailedParameters();
        _ui->setupUi(this);
        setAttribute(Qt::WA_DeleteOnClose);
        Qt::WindowFlags flags = windowFlags();
        flags &= ~Qt::WindowContextHelpButtonHint;
        setWindowFlags(flags);

        connect(_ui->pushButton_OK, &QPushButton::clicked, this, &GUIDetailedParameters::on_pushButton_OK_clicked);

        this->init();
    }

    GUIDetailedParameters::~GUIDetailedParameters()
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


    void GUIDetailedParameters::init()
    {
        setWindowTitle(tr("Tmsh Settings: Detailed Parameters"));

        Interface::FITKGlobalMeshSizeInfo* meshSizeInfo = Interface::FITKMeshGenInterface::getInstance()->getGlobalMeshSizeInfo(kMesherKey);
        Interface::FITKGlobalMeshGenerateAlgorithmInfo* meshGenerateAlgorithmInfo = Interface::FITKMeshGenInterface::getInstance()->getGlobalMeshGenerateAlgorithmInfo(kMesherKey);
        if (!meshSizeInfo || !meshGenerateAlgorithmInfo) return;

        // Tmsh 专用算法信息（由 FITKMeshAlgorithmGeneratorTmshExec 创建�?
        Tmsh::FITKTmshGlobalMeshGenerateAlgorithmInfo* tmshAlg =
            dynamic_cast<Tmsh::FITKTmshGlobalMeshGenerateAlgorithmInfo*>(meshGenerateAlgorithmInfo);
        if (tmshAlg)
        {
            _ui->lineEdit_UseMultiThreading->setText(QString::number(tmshAlg->getUseMultiThreading(), 'g', 16));
            _ui->lineEdit_RefineIter->setText(QString::number(tmshAlg->getRefineIter(), 'g', 16));
            _ui->lineEdit_RefineSmoothIter->setText(QString::number(tmshAlg->getRefineSmoothIter(), 'g', 16));
            _ui->lineEdit_AdaptIter->setText(QString::number(tmshAlg->getAdaptIter(), 'g', 16));
            _ui->lineEdit_AdaptSmoothIter->setText(QString::number(tmshAlg->getAdaptSmoothIter(), 'g', 16));
        }
    }


    void GUIDetailedParameters::on_pushButton_OK_clicked()
    {
        Interface::FITKGlobalMeshSizeInfo* meshSizeInfo = Interface::FITKMeshGenInterface::getInstance()->getGlobalMeshSizeInfo(kMesherKey);
        if (meshSizeInfo == nullptr) return;

        Interface::FITKGlobalMeshGenerateAlgorithmInfo* meshGenerateAlgorithmInfo = Interface::FITKMeshGenInterface::getInstance()->getGlobalMeshGenerateAlgorithmInfo(kMesherKey);
        if (meshGenerateAlgorithmInfo == nullptr) return;

        // 将界面参数写�? Tmsh 算法信息
        Tmsh::FITKTmshGlobalMeshGenerateAlgorithmInfo* tmshAlg =
            dynamic_cast<Tmsh::FITKTmshGlobalMeshGenerateAlgorithmInfo*>(meshGenerateAlgorithmInfo);
        if (tmshAlg)
        {
            QString UseMultiThreadingText = _ui->lineEdit_UseMultiThreading->text().trimmed().toLower();
            bool UseMultiThreadingValue = (UseMultiThreadingText == "true" || UseMultiThreadingText == "1");
            tmshAlg->setUseMultiThreading(UseMultiThreadingValue);

            tmshAlg->setRefineIter(_ui->lineEdit_RefineIter->text().toInt());
            tmshAlg->setRefineSmoothIter(_ui->lineEdit_RefineSmoothIter->text().toInt());
            tmshAlg->setAdaptIter(_ui->lineEdit_AdaptIter->text().toInt());
            tmshAlg->setAdaptSmoothIter(_ui->lineEdit_AdaptSmoothIter->text().toInt());
        }

        QVariant v = QVariant::fromValue(_virtualTopos);
        if (_driver)
        {
            _driver->setValue("virtualTopos", v);
        }

        this->accept();
    }

}
