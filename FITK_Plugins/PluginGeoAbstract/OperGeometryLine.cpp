#include "OperGeometryLine.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "OperatorsInterface/GraphEventOperator.h"
#include "GUIWidget/GUIEnumType.h"
#include "GUIFrame/MainWindow.h"
#include "GUIFrame/ControlPanel.h"

#include "FITK_Plugins/PluginGeoAbstract/GUIGeometryMergeLineDialog.h"
#include "FITK_Plugins/PluginGeoAbstract/GUIGeometryLineSplitterDialog.h"
#include <QWidget>

bool ModelOper::OperGeometryLine::execGUI(){
    //获取动作名称
    QString actionName = _emitter->objectName();
    if (actionName == "actionMergeLine")
    {
        this->createGeometryMergeLine();
    }
    else if (actionName == "actionCurveSplitter")
    {
        this->createGeometrySplitterLine();
    }
    return false;
}

bool ModelOper::OperGeometryLine::execProfession()
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
bool ModelOper::OperGeometryLine::createGeometryMergeLine()
{
    //主界面
    QWidget* mw = FITKAPP->getGlobalData()->getMainWindow();
    if (!mw) return false;
    //创建界面
    GUI::GUIGeometryMergeLineDialog* dialog = new GUI::GUIGeometryMergeLineDialog(this, mw);
    //当界面执行完毕后执行
    connect(dialog, &QDialog::accepted, [this]() { this->execProfession(); });
    dialog->show();
    return true;
}
bool ModelOper::OperGeometryLine::createGeometrySplitterLine()
{
    //主界面
    QWidget* mw = FITKAPP->getGlobalData()->getMainWindow();
    if (!mw) return false;
    //创建界面
    GUI::GUIGeometryLineSplitterDialog* dialog = new GUI::GUIGeometryLineSplitterDialog(this, mw);
    //当界面执行完毕后执行
    connect(dialog, &QDialog::accepted, [this]() { this->execProfession(); });
    dialog->show();
    return true;
}

