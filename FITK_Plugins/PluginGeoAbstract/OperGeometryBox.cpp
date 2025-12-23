#include "OperGeometryBox.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "OperatorsInterface/GraphEventOperator.h"
#include "GUIWidget/GUIEnumType.h"
#include "GUIFrame/MainWindow.h"
#include "GUIFrame/ControlPanel.h"
#include <QWidget>

#include "FITK_Plugins/PluginGeoAbstract/GUIGeometryBoxDialog.h"
#include "FITK_Plugins/PluginGeoAbstract/GUIGeometryCylinderDialog.h"
#include "FITK_Plugins/PluginGeoAbstract/GUIGeometrySphereDialog.h"
#include "FITK_Plugins/PluginGeoAbstract/GUIGeometryImprintSolidDialog.h"
#include "FITK_Plugins/PluginGeoAbstract/GUIGeometrySolidSplitterDialog.h"

bool ModelOper::OperGeometryBox::execGUI(){
    //获取动作名称
    QString actionName = _emitter->objectName();
    if (actionName == "actionCreateBox")
    {
        this->createGeometryBox();
    }
    else if (actionName == "actionCreateCylinder")
    {
        this->createGeometryCylinder();
    }
    else if (actionName == "actionCreateSphere")
    {
        this->createGeometrySphere();
    }
    else if (actionName == "actionImprintSolid")
    {
        this->createGeometryImprintSolid();
    }
    else if (actionName == "actionSolidSplitter")
    {
        this->createGeometrySolidSplitter();
    }
    return false;
}

bool ModelOper::OperGeometryBox::execProfession()
{
    int objID = -1;
    this->argValue("objID", objID);
    //获取三维可视对象数据-用于渲染数据对象
    EventOper::GraphEventOperator* graphOper = FITKOPERREPO->getOperatorT<EventOper::GraphEventOperator>("GraphPreprocess");
    if (graphOper == nullptr)return false;
    graphOper->updateGraphRecursively(objID, 2, false);
    //刷新树
    GUI::MainWindow* mw = GUI::MainWindow::GetMainWindowFromFramework();
    if (mw == nullptr) return false;
    GUI::ControlPanel* cp = mw->getControlPanel();
    if (cp == nullptr) return false;
    GUI::TreeWidgetPage* tw = cp->getTreeWidget(GUI::TreeWidgetType::PageModel);
    if (tw == nullptr) return false;
    tw->updatePage();
    return true;
}
bool ModelOper::OperGeometryBox::createGeometryBox()
{
    //主界面
    QWidget* mw = FITKAPP->getGlobalData()->getMainWindow();
    if (!mw) return false;
    //创建界面
    GUI::GUIGeometryBoxDialog* dialog = new GUI::GUIGeometryBoxDialog(this, mw);
    //当界面执行完毕后执行
    connect(dialog, &QDialog::accepted, [this]() { this->execProfession(); });
    dialog->show();
    return true;
}
bool ModelOper::OperGeometryBox::createGeometryCylinder()
{
    //主界面
    QWidget* mw = FITKAPP->getGlobalData()->getMainWindow();
    if (!mw) return false;
    //创建界面
    GUI::GUIGeometryCylinderDialog* dialog = new GUI::GUIGeometryCylinderDialog(this, mw);
    //当界面执行完毕后执行
    connect(dialog, &QDialog::accepted, [this]() { this->execProfession(); });
    dialog->show();
    return true;
}
bool ModelOper::OperGeometryBox::createGeometrySphere()
{
    //主界面
    QWidget* mw = FITKAPP->getGlobalData()->getMainWindow();
    if (!mw) return false;
    //创建界面
    GUI::GUIGeometrySphereDialog* dialog = new GUI::GUIGeometrySphereDialog(this, mw);
    //当界面执行完毕后执行
    connect(dialog, &QDialog::accepted, [this]() { this->execProfession(); });
    dialog->show();
    return true;
}
bool ModelOper::OperGeometryBox::createGeometryImprintSolid()
{
    //主界面
    QWidget* mw = FITKAPP->getGlobalData()->getMainWindow();
    if (!mw) return false;
    //创建界面
    GUI::GUIGeometryImprintSolidDialog* dialog = new GUI::GUIGeometryImprintSolidDialog(this, mw);
    //当界面执行完毕后执行
    connect(dialog, &QDialog::accepted, [this]() { this->execProfession(); });
    dialog->show();
    return true;
}
bool ModelOper::OperGeometryBox::createGeometrySolidSplitter()
{
    //主界面
    QWidget* mw = FITKAPP->getGlobalData()->getMainWindow();
    if (!mw) return false;
    //创建界面
    GUI::GUIGeometrySolidSplitterDialog* dialog = new GUI::GUIGeometrySolidSplitterDialog(this, mw);
    //当界面执行完毕后执行
    connect(dialog, &QDialog::accepted, [this]() { this->execProfession(); });
    dialog->show();
    return true;
}