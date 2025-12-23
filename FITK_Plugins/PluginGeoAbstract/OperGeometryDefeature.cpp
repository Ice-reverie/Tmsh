#include "OperGeometryDefeature.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "OperatorsInterface/GraphEventOperator.h"
#include "GUIWidget/GUIEnumType.h"
#include "GUIFrame/MainWindow.h"
#include "GUIFrame/ControlPanel.h"
#include <QWidget>

#include "FITK_Plugins/PluginGeoAbstract/GUIGeometryChamferDialog.h"
#include "FITK_Plugins/PluginGeoAbstract/GUIGeometryFilletDialog.h"
#include "FITK_Plugins/PluginGeoAbstract/GUIGeometryDefeatureDialog.h"

bool ModelOper::OperGeometryDefeature::execGUI(){
    //获取动作名称
    QString actionName = _emitter->objectName();
    if (actionName == "actionChamfer")
    {
        this->createGeometryChamfer();
    }
    else if (actionName == "actionFillet")
    {
        this->createGeometryFIllet();
    }
    else if (actionName == "actionDeleteFace")
    {
        this->createGeometryDefeature();
    }
    return false;
}

bool ModelOper::OperGeometryDefeature::execProfession()
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
bool ModelOper::OperGeometryDefeature::createGeometryChamfer()
{
    //主界面
    QWidget* mw = FITKAPP->getGlobalData()->getMainWindow();
    if (!mw) return false;
    //创建界面
    GUI::GUIGeometryChamferDialog* dialog = new GUI::GUIGeometryChamferDialog(this, mw);
    //当界面执行完毕后执行
    connect(dialog, &QDialog::accepted, [this]() { this->execProfession(); });
    dialog->show();
    return true;
}
bool ModelOper::OperGeometryDefeature::createGeometryFIllet()
{
    //主界面
    QWidget* mw = FITKAPP->getGlobalData()->getMainWindow();
    if (!mw) return false;
    //创建界面
    GUI::GUIGeometryFilletDialog* dialog = new GUI::GUIGeometryFilletDialog(this, mw);
    //当界面执行完毕后执行
    connect(dialog, &QDialog::accepted, [this]() { this->execProfession(); });
    dialog->show();
    return true;
}
bool ModelOper::OperGeometryDefeature::createGeometryDefeature()
{
    //主界面
    QWidget* mw = FITKAPP->getGlobalData()->getMainWindow();
    if (!mw) return false;
    //创建界面
    GUI::GUIGeometryDefeatureDialog* dialog = new GUI::GUIGeometryDefeatureDialog(this, mw);
    //当界面执行完毕后执行
    connect(dialog, &QDialog::accepted, [this]() { this->execProfession(); });
    dialog->show();
    return true;
}
