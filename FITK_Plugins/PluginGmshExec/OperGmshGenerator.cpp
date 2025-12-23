#include "OperGmshGenerator.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppSettings.h"
#include "FITK_Kernel/FITKAppFramework/FITKComponents.h"
#include "FITK_Kernel/FITKAppFramework/FITKMessage.h"
#include "OperatorsInterface/GraphEventOperator.h"
#include "GUIFrame/MainWindow.h"
//#include "GUIFrame/ControlPanel.h"
#include "ModelData/MeshKernel.h"
#include "ModelData/MeshData.h"
#include "ModelData/MeshManager.h"
#include "FITK_Kernel/FITKCore/FITKDirFileTools.h"

#include "FITK_Interface/FITKInterfaceGeometry/FITKGeoInterfaceFactory.h"
//#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoModelExport.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoModelExportTopos.h"
#include "FITK_Interface/FITKInterfaceMeshGen/FITKMeshGenInterface.h"
#include "FITK_Interface/FITKInterfaceMeshGen/FITKAbstractMesherDriver.h"
#include "FITK_Interface/FITKInterfaceMeshGen/FITKAbstractMeshProcessor.h"
#include "FITK_Interface/FITKInterfaceMeshGen/FITKGlobalMeshGenerateAlgorithmInfo.h"
#include "FITK_Interface/FITKInterfaceMesh/FITKUnstructuredMeshVTK.h"
#include "FITK_Component/FITKGmshExeDriver/FITKMeshGenGmshExecInterface.h"
#include "FITK_Interface/FITKInterfaceMeshGen/FITKAbstractMeshSizeInfoGenerator.h"
#include "FITK_Interface/FITKInterfaceModel/FITKComponentManager.h"
#include "FITK_Component/FITKGmshExeDriver/GUIGmshSettings.h"
#include "GUIWidget/GUIEnumType.h"
#include "GUIFrame/ControlPanel.h"

#include <QDir>
#include <QDateTime>
#include <QMessageBox>

namespace ModelOper
{
    namespace
    {
        // 刷新左侧控制面板中的网格树
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

    bool OperGmshGenerator::execGUI()
    {
        QWidget* mw = FITKAPP->getGlobalData()->getMainWindow();
        if (!mw) return false;

        if (_emitter->objectName() == "actionGmshClean")
        {
            this->meshClean();
        }
        if (_emitter->objectName() == "actionCreateGlobalMesh")
        {
            this->meshGenOper();
        }
        return false;        
    }

    bool OperGmshGenerator::execProfession()
    {
        return true;
    }

    void OperGmshGenerator::meshGenOper()
    {
        //获取工作目录
        QString meshPath = FITKAPP->getTempDir(false, "Gmsh");
        QString meshFile = QString("%1/%2").arg(meshPath).arg("mesh.msh");
        //获取网格划分接口
        Interface::FITKMeshGenInterface* mf = Interface::FITKMeshGenInterface::getInstance();
        if (!mf) return;
        Interface::FITKAbstractMesherDriver* mesher = mf->getMesherDriver("GmshExec");
        if (!mesher) return;

        //关联信号
        disconnect(mesher, SIGNAL(mesherFinished()), nullptr, nullptr);
        connect(mesher, SIGNAL(mesherFinished()), this, SLOT(meshGenFinished()));
        //设置参数
        mesher->setValue("MeshFile", meshFile);
        mesher->setValue("Method", 1);
        //开始划分网格
        mesher->startMesher();
        this->setArgs("MeshFile", meshFile);
    }

    void OperGmshGenerator::meshGenFinished()
    {
        QString meshFile = {};
        //获取网格文件，并判断是否生成成功
        if (!this->argValue<QString>("MeshFile", meshFile) || !QFileInfo(meshFile).isFile())
        {
            QMessageBox::warning(FITKAPP->getGlobalData()->getMainWindow(), tr("warning"), tr("Err! Mesher ProgramExec generate meshes failed."), QMessageBox::StandardButton::Ok);
            return;
        }

        //获取网格管理器
        ModelData::MeshManager* meshManager = FITKAPP->getGlobalData()->getMeshData<ModelData::MeshManager>();
        if (!meshManager) return;
        ModelData::MeshData* meshData = meshManager->getMeshDataObjectByNameT<ModelData::MeshData>("Gmsh");
        if (!meshData) return;
        //获取网格划分接口
        Interface::FITKMeshGenInterface* mf = Interface::FITKMeshGenInterface::getInstance();
        if (!mf) return;
        Interface::FITKAbstractMeshProcessor* processorMesh = mf->getMeshProcessor("GmshExec");
        if (!processorMesh) return;
        processorMesh->setValue("File", meshFile);
        processorMesh->setValue("FilterDim", QList<QVariant>() << 0 << 1);

        //创建网格数据
        Interface::FITKUnstructuredMesh* mesh = new Interface::FITKUnstructuredMesh;
        ModelData::MeshKernel* meshKernel = new ModelData::MeshKernel;
        meshData->appendDataObj(meshKernel);
        meshKernel->setMesh(mesh);
        //网格数据填充
        processorMesh->insertDataObject("Mesh", mesh);
        processorMesh->insertDataObject("ComponentManager", meshKernel->getComponentManager());
        //调用网格处理接口
        processorMesh->start(QStringList() << "MSH");
        int objID = meshKernel->getDataObjectID();

        AppFrame::FITKMessageNormal(QString("Read file succeed:%1 %2").arg(meshFile).arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss:zzz")));
        //更新渲染
        EventOper::GraphEventOperator* operGraph = FITKOPERREPO->getOperatorT<EventOper::GraphEventOperator>("GraphPreprocess");
        if (operGraph)
        {
            operGraph->updateGraph(objID, true);
        }
        AppFrame::FITKMessageNormal(QString("Graph succeed:%1").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss:zzz")));

        refreshMeshTree();
    }

    void OperGmshGenerator::meshClean()
    {
        //清除Gmesh网格
        ModelData::MeshManager* meshManager = FITKAPP->getGlobalData()->getMeshData<ModelData::MeshManager>();
        if (!meshManager) return;
        ModelData::MeshData* meshData = meshManager->getMeshDataObjectByNameT<ModelData::MeshData>("Gmsh");
        if (!meshData) return;
        meshData->clear();
        ////清除区域网格
        //Interface::FITKRegionMeshSizeManager* regionMeshSizeManager = Interface::FITKMeshGenInterface::getInstance()->getRegionMeshSizeMgr("GmshExec");
        //if (!regionMeshSizeManager) return;
        //regionMeshSizeManager->clear();
        //更新渲染
        EventOper::GraphEventOperator* operGraph = FITKOPERREPO->getOperatorT<EventOper::GraphEventOperator>("GraphPreprocess");
        if (operGraph)
        {
            operGraph->updateAllGraph();
        }

        refreshMeshTree();
    }
}
