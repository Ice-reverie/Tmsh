#include "FITKMesherDriverGmshExec.h"

#include "FITKGmshExecProgramInputInfo.h"
#include "FITKGmshExecProgramDriver.h"
#include "GUIGmshSettings.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Kernel/FITKAppFramework/FITKProgramTaskManager.h"
#include "FITK_Kernel/FITKAppFramework/FITKMessage.h"
#include "FITK_Kernel/FITKCore/FITKDirFileTools.h"
#include "FITK_Kernel/FITKCore/FITKThreadPool.h"

#include "FITK_Interface/FITKInterfaceGeometry/FITKGeoInterfaceFactory.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoModelExportTopos.h"

#include "FITK_Interface/FITKInterfaceMeshGen/FITKMeshGenInterface.h"
#include "FITK_Interface/FITKInterfaceMeshGen/FITKGlobalMeshGenerateAlgorithmInfo.h"

#include <QFileInfo>
#include <QTimer>
#include <QDir>

namespace Gmsh
{
    FITKMesherDriverGmshExec::FITKMesherDriverGmshExec()
    {

    }

    FITKMesherDriverGmshExec::~FITKMesherDriverGmshExec()
    {

    }
    /**
     * @brief    Method(划分方式）
     *           0或者空（直接驱动Gmsh划分所需参数)：ShapeFile（需要划分的模型文件）、MeshFile（划分完成的网格文件路径）、ScriptFile（驱动Gmsh网格划分的脚本文件）
     *           1(通过setting界面拾取模型并进行划分设置)：MeshFile（划分完成的网格文件路径）  注：ShapeFile（需要划分的模型文件）、ScriptFile（驱动Gmsh网格划分的脚本文件）会通过界面拾取与设置的数据进行自动生成
     * @author   liuzhonghua (liuzhonghuaszch@163.com)
     * @date     2025-10-17
     */
    void FITKMesherDriverGmshExec::startMesher(QStringList info /*= QStringList()*/)
    {
        // TODO: 实现网格生成的启动逻辑
        Q_UNUSED(info);
        //获取划分方式
        int method = 0;
        if (!this->getValue("Method").isNull())
            method = this->getValueT<int>("Method");
        //直接驱动Gmsh划分所需参数
        if (method == 0)
            this->startGmshMeshGenerationDriven();
        //通过setting界面拾取模型并进行划分设置
        else if (method == 1)
            this->gmshMeshGenSetting();
    }
    
    void FITKMesherDriverGmshExec::stopMesher(QStringList info /*= QStringList()*/)
    {
        //todo
    }

    void FITKMesherDriverGmshExec::gmshMeshGenSetting()
    {
        //gmsh网格生成设置
        GUI::GUIGmshSettings* dialog = new GUI::GUIGmshSettings(this, FITKAPP->getGlobalData()->getMainWindow());
        //当界面执行完毕后执行
        connect(dialog, &QDialog::accepted, this, &FITKMesherDriverGmshExec::writerModelGeoScriptFileDriven);
        dialog->show();
    }

    void FITKMesherDriverGmshExec::startGmshMeshGenerationDriven()
    {
        //获取划分维度、模型文件和网格文件
        QString shapeFile = this->getValueT<QString>("ShapeFile");
        QString meshFile = this->getValueT<QString>("MeshFile");
        QString geoScriptFile = this->getValueT<QString>("ScriptFile");
        if (shapeFile.isEmpty() || meshFile.isEmpty() || geoScriptFile.isEmpty()) return;
        //创建Gmsh可执行程序驱动输入信息
        GmshExe::FITKGmshExecProgramInputInfo* inputInfo = new GmshExe::FITKGmshExecProgramInputInfo;
        inputInfo->setGeoScriptFile(geoScriptFile);
        inputInfo->setMeshFile("msh", meshFile);
        //获取可执行程序驱动管理器
        AppFrame::FITKProgramTaskManeger* programTaskMgr = FITKAPP->getProgramTaskManager();
        GmshExe::FITKGmshExecProgramDriver* gmshExe = dynamic_cast<GmshExe::FITKGmshExecProgramDriver*>(programTaskMgr->createProgram(1, "FITKGmshExecProgramDriver", inputInfo));
        if (!gmshExe)return;
        gmshExe->writeConfigFile(shapeFile, geoScriptFile, meshFile);
        if (!gmshExe->isExistExeProgram())
        {
            delete gmshExe;
            return;
        }
        connect(gmshExe, SIGNAL(sig_Finish()), this, SIGNAL(mesherFinished()));
        //开始驱动Gmsh进行网格划分
        gmshExe->start();
    }

    void FITKMesherDriverGmshExec::writerModelGeoScriptFileDriven()
    {
        //获取几何虚拓扑数据
        QVariant v = {};
        v = this->getValue("virtualTopos");
        QList<Interface::VirtualShape> topos = v.value<QList<Interface::VirtualShape>>();
        if (topos.size() == 0) return;
        //获取工作目录
        QString meshPath = FITKAPP->getTempDir(false, "Gmsh");
        //指定网格划分路径
        QString shapeFile = meshPath + "/geometryFile.brep";//指定软件生成的几何文件
        QString scriptFile = QString("%1/%2").arg(meshPath).arg("script.geo");//指定网格划分驱动程序所需的脚本文件路径
        //确保网格划分的工作路径存在
        QDir meshDir(meshPath);
        if (!meshDir.exists())
            meshDir.mkdir(meshPath);
        //移除旧网格划分结果，避免干扰新结果
        Core::ClearDir(meshPath);
        //获取网格划分接口
        Interface::FITKMeshGenInterface* mf = Interface::FITKMeshGenInterface::getInstance();
        if (!mf) return;
        Interface::FITKGlobalMeshGenerateAlgorithmInfo* meshGenAlgInfo = mf->getGlobalMeshGenerateAlgorithmInfo("GmshExec");
        if (!meshGenAlgInfo) return;
        //创建几何文件生成工具
        Interface::FITKInterfaceGeometryFactory* geoFactory = Interface::FITKInterfaceGeometryFactory::getInstance();
        if (!geoFactory) return;
        Interface::FITKAbsGeoModelExportTopos* geoExport = dynamic_cast<Interface::FITKAbsGeoModelExportTopos*>
            (geoFactory->createCommand(Interface::FITKGeoEnum::FITKGeometryComType::FGTExportTopos));
        if (!geoExport) return;
        //设置几何数据并执行模型文件写出
        if (meshGenAlgInfo->getMeshGenerateDimension() == 2)//最高维度仅是2维才进行几何缝合
            geoExport->setEnableStitch(meshGenAlgInfo->getGeometryStitch());
        geoExport->setExportTopos(topos);
        geoExport->setFileName(shapeFile);
        if (geoExport->update() == false) return;
        //设置网格划分参数
        this->setValue("ShapeFile", shapeFile);
        this->setValue("ScriptFile", scriptFile);
        //开始网格划分
        this->startGmshMeshGenerationDriven();
    }

}
