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

#include <QDateTime>
#include <QFileInfo>
#include <QMessageBox>

namespace ModelOper
{
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
		//获取工作目录
		QString meshPath = FITKAPP->getTempDir(false, "");
		QString meshFile = QString("%1/%2").arg(meshPath).arg("geometryFile");
        //QString meshName = "";
        QString meshName = Interface::FITKMeshGenInterface::getMeshFileName();
		if (meshName == "")
		{
            meshName = "geometryFile.surf.mesh";
		}
		QString meshFileProcessor = QString("%1/%2").arg(meshPath).arg(meshName);
		//获取网格划分接口
		Interface::FITKMeshGenInterface* mf = Interface::FITKMeshGenInterface::getInstance();
		if (!mf) return;
		Interface::FITKAbstractMesherDriver* mesher = mf->getMesherDriver("TmshExec");
		if (!mesher) return;
		_mesher = mesher;
		//关联信号
		disconnect(mesher, SIGNAL(mesherFinished()), nullptr, nullptr);
		connect(mesher, SIGNAL(mesherFinished()), this, SLOT(meshGenFinished()));
		//设置参数
		mesher->setValue("MeshFile", meshFile);
		mesher->setValue("MeshFileProcessor", meshFileProcessor);
		mesher->setValue("Method", 1);
		//开始划分网格
		mesher->startMesher();
		this->setArgs("MeshFile", meshFile);
	}

    void OperTmshGenerator::tetGenOper()
    {
		//获取工作目录
		QString meshPath = FITKAPP->getTempDir(false, "");
		QString meshFile = QString("%1/%2").arg(meshPath).arg("geometryFile");
		//QString meshName = "";
		QString meshName = Interface::FITKMeshGenInterface::getMeshFileName();
		if (meshName == "")
		{
			meshName = "geometryFile.surf.1.mesh";
		}
		QString meshFileProcessor = QString("%1/%2").arg(meshPath).arg(meshName);
        //获取网格划分接口
        Interface::FITKMeshGenInterface* mf = Interface::FITKMeshGenInterface::getInstance();
        if (!mf) return;
        Interface::FITKAbstractMesherDriver* mesher = mf->getMesherDriver("TmshExec");
        if (!mesher) return;
        _mesher = mesher;
        //关联信号
        disconnect(mesher, SIGNAL(mesherFinished()), nullptr, nullptr);
        connect(mesher, SIGNAL(mesherFinished()), this, SLOT(meshGenFinished()));
        //设置参数
        mesher->setValue("MeshFile", meshFile);
        mesher->setValue("MeshFileProcessor", meshFileProcessor);
        mesher->setValue("Method", 2);
        //开始划分网格
        mesher->startMesher();
        this->setArgs("MeshFile", meshFile);
    }


    void OperTmshGenerator::meshGenFinished()
    {
        if (!_mesher) return;

        const QString meshFile = _mesher->getValueT<QString>("MeshFileProcessor");


        if (meshFile.isEmpty() || !QFileInfo(meshFile).isFile())
        {
            QMessageBox::warning(FITKAPP->getGlobalData()->getMainWindow(), tr("warning"), tr("Err! Mesher ProgramExec generate meshes failed."), QMessageBox::StandardButton::Ok);
            return;
        }

        ModelData::MeshManager* meshManager = FITKAPP->getGlobalData()->getMeshData<ModelData::MeshManager>();
        if (!meshManager) return;
        ModelData::MeshData* meshData = meshManager->getMeshDataObjectByNameT<ModelData::MeshData>("Tmsh");
        if (!meshData) return;

        Interface::FITKMeshGenInterface* mf = Interface::FITKMeshGenInterface::getInstance();
        if (!mf) return;
        Interface::FITKAbstractMeshProcessor* processorMesh = mf->getMeshProcessor("TmshExec");
        if (!processorMesh) return;
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

        AppFrame::FITKMessageNormal(QString("Read file succeed:%1 %2").arg(meshFile).arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss:zzz")));

        EventOper::GraphEventOperator* operGraph = FITKOPERREPO->getOperatorT<EventOper::GraphEventOperator>("GraphPreprocess");
        if (operGraph)
        {
            operGraph->updateGraph(objID, true);
        }

        refreshMeshTree();
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
