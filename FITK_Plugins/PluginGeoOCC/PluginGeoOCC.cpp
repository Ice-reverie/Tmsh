#include "PluginGeoOCC.h"
#include "PluginAbstract/PluginActionOpersManager.h"
#include "FITK_Kernel/FITKAppFramework/FITKComponents.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKGeoCommandList.h"
#include "FITK_Component/FITKGeoCompOCC/FITKGeoCompOCCInterface.h"
#include "FITK_Plugins/PluginGeoAbstract/OperGeometryBox.h"
#include "FITK_Plugins/PluginGeoAbstract/OperGeometryBool.h"
#include "FITK_Plugins/PluginGeoAbstract/OperGeometryDefeature.h"
#include "FITK_Plugins/PluginGeoAbstract/OperGeometryTransformation.h"
#include "FITK_Plugins/PluginGeoAbstract/OperGeometryLine.h"
#include "FITK_Plugins/PluginGeoAbstract/OperGeometryFace.h"

AppFrame::FITKAbstractPlugin*  FITKLibraryLoadFun(QLibrary* lib)
{
    return new PluginGeoOCC(lib);
}

QString FITKLibraryRecognizeFun()
{
    return Plugin::PluginAbstract::PluginKey();
}

PluginGeoOCC::PluginGeoOCC(QLibrary* dylibrary) : Plugin::PluginGeoAbstract(dylibrary)
{
}

PluginGeoOCC::~PluginGeoOCC()
{

}

QString PluginGeoOCC::getPluginName()
{
    return "OpenCASCADE";
}

void PluginGeoOCC::install()
{
    this->initRibbonUI();
    //创建组件
    _occComp = new OCC::FITKGeoCompOCCInterface;

    //交由主程序管理组件
    FITKAPP->getComponents()->addComponent(_occComp);

    // 添加几何体操作器
    this->addOperator("actionCreateBox", []() { return new ModelOper::OperGeometryBox(); });
    this->addOperator("actionCreateCylinder", []() { return new ModelOper::OperGeometryBox(); });
    this->addOperator("actionCreateSphere", []() { return new ModelOper::OperGeometryBox(); });
    this->addOperator("actionImprintSolid", []() { return new ModelOper::OperGeometryBox(); });
    this->addOperator("actionSolidSplitter", []() { return new ModelOper::OperGeometryBox(); });
    // 添加bool运算操作器
    this->addOperator("actionBooleanDifference", []() { return new ModelOper::OperGeometryBool(); });
    this->addOperator("actionBooleanUnion", []() { return new ModelOper::OperGeometryBool(); });
    this->addOperator("actionBooleanIntersection", []() { return new ModelOper::OperGeometryBool(); });
    // 添加几何特征操作器
    this->addOperator("actionChamfer", []() { return new ModelOper::OperGeometryDefeature(); });
    this->addOperator("actionFillet", []() { return new ModelOper::OperGeometryDefeature(); });
    this->addOperator("actionDeleteFace", []() { return new ModelOper::OperGeometryDefeature(); });
    // 添加特征变换操作器
    this->addOperator("actionMirror", []() { return new ModelOper::OperGeometryTransformation(); });
    this->addOperator("actionRectangularPattern", []() { return new ModelOper::OperGeometryTransformation(); });
    this->addOperator("actionCircularPattern", []() { return new ModelOper::OperGeometryTransformation(); });
    //添加线操作器
    this->addOperator("actionMergeLine", []() { return new ModelOper::OperGeometryLine(); });
    this->addOperator("actionCurveSplitter", []() { return new ModelOper::OperGeometryLine(); });
    //添加面操作器
    this->addOperator("actionFillGap", []() { return new ModelOper::OperGeometryFace(); });
    this->addOperator("actionSurfaceSplitter", []() { return new ModelOper::OperGeometryFace(); });
}

