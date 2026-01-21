#include "OperatorImportMesh.h"

#include "OperatorsInterface/GraphEventOperator.h"
#include "GraphData/GraphDataPick/GraphPickProvider.h"
#include "GraphData/GraphDataPick/GraphPickData.h"

#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppSettings.h"
#include "FITK_Kernel/FITKCore/FITKThreadPool.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKGeoCommandList.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKGeoInterfaceFactory.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoModelImport.h"

#include "GUIFrame/MainWindow.h"
#include "GUIFrame/ControlPanel.h"
#include "GUIWidget/GUIEnumType.h"
#include "ModelData/GeometryManager.h"
#include <QFileDialog>
#include <QMessageBox>

#include "ModelData/MeshKernel.h"
#include "ModelData/MeshData.h"
#include "ModelData/MeshManager.h"

#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKMessage.h"
#include "FITK_Interface/FITKInterfaceMeshGen/FITKMeshGenInterface.h"
#include "FITK_Interface/FITKInterfaceMeshGen/FITKAbstractMesherDriver.h"
#include "FITK_Interface/FITKInterfaceMeshGen/FITKAbstractMeshProcessor.h"

#include "FITK_Interface/FITKInterfaceMesh/FITKUnstructuredMeshVTK.h"
#include "FITK_Interface/FITKInterfaceModel/FITKComponentManager.h"
#include <QDateTime>

namespace ModelOper
{
    OperatorsImportMesh::OperatorsImportMesh()
    {

    }

