#include "OperTmshGenerator.h"

#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKMessage.h"

#include "OperatorsInterface/GraphEventOperator.h"
#include "GUIFrame/MainWindow.h"
#include "ModelData/MeshKernel.h"
#include "ModelData/MeshData.h"
#include "ModelData/MeshManager.h"

#include "FITK_Interface/FITKInterfaceMeshGen/FITKMeshGenInterface.h"
#include "FITK_Interface/FITKInterfaceMeshGen/FITKAbstractMesherDriver.h"
#include "FITK_Interface/FITKInterfaceMeshGen/FITKAbstractMeshProcessor.h"

#include "FITK_Interface/FITKInterfaceMesh/FITKUnstructuredMeshVTK.h"
#include "FITK_Interface/FITKInterfaceModel/FITKComponentManager.h"
#include "GUIWidget/GUIEnumType.h"
#include "GUIFrame/ControlPanel.h"

#include <QDir>
#include <QDateTime>
#include <QFileInfo>
#include <QMessageBox>
#include <QThread>

namespace ModelOper
{
    bool OperTmshGenerator::_isMeshFinishedExecuting = false;

    OperTmshGenerator::~OperTmshGenerator()
    {
        if (_mesher)
        {
            disconnect(_mesher, &Interface::FITKAbstractMesherDriver::mesherFinished, this, &OperTmshGenerator::meshGenFinished);
        }
    }

    namespace
    {
        void refreshMeshTree()
        {
            GUI::MainWindow* mw = GUI::MainWindow::GetMainWindowFromFramework();
            if (!mw) return;
            GUI::ControlPanel* cp = mw->getControlPanel();
            if (!cp) return;
            GUI::TreeWidgetPage* tw = cp->getTreeWidget(GUI::TreeWidgetType::PageMesh);
            if (!tw) return;
            tw->updatePage();
        }
    }

    bool OperTmshGenerator::execGUI()
    {
        QWidget* mw = FITKAPP->getGlobalData()->getMainWindow();
        if (!mw) return false;

        if (_emitter->objectName() == "actionTmshClean")
        {
            this->meshClean();
        }
        if (_emitter->objectName() == "actionTmshCreateGlobalMesh")
        {
            this->meshGenOper();
        }

        if (_emitter->objectName() == "actionCreateTetGen")
        {
            this->tetGenOper();
        }

        return false;
    }

    bool OperTmshGenerator::execProfession()
    {
        return true;
    }

    void OperTmshGenerator::meshGenOper()
    {
        QString meshPath = FITKAPP->getTempDir(false, "");
        QString meshFile = QString("%1/%2").arg(meshPath).arg("geometryFile");
        QString meshName = Interface::FITKMeshGenInterface::getMeshFileName();
        if (meshName == "")
        {
            meshName = "geometryFile.surf.mesh";
        }
        QString meshFileProcessor = QString("%1/%2").arg(meshPath).arg(meshName);

        // 清理旧的网格文件，避免读取历史文件
        QDir meshDir(meshPath);
        QStringList oldMeshFiles = meshDir.entryList(QStringList() << "geometryFile.surf.*.mesh", QDir::Files);
        for (const QString& oldFile : oldMeshFiles)
        {
            QString fullOldPath = meshDir.filePath(oldFile);
            if (QFile::remove(fullOldPath))
            {
                AppFrame::FITKMessageNormal(QString("Removed old mesh file: %1").arg(oldFile));
            }
        }
        // 也删除当前的surf.mesh文件
        QString currentSurfMesh = meshDir.filePath("geometryFile.surf.mesh");
        if (QFile::exists(currentSurfMesh))
        {
            QFile::remove(currentSurfMesh);
            AppFrame::FITKMessageNormal("Removed old geometryFile.surf.mesh");
        }

        Interface::FITKMeshGenInterface* mf = Interface::FITKMeshGenInterface::getInstance();
        if (!mf) return;
        Interface::FITKAbstractMesherDriver* mesher = mf->getMesherDriver("TmshExec");
        if (!mesher) return;
        _mesher = mesher;
        disconnect(mesher, &Interface::FITKAbstractMesherDriver::mesherFinished, this, &OperTmshGenerator::meshGenFinished);
        connect(mesher, &Interface::FITKAbstractMesherDriver::mesherFinished, this, &OperTmshGenerator::meshGenFinished, Qt::UniqueConnection);
        mesher->setValue("MeshFile", meshFile);
        mesher->setValue("MeshFileProcessor", meshFileProcessor);
        mesher->setValue("Method", 1);
        mesher->startMesher();
        this->setArgs("MeshFile", meshFile);
    }

