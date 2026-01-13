#include "GUITetGenHelp.h"
#include "ui_GUITetGenHelp.h"

#include "FITK_Interface/FITKInterfaceMeshGen/FITKMeshGenInterface.h"
#include "FITK_Interface/FITKInterfaceMeshGen/FITKGlobalMeshSizeInfo.h"
#include "FITK_Interface/FITKInterfaceMeshGen/FITKGlobalMeshGenerateAlgorithmInfo.h"
#include "FITK_Interface/FITKInterfaceMeshGen/FITKAbstractMesherDriver.h"
#include "FITKTmshGlobalMeshGenerateAlgorithmInfo.h"

#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKSignalTransfer.h"
#include "FITK_Kernel/FITKCore/FITKDataRepo.h"

#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoCommand.h"

#include "GUITetGenSettings.h"

#include <QMessageBox>

namespace GUI
{
    namespace
    {
        constexpr const char* kMesherKey = "TmshExec";
    }

    GUITetGenHelp::GUITetGenHelp(QWidget* parent/* = nullptr*/) : Core::FITKDialog(parent)
    {
        _ui = new Ui::GUITetGenHelp();
        _ui->setupUi(this);
        setAttribute(Qt::WA_DeleteOnClose);
        Qt::WindowFlags flags = windowFlags();
        flags &= ~Qt::WindowContextHelpButtonHint;
        setWindowFlags(flags);

        connect(_ui->pushButton_Back, &QPushButton::clicked, this, &GUITetGenHelp::on_pushButton_Back_clicked);

        this->init();
    }

    GUITetGenHelp::GUITetGenHelp(Interface::FITKAbstractMesherDriver* driver, QWidget* parent/* = nullptr*/)
        : Core::FITKDialog(parent), _driver(driver)
    {
        _ui = new Ui::GUITetGenHelp();
        _ui->setupUi(this);
        setAttribute(Qt::WA_DeleteOnClose);
        Qt::WindowFlags flags = windowFlags();
        flags &= ~Qt::WindowContextHelpButtonHint;
        setWindowFlags(flags);

        connect(_ui->pushButton_Back, &QPushButton::clicked, this, &GUITetGenHelp::on_pushButton_Back_clicked);

        this->init();
    }

    GUITetGenHelp::~GUITetGenHelp()
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

    void GUITetGenHelp::init()
    {
        setWindowTitle(tr("TetGen Help"));

        Interface::FITKGlobalMeshSizeInfo* meshSizeInfo = Interface::FITKMeshGenInterface::getInstance()->getGlobalMeshSizeInfo(kMesherKey);
        Interface::FITKGlobalMeshGenerateAlgorithmInfo* meshGenerateAlgorithmInfo = Interface::FITKMeshGenInterface::getInstance()->getGlobalMeshGenerateAlgorithmInfo(kMesherKey);
        if (!meshSizeInfo || !meshGenerateAlgorithmInfo) return;
    }


    void GUITetGenHelp::on_pushButton_Back_clicked()
    {
        this->reject();
    }

}