    OperatorsImportMesh::~OperatorsImportMesh()
    {

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

    bool OperatorsImportMesh::execGUI()
    {
        //if (_emitter->objectName() == "actionWorkingDir") {
        //    Graph::GraphPickProvider* provider = Graph::GraphPickProvider::getInstance();
        //    if (provider == nullptr)return false;

        //    Graph::GraphDataPickInfoStru pickStr;
        //    pickStr._pickObjType = Graph::PickObjType::PickGeoFace;
        //    pickStr._pickMethod = Graph::PickMethod::PMIndividually;
        //    Graph::GraphDataPickInfo::ClearPickInfo();
        //    Graph::GraphDataPickInfo::SetPickInfo(pickStr);
        //    return false;
        //}

        //主界面
        QWidget* mw = FITKAPP->getGlobalData()->getMainWindow();
        //获取工作目录
        QString wk;
        auto settings = FITKAPP->getAppSettings();
        if (settings)
            wk = settings->getWorkingDir();
        QFileDialog dlg(mw, QObject::tr("Import Model File"), wk, "Mesh File(*.mesh)");
        dlg.setAcceptMode(QFileDialog::AcceptOpen);  //打开模式
        dlg.show();
        //阻塞，否则键盘事件处理会出问题
        bool accept = false;
        QEventLoop loop;
        connect(&dlg, &QFileDialog::accepted, [&] {loop.quit(); accept = true;  });
        connect(&dlg, &QFileDialog::rejected, [&] {loop.quit(); accept = false;  });
        loop.exec();
        if (!accept) return false;
        QStringList files = dlg.selectedFiles(); //选择的文件名称
        if (files.isEmpty()) return false;
        QString fileName = files.at(0);

        if (fileName.isEmpty())
            return false;
        //存储信息
        //if (_emitter == nullptr) return false;
        //存储
        this->setArgs("FileName", fileName);
        return true;
    }

    bool OperatorsImportMesh::execProfession()
    {
        //获取线程池
        Core::FITKThreadPool* pool = Core::FITKThreadPool::getInstance();
        if (pool == nullptr)return false;
        QString fileName;
        this->argValue<QString>("FileName", fileName);
        if (fileName.isEmpty()) return false;

		////////////////////////////////////////////

		/*const QString meshFile = this->getValueT<QString>("MeshFileProcessor");*/


		if (fileName.isEmpty() || !QFileInfo(fileName).isFile())
		{
			QMessageBox::warning(FITKAPP->getGlobalData()->getMainWindow(), tr("warning"), tr("Err! Mesher ProgramExec generate meshes failed."), QMessageBox::StandardButton::Ok);
			return false;
		}

		ModelData::MeshManager* meshManager = FITKAPP->getGlobalData()->getMeshData<ModelData::MeshManager>();
		if (!meshManager) return false;
		ModelData::MeshData* meshData = meshManager->getMeshDataObjectByNameT<ModelData::MeshData>("Tmsh");
		if (!meshData) return false;

		Interface::FITKMeshGenInterface* mf = Interface::FITKMeshGenInterface::getInstance();
		if (!mf) return false;
		Interface::FITKAbstractMeshProcessor* processorMesh = mf->getMeshProcessor("TmshExec");
		if (!processorMesh) return false;
		processorMesh->setValue("File", fileName);
		processorMesh->setValue("FilterDim", QList<QVariant>() << 0 << 1);

		Interface::FITKUnstructuredMesh* mesh = new Interface::FITKUnstructuredMesh;
		ModelData::MeshKernel* meshKernel = new ModelData::MeshKernel;
		meshData->appendDataObj(meshKernel);
		meshKernel->setMesh(mesh);

		processorMesh->insertDataObject("Mesh", mesh);
		processorMesh->insertDataObject("ComponentManager", meshKernel->getComponentManager());
		processorMesh->start(QStringList() << "MSH");
		int objID = meshKernel->getDataObjectID();

		AppFrame::FITKMessageNormal(QString("Read file succeed:%1 %2").arg(fileName).arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss:zzz")));

		EventOper::GraphEventOperator* operGraph = FITKOPERREPO->getOperatorT<EventOper::GraphEventOperator>("GraphPreprocess");
		if (operGraph)
		{
			operGraph->updateGraph(objID, true);
		}

		refreshMeshTree();
		///////////////////////////////////////////////////
        /*ImportReadThread* importThread = new ImportReadThread();
        importThread->_fileName = fileName;
        connect(importThread, SIGNAL(sigImportFinish(bool, int)), this, SLOT(slotImportFinish(bool, int)));
        pool->execTask(importThread);*/
        //       this->clearArgs();
        return true;
    }

    void OperatorsImportMesh::slotImportFinish(bool result, int objID)
    {
        if (result == false)return;
        if (objID < 0)return;


        EventOper::GraphEventOperator* graphOper = FITKOPERREPO->getOperatorT<EventOper::GraphEventOperator>("GraphPreprocess");
        if (graphOper) {
            graphOper->updateGraph(objID);
        }

        //刷新树
        GUI::MainWindow* mw = GUI::MainWindow::GetMainWindowFromFramework();
        if (mw == nullptr) return;
        GUI::ControlPanel* cp = mw->getControlPanel();
        if (cp == nullptr) return;
        GUI::TreeWidgetPage* tw = cp->getTreeWidget(GUI::TreeWidgetType::PageModel);
        if (tw == nullptr) return;
        tw->updatePage();
    }

    //void ImportReadThread::run()
    //{
    //    Interface::FITKGeoCommandList*  geometryData = FITKAPP->getGlobalData()->
    //        getGeometryData<ModelData::GeometryManager>();
    //    if (geometryData == nullptr) return;

    //    Interface::FITKInterfaceGeometryFactory* geoFactory = Interface::FITKInterfaceGeometryFactory::getInstance();
    //    if (geoFactory == nullptr)return;
    //    auto geoObj = geoFactory->createCommandT<Interface::FITKAbsGeoModelImport>(Interface::FITKGeoEnum::FITKGeometryComType::FGTImport);
    //    if (geoObj == nullptr)return;
    //    geoObj->setFileName(_fileName);


    //    if (geoObj->getDataObjectName().isEmpty())
    //    {
    //        QFileInfo fileInfo(_fileName);
    //        // 获取文件名称（不包含路径与文件类型）
    //        QString name = fileInfo.baseName();
    //        geoObj->setDataObjectName(geometryData->checkName(name));
    //    }
    //    bool result = geoObj->update();
    //    int objID = geoObj->getDataObjectID();

    //    if (result)//读取成功
    //    {
    //        geometryData->appendDataObj(geoObj);
    //        emit sigImportFinish(result, objID);
    //    }
    //    else //读取失败
    //    {
    //        if (geoObj != nullptr) delete geoObj;
    //    }

    //}
}