    void OperTmshGenerator::tetGenOper()
    {
        QString meshPath = FITKAPP->getTempDir(false, "");
        QString meshFile = QString("%1/%2").arg(meshPath).arg("geometryFile");
        QString meshName = Interface::FITKMeshGenInterface::getMeshFileName();
        if (meshName == "")
        {
            meshName = "geometryFile.surf.1.mesh";
        }
        QString meshFileProcessor = QString("%1/%2").arg(meshPath).arg(meshName);

        // 清理旧的网格文件，避免读取历史文件
        QDir meshDir(meshPath);
        QStringList oldMeshFiles = meshDir.entryList(QStringList() << "geometryFile.surf.*.mesh", QDir::Files);
        for (const QString& oldFile : oldMeshFiles)
        {
            QString fullOldPath = meshDir.filePath(oldFile);
            if (QFile::remove(fullOldPath))
            {
                AppFrame::FITKMessageNormal(QString("Removed old mesh file: %1").arg(oldFile));
            }
        }
        // 也删除当前的surf.mesh文件
        QString currentSurfMesh = meshDir.filePath("geometryFile.surf.mesh");
        if (QFile::exists(currentSurfMesh))
        {
            QFile::remove(currentSurfMesh);
            AppFrame::FITKMessageNormal("Removed old geometryFile.surf.mesh");
        }

        Interface::FITKMeshGenInterface* mf = Interface::FITKMeshGenInterface::getInstance();
        if (!mf) return;
        Interface::FITKAbstractMesherDriver* mesher = mf->getMesherDriver("TmshExec");
        if (!mesher) return;
        _mesher = mesher;
        disconnect(mesher, &Interface::FITKAbstractMesherDriver::mesherFinished, this, &OperTmshGenerator::meshGenFinished);
        connect(mesher, &Interface::FITKAbstractMesherDriver::mesherFinished, this, &OperTmshGenerator::meshGenFinished, Qt::UniqueConnection);
        mesher->setValue("MeshFile", meshFile);
        mesher->setValue("MeshFileProcessor", meshFileProcessor);
        mesher->setValue("Method", 2);
        mesher->startMesher();
        this->setArgs("MeshFile", meshFile);
    }