void PluginGeoOCC::unInstall()
{
    //删除组件
    PluginGeoAbstract::unInstall();

    //清空数据
    Interface::FITKGeoCommandList* geometryData = FITKAPP->getGlobalData()->
        getGeometryData<Interface::FITKGeoCommandList>();
    if (geometryData != nullptr) geometryData->clear();
     
    //remove时，会delete对象
    FITKAPP->getComponents()->removeComponent(_occComp);
    //不能重复delete
    _occComp = nullptr;
}

    void PluginGeoOCC::initRibbonUI()
    {
        //添加界面
        if (_actOpersMgr == nullptr) return;
   
        SARibbonCategory* page = _actOpersMgr->createPage("OpenCASCADE");

        SARibbonPannel* panel = _actOpersMgr->addPanelToCategory(page, "Model");
        QAction* actionBox = _actOpersMgr->createAction("Box", "actionCreateBox", QIcon(":/icons/Box.svg"));
        _actOpersMgr->addActionToPanel(panel, actionBox, SARibbonPannelItem::Large);
        QAction* actionCylinder = _actOpersMgr->createAction("Cylinder", "actionCreateCylinder", QIcon(":/icons/Cylinder.svg"));
        _actOpersMgr->addActionToPanel(panel, actionCylinder, SARibbonPannelItem::Large);
        QAction* actionSphere = _actOpersMgr->createAction("Sphere", "actionCreateSphere", QIcon(":/icons/Sphere.svg"));
        _actOpersMgr->addActionToPanel(panel, actionSphere, SARibbonPannelItem::Large);
        // 添加布尔运算的panel
        SARibbonPannel* booleanPanel = _actOpersMgr->addPanelToCategory(page, "Boolean Operations");
        QAction* actionDifference = _actOpersMgr->createAction("Difference", "actionBooleanDifference", QIcon(":/icons/Difference.svg"));
        _actOpersMgr->addActionToPanel(booleanPanel, actionDifference, SARibbonPannelItem::Large);

        QAction* actionUnion = _actOpersMgr->createAction("Union", "actionBooleanUnion", QIcon(":/icons/Union.svg"));
        _actOpersMgr->addActionToPanel(booleanPanel, actionUnion, SARibbonPannelItem::Large);

        QAction* actionIntersection = _actOpersMgr->createAction("Intersection", "actionBooleanIntersection", QIcon(":/icons/Intersection.svg"));
        _actOpersMgr->addActionToPanel(booleanPanel, actionIntersection, SARibbonPannelItem::Large);
    
        // 添加defeature的panel
        SARibbonPannel* defeaturePanel = _actOpersMgr->addPanelToCategory(page, "Defeature");
        QAction* actionChamfer = _actOpersMgr->createAction("Chamfer", "actionChamfer", QIcon(":/icons/Chamfer.svg"));
        _actOpersMgr->addActionToPanel(defeaturePanel, actionChamfer, SARibbonPannelItem::Large);
        QAction* actionFillet = _actOpersMgr->createAction("Fillet", "actionFillet", QIcon(":/icons/Fillet.svg"));
        _actOpersMgr->addActionToPanel(defeaturePanel, actionFillet, SARibbonPannelItem::Large);
        QAction* actionDeleteFace = _actOpersMgr->createAction("Delete Face", "actionDeleteFace", QIcon(":/icons/Delete_Face.svg"));
        _actOpersMgr->addActionToPanel(defeaturePanel, actionDeleteFace, SARibbonPannelItem::Large);

        //添加几何线的panel
        SARibbonPannel* linePanel = _actOpersMgr->addPanelToCategory(page, "Line");
        QAction* actionMergeLine = _actOpersMgr->createAction("MergeLine", "actionMergeLine", QIcon(":/icons/Merge_Line.svg"));
        _actOpersMgr->addActionToPanel(linePanel, actionMergeLine, SARibbonPannelItem::Large);

        //添加几何面的panel
        SARibbonPannel* facePanel = _actOpersMgr->addPanelToCategory(page, "Face");
        QAction* actionFillGap = _actOpersMgr->createAction("Fill Gaps", "actionFillGap", QIcon(":/icons/Fill_Gaps.svg"));
        _actOpersMgr->addActionToPanel(facePanel, actionFillGap, SARibbonPannelItem::Large);
        //添加几何体的panel
        SARibbonPannel* solidPanel = _actOpersMgr->addPanelToCategory(page, "Solid");
        QAction* actionImprintSolid = _actOpersMgr->createAction("ImprintSolid", "actionImprintSolid", QIcon(":/icons/Imprint_Solid.svg"));
        _actOpersMgr->addActionToPanel(solidPanel, actionImprintSolid, SARibbonPannelItem::Large);

        //添加模型变换的panel
        SARibbonPannel* transformationPanel = _actOpersMgr->addPanelToCategory(page, "Transformation");
        QAction* actionMirror = _actOpersMgr->createAction("Mirror", "actionMirror", QIcon(":/icons/Mirror.svg"));
        _actOpersMgr->addActionToPanel(transformationPanel, actionMirror, SARibbonPannelItem::Large);
        QAction* recPatternMirror = _actOpersMgr->createAction("Rectangular Pattern", "actionRectangularPattern", QIcon(":/icons/Rect_Pattern.svg"));
        _actOpersMgr->addActionToPanel(transformationPanel, recPatternMirror, SARibbonPannelItem::Large);
        QAction* cirPatternMirror = _actOpersMgr->createAction("Circular Pattern", "actionCircularPattern", QIcon(":/icons/Circ_Pattern.svg"));
        _actOpersMgr->addActionToPanel(transformationPanel, cirPatternMirror, SARibbonPannelItem::Large);

        //添加模型切割的panel
        SARibbonPannel* splitterPanel = _actOpersMgr->addPanelToCategory(page, "Splitter");
        QAction* actionCurveSplitter = _actOpersMgr->createAction("Curve Splitter", "actionCurveSplitter", QIcon(":/icons/Curve_Split.svg"));
        _actOpersMgr->addActionToPanel(splitterPanel, actionCurveSplitter, SARibbonPannelItem::Large);
        QAction* actionSurfaceSplitter = _actOpersMgr->createAction("Surface Splitter", "actionSurfaceSplitter", QIcon(":/icons/Surf_Split.svg"));
        _actOpersMgr->addActionToPanel(splitterPanel, actionSurfaceSplitter, SARibbonPannelItem::Large);
        QAction* actionSolidSplitter = _actOpersMgr->createAction("Soild Splitter", "actionSolidSplitter", QIcon(":/icons/Solid_Split.svg"));
        _actOpersMgr->addActionToPanel(splitterPanel, actionSolidSplitter, SARibbonPannelItem::Large);
    }

