#include "PluginTmshExec.h"

#include "PluginAbstract/PluginActionOpersManager.h"
#include "FITK_Kernel/FITKAppFramework/FITKComponents.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"

#include "FITK_Component/FITKTmshExeDriver/FITKMeshGenTmshExecInterface.h"
#include "FITK_Component/FITKTmshMshIO/FITKTmshMshIOInterface.h"
#include "ModelData/MeshManager.h"

#include "OperTmshGenerator.h"
#include "MeshDataTmshExec.h"

AppFrame::FITKAbstractPlugin* FITKLibraryLoadFun(QLibrary* lib)
{
    return new Plugin::PluginTmshExec(lib);
}

QString FITKLibraryRecognizeFun()
{
    return Plugin::PluginAbstract::PluginKey();
}

namespace Plugin
{
    PluginTmshExec::PluginTmshExec(QLibrary* dylibrary) : Plugin::PluginGridAbstract(dylibrary)
    {
    }

    PluginTmshExec::~PluginTmshExec()
    {
    }

    QString PluginTmshExec::getPluginName()
    {
        return "Tmsh";
    }

    void PluginTmshExec::install()
    {
        this->initRibbonUI();

        _tMshExecGen = new Tmsh::FITKMeshGenTmshExecInterface;

		// 组件管理器添加TmeshIO组件
		Tmsh::FITKTmshMshIOInterface* tmshIO = new Tmsh::FITKTmshMshIOInterface;

        FITKAPP->getComponents()->addComponent(_tMshExecGen);
        FITKAPP->getComponents()->addComponent(tmshIO);


        this->addOperator("actionTmshCreateGlobalMesh", []() { return new ModelOper::OperTmshGenerator(); });

//        this->addOperator("actionTmshRegionMeshSizeBox", []() { return new ModelOper::OperRegionMeshSizeTmsh(); });
//        this->addOperator("actionTmshRegionMeshSizeCylinder", []() { return new ModelOper::OperRegionMeshSizeTmsh(); });
//        this->addOperator("actionTmshRegionMeshSizeSphere", []() { return new ModelOper::OperRegionMeshSizeTmsh(); });
//        this->addOperator("actionTmshRegionMeshInfo", []() { return new ModelOper::OperRegionMeshSizeTmsh(); });

        AppFrame::FITKGlobalData* globalData = FITKAPP->getGlobalData();
        ModelData::MeshManager* mgr = globalData->getMeshData<ModelData::MeshManager>();
        if (mgr == nullptr) return;
        mgr->registerMeshDataCreator(this->getPluginName(), [&]() { return new ModelData::MeshDataTmshExec; });
    }

    void PluginTmshExec::unInstall()
    {
        AppFrame::FITKGlobalData* globalData = FITKAPP->getGlobalData();
        ModelData::MeshManager* mgr = globalData->getMeshData<ModelData::MeshManager>();
        if (mgr != nullptr)
        {
            mgr->unregisterMeshDataCreator(this->getPluginName());
        }

        PluginGridAbstract::unInstall();
        FITKAPP->getComponents()->removeComponent(_tMshExecGen);
        _tMshExecGen = nullptr;
    }

    void PluginTmshExec::initRibbonUI()
    {
        if (_actOpersMgr == nullptr) return;

        SARibbonCategory* page = _actOpersMgr->createPage("Tmsh");

        SARibbonPannel* panel = _actOpersMgr->addPanelToCategory(page, "Mesh");
        QAction* action = _actOpersMgr->createAction("Settings", "actionTmshCreateGlobalMesh", QIcon(":/icons/Mesh_Settings.svg"));
        _actOpersMgr->addActionToPanel(panel, action, SARibbonPannelItem::Large);

//        panel = _actOpersMgr->addPanelToCategory(page, QObject::tr("Region Mesh Size"));
//        action = _actOpersMgr->createAction("Box", "actionTmshRegionMeshSizeBox", QIcon(":/icons/Mesh_Box_Mesh.svg"));
//        _actOpersMgr->addActionToPanel(panel, action, SARibbonPannelItem::Large);
//        action = _actOpersMgr->createAction("Cylinder", "actionTmshRegionMeshSizeCylinder", QIcon(":/icons/Mesh_Cylinder_Mesh.svg"));
//        _actOpersMgr->addActionToPanel(panel, action, SARibbonPannelItem::Large);
//        action = _actOpersMgr->createAction("Sphere", "actionTmshRegionMeshSizeSphere", QIcon(":/icons/Mesh_Sphere_Mesh.svg"));
//        _actOpersMgr->addActionToPanel(panel, action, SARibbonPannelItem::Large);
//        action = _actOpersMgr->createAction("Region Info", "actionTmshRegionMeshInfo", QIcon(":/icons/Mesh_Region_Info.svg"));
//        _actOpersMgr->addActionToPanel(panel, action, SARibbonPannelItem::Large);
    }
}