    void OperTmshGenerator::meshGenFinished()
    {
        AppFrame::FITKMessageNormal(QString("meshGenFinished started: %1").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss:zzz")));

        if (_isMeshFinishedExecuting || !_mesher)
        {
            AppFrame::FITKMessageNormal(QString("meshGenFinished skipped: executing=%1, mesher=%2")
                .arg(_isMeshFinishedExecuting).arg(_mesher ? "valid" : "null"));
            return;
        }
        _isMeshFinishedExecuting = true;

        const QString meshFile = _mesher->getValueT<QString>("MeshFileProcessor");

        AppFrame::FITKMessageNormal(QString("MeshFileProcessor: %1").arg(meshFile));

        if (meshFile.isEmpty())
        {
            AppFrame::FITKMessageError("MeshFileProcessor is empty!");
            QMessageBox::warning(FITKAPP->getGlobalData()->getMainWindow(), tr("Warning"),
                tr("Mesh file path is empty!"), QMessageBox::StandardButton::Ok);
            _isMeshFinishedExecuting = false;
            return;
        }

        QFileInfo fileInfo(meshFile);
        int waitCount = 0;
        while (!fileInfo.exists() && waitCount < 50)
        {
            QThread::msleep(100);
            fileInfo.refresh();
            waitCount++;
            AppFrame::FITKMessageNormal(QString("Waiting for mesh file... (%1/50)").arg(waitCount));
        }

        if (!fileInfo.exists())
        {
            AppFrame::FITKMessageError(QString("Mesh file does not exist after waiting: %1").arg(meshFile));
            QMessageBox::warning(FITKAPP->getGlobalData()->getMainWindow(), tr("Warning"),
                tr("Mesh file not found:\n%1").arg(meshFile), QMessageBox::StandardButton::Ok);
            _isMeshFinishedExecuting = false;
            return;
        }

        if (!fileInfo.isFile())
        {
            AppFrame::FITKMessageError(QString("Path is not a file: %1").arg(meshFile));
            QMessageBox::warning(FITKAPP->getGlobalData()->getMainWindow(), tr("Warning"),
                tr("Path is not a file:\n%1").arg(meshFile), QMessageBox::StandardButton::Ok);
            _isMeshFinishedExecuting = false;
            return;
        }

        if (fileInfo.size() == 0)
        {
            AppFrame::FITKMessageError(QString("Mesh file is empty: %1").arg(meshFile));
            QMessageBox::warning(FITKAPP->getGlobalData()->getMainWindow(), tr("Warning"),
                tr("Mesh file is empty:\n%1").arg(meshFile), QMessageBox::StandardButton::Ok);
            _isMeshFinishedExecuting = false;
            return;
        }

        AppFrame::FITKMessageNormal(QString("Mesh file verified: %1, size: %2 bytes").arg(meshFile).arg(fileInfo.size()));

        ModelData::MeshManager* meshManager = FITKAPP->getGlobalData()->getMeshData<ModelData::MeshManager>();
        if (!meshManager)
        {
            AppFrame::FITKMessageError("Failed to get MeshManager");
            _isMeshFinishedExecuting = false;
            return;
        }

        ModelData::MeshData* meshData = meshManager->getMeshDataObjectByNameT<ModelData::MeshData>("Tmsh");
        if (!meshData)
        {
            AppFrame::FITKMessageError("Failed to get MeshData 'Tmsh'");
            _isMeshFinishedExecuting = false;
            return;
        }

        // 清空旧的网格数据，避免显示历史网格
        meshData->clear();
        AppFrame::FITKMessageNormal("Cleared previous mesh data");

        Interface::FITKMeshGenInterface* mf = Interface::FITKMeshGenInterface::getInstance();
        if (!mf)
        {
            AppFrame::FITKMessageError("Failed to get FITKMeshGenInterface");
            _isMeshFinishedExecuting = false;
            return;
        }

        Interface::FITKAbstractMeshProcessor* processorMesh = mf->getMeshProcessor("TmshExec");
        if (!processorMesh)
        {
            AppFrame::FITKMessageError("Failed to get MeshProcessor 'TmshExec'");
            _isMeshFinishedExecuting = false;
            return;
        }

        processorMesh->setValue("File", meshFile);
        processorMesh->setValue("FilterDim", QList<QVariant>() << 0 << 1);

        Interface::FITKUnstructuredMesh* mesh = new Interface::FITKUnstructuredMesh;
        ModelData::MeshKernel* meshKernel = new ModelData::MeshKernel;
        meshData->appendDataObj(meshKernel);
        meshKernel->setMesh(mesh);

        processorMesh->insertDataObject("Mesh", mesh);
        processorMesh->insertDataObject("ComponentManager", meshKernel->getComponentManager());
        processorMesh->start(QStringList() << "MSH");
        int objID = meshKernel->getDataObjectID();

        AppFrame::FITKMessageNormal(QString("Read file succeed: %1 %2").arg(meshFile).arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss:zzz")));

        EventOper::GraphEventOperator* operGraph = FITKOPERREPO->getOperatorT<EventOper::GraphEventOperator>("GraphPreprocess");
        if (operGraph)
        {
            operGraph->updateGraph(objID, true);
        }

        refreshMeshTree();

        _isMeshFinishedExecuting = false;
        AppFrame::FITKMessageNormal(QString("meshGenFinished completed: %1").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss:zzz")));
    }

    void OperTmshGenerator::meshClean()
    {
        ModelData::MeshManager* meshManager = FITKAPP->getGlobalData()->getMeshData<ModelData::MeshManager>();
        if (!meshManager) return;
        ModelData::MeshData* meshData = meshManager->getMeshDataObjectByNameT<ModelData::MeshData>("Tmsh");
        if (!meshData) return;
        meshData->clear();

        EventOper::GraphEventOperator* operGraph = FITKOPERREPO->getOperatorT<EventOper::GraphEventOperator>("GraphPreprocess");
        if (operGraph)
        {
            operGraph->updateAllGraph();
        }

        refreshMeshTree();
    }
}
