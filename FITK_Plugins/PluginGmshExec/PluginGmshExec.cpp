#include "PluginGmshExec.h"
#include "PluginAbstract/PluginActionOpersManager.h"
#include "FITK_Kernel/FITKAppFramework/FITKComponents.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Component/FITKGmshExeDriver/FITKMeshGenGmshExecInterface.h"
#include "ModelData\MeshManager.h"
#include "OperGmshGlobalSize.h"
#include "OperGmshGenerator.h"
#include "MeshDataGmshExec.h"
#include "OperRegionMeshSize.h"

AppFrame::FITKAbstractPlugin*  FITKLibraryLoadFun(QLibrary* lib)
{
    return new Plugin::PluginGmshExec(lib);
}

QString FITKLibraryRecognizeFun()
{
    return Plugin::PluginAbstract::PluginKey();
}
namespace Plugin
{
    PluginGmshExec::PluginGmshExec(QLibrary* dylibrary) : Plugin::PluginGridAbstract(dylibrary)
    {
    }

    PluginGmshExec::~PluginGmshExec()
    {

    }

    QString PluginGmshExec::getPluginName()
    {
        return "Gmsh";
    }

    void PluginGmshExec::install()
    {
        this->initRibbonUI();
        //创建组件
        _gMshExecGen = new Gmsh::FITKMeshGenGmshExecInterface;

        //交由主程序管理组件
        FITKAPP->getComponents()->addComponent(_gMshExecGen);
        // 添加操作器
        this->addOperator("actionCreateGlobalMesh", []() { return new ModelOper::OperGmshGenerator(); });
        //this->addOperator("actionGmshGenerator2D", []() { return new ModelOper::OperGmshGenerator(); });
        //this->addOperator("actionGmshGenerator3D", []() { return new ModelOper::OperGmshGenerator(); });
        //this->addOperator("actionGmshClean", []() { return new ModelOper::OperGmshGenerator(); });

        this->addOperator("actionRegionMeshSizeBox", []() { return new ModelOper::OperRegionMeshSize(); });
        this->addOperator("actionRegionMeshSizeCylinder", []() { return new ModelOper::OperRegionMeshSize(); });
        this->addOperator("actionRegionMeshSizeSphere", []() { return new ModelOper::OperRegionMeshSize(); });
        this->addOperator("actionRegionMeshInfo", []() { return new ModelOper::OperRegionMeshSize(); });

        AppFrame::FITKGlobalData* globalData = FITKAPP->getGlobalData();
        ModelData::MeshManager* mgr = globalData->getMeshData<ModelData::MeshManager>();
        if (mgr == nullptr) return;
        mgr->registerMeshDataCreator(this->getPluginName(), [&]() {return new ModelData::MeshDataGmshExec; });
    }

    void PluginGmshExec::unInstall()
    {
        //清空数据
        AppFrame::FITKGlobalData* globalData = FITKAPP->getGlobalData();
        ModelData::MeshManager* mgr = globalData->getMeshData<ModelData::MeshManager>();
        if (mgr != nullptr)
            mgr->unregisterMeshDataCreator(this->getPluginName());
        //删除组件
        PluginGridAbstract::unInstall();
        //remove时，会delete对象
        FITKAPP->getComponents()->removeComponent(_gMshExecGen);
        //不能重复delete
        _gMshExecGen = nullptr;
    }

    void PluginGmshExec::initRibbonUI()
    {
        //添加界面
        if (_actOpersMgr == nullptr) return;

        SARibbonCategory* page = _actOpersMgr->createPage("Gmsh");

        SARibbonPannel* panel = _actOpersMgr->addPanelToCategory(page, "Mesh");
        QAction* action = _actOpersMgr->createAction("Settings", "actionCreateGlobalMesh", QIcon(":/icons/Mesh_Settings.svg"));
        _actOpersMgr->addActionToPanel(panel, action, SARibbonPannelItem::Large);

        //panel = _actOpersMgr->addPanelToCategory(page, QObject::tr("Mesh-Generator"));
        //action = _actOpersMgr->createAction("2D", "actionGmshGenerator2D", QIcon());
        //_actOpersMgr->addActionToPanel(panel, action, SARibbonPannelItem::Large);
        //action = _actOpersMgr->createAction("3D", "actionGmshGenerator3D", QIcon());
        //_actOpersMgr->addActionToPanel(panel, action, SARibbonPannelItem::Large);
        //action = _actOpersMgr->createAction("Clean", "actionGmshClean", QIcon());
        //_actOpersMgr->addActionToPanel(panel, action, SARibbonPannelItem::Large);

        panel = _actOpersMgr->addPanelToCategory(page, QObject::tr("Region Mesh Size"));
        action = _actOpersMgr->createAction("Box", "actionRegionMeshSizeBox", QIcon(":/icons/Mesh_Box_Mesh.svg"));
        _actOpersMgr->addActionToPanel(panel, action, SARibbonPannelItem::Large);
        action = _actOpersMgr->createAction("Cylinder", "actionRegionMeshSizeCylinder", QIcon(":/icons/Mesh_Cylinder_Mesh.svg"));
        _actOpersMgr->addActionToPanel(panel, action, SARibbonPannelItem::Large);
        action = _actOpersMgr->createAction("Sphere", "actionRegionMeshSizeSphere", QIcon(":/icons/Mesh_Sphere_Mesh.svg"));
        _actOpersMgr->addActionToPanel(panel, action, SARibbonPannelItem::Large);
        action = _actOpersMgr->createAction("Region Info", "actionRegionMeshInfo", QIcon(":/icons/Mesh_Region_Info.svg"));
        _actOpersMgr->addActionToPanel(panel, action, SARibbonPannelItem::Large);
    }
}

