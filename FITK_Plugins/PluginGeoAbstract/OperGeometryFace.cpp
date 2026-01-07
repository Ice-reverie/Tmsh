#include "OperGeometryFace.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "OperatorsInterface/GraphEventOperator.h"
#include "GUIWidget/GUIEnumType.h"
#include "GUIFrame/MainWindow.h"
#include "GUIFrame/ControlPanel.h"

#include "FITK_Plugins/PluginGeoAbstract/GUIGeometryFillGapsDialog.h"
#include "FITK_Plugins/PluginGeoAbstract/GUIGeometryFaceSplitterDialog.h"
#include <QWidget>

bool ModelOper::OperGeometryFace::execGUI(){
    //获取动作名称
    QString actionName = _emitter->objectName();
    if (actionName == "actionFillGap")
    {
        this->createGeometryFillGaps();
    }
    else if (actionName == "actionSurfaceSplitter")
    {
        this->createGeometryFaceSplitter();
    }
    return false;
}

bool ModelOper::OperGeometryFace::execProfession()
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
bool ModelOper::OperGeometryFace::createGeometryFillGaps()
{
    //主界面
    QWidget* mw = FITKAPP->getGlobalData()->getMainWindow();
    if (!mw) return false;
    //创建界面
    GUI::GUIGeometryFillGapsDialog* dialog = new GUI::GUIGeometryFillGapsDialog(this, mw);
    //当界面执行完毕后执行
    connect(dialog, &QDialog::accepted, [this]() { this->execProfession(); });
    dialog->show();
    return true;
}
bool ModelOper::OperGeometryFace::createGeometryFaceSplitter()
{
    //主界面
    QWidget* mw = FITKAPP->getGlobalData()->getMainWindow();
    if (!mw) return false;
    //创建界面
    GUI::GUIGeometryFaceSplitterDialog* dialog = new GUI::GUIGeometryFaceSplitterDialog(this, mw);
    //当界面执行完毕后执行
    connect(dialog, &QDialog::accepted, [this]() { this->execProfession(); });
    dialog->show();
    return true;
}
