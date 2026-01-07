#include "GUIGeometryGroupDialog.h"
#include "GUIGeometryFaceGroupSelectWidget.h"
#include "ui_GUIGeometryGroupDialog.h"

#include "FITK_Kernel/FITKCore/FITKActionOperator.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppSettings.h" 
#include "FITK_Kernel/FITKCore/FITKOperatorRepo.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKGeoCommandList.h"
#include "FITK_Interface/FITKInterfaceModel/FITKAbstractGeoModel.h"

#include "GraphData/GraphDataPick/GraphPickProvider.h"
#include "GraphData/GraphDataPick/GraphDataPickInfo.h"

#include "OperatorsInterface/GraphEventOperator.h"
#include "ModelData/GeometryManager.h"
#include <QFileDialog>
#include <QFileInfo>
#include <QSet>
#include <QMessageBox>

namespace GUI
{
    GUIGeometryGroupDialog::GUIGeometryGroupDialog(Core::FITKActionOperator* oper, Interface::FITKAbsGeoCommand* geoCommand, QWidget *parent) :
        Core::FITKDialog(parent),
        _obj(geoCommand),
        _oper(oper),
        _ui(new Ui::GUIGeometryGroupDialog)
    {
        _ui->setupUi(this);
        // 设置对话框关闭时自动删除
        setAttribute(Qt::WA_DeleteOnClose);
        //去掉问号
        Qt::WindowFlags flags = windowFlags();
        flags &= ~Qt::WindowContextHelpButtonHint;
        setWindowFlags(flags);
        //初始化
        this->init();
    }

    GUIGeometryGroupDialog::~GUIGeometryGroupDialog()
    {
        if (_ui) {
            delete _ui;
            _ui = nullptr;
        }
    }

    void GUIGeometryGroupDialog::init()
    {
        this->initTableWidget();
        if (!_obj) return;
        //设置名称
        _ui->lineEdit_Name->setText(_obj->getDataObjectName());
        //设置数据
        this->setDataToWidget();
    }

    void GUIGeometryGroupDialog::on_pushButton_OK_clicked()
    {
        //清理拾取数据和渲染高亮
        clearGraphHight();
        QHash<int, GUIGeometryFaceGroupSelectWidget*> widgetList;
        QList<GUIGeometryFaceGroupSelectWidget*> newGroupList;
        for (int i = 0; i < _ui->tableWidget->rowCount(); i++) {
            GUIGeometryFaceGroupSelectWidget* widget = dynamic_cast<GUIGeometryFaceGroupSelectWidget*>(_ui->tableWidget->cellWidget(i, 0));
            if (widget == nullptr)continue;
            if (widget->getGroupID() < 0)
                newGroupList.append(widget);
            else
                widgetList.insert(widget->getGroupID(), widget);
        }
        //获取几何分组管理器
        Interface::FITKAbsGeoShapeAgent* shapeAgent = _obj->getShapeAgent();
        if (!shapeAgent) return;
        Interface::FITKGeoComponentManager* componentManager = shapeAgent->getGeoComponentManager();
        if (!componentManager) return;
        //设置旧分组数据
        QList<Interface::FITKGeoComponent*> deleteGroupList;
        QListIterator<Core::FITKAbstractDataObject*> componentIter = componentManager->getIterator();
        while (componentIter.hasNext())
        {
            Interface::FITKGeoComponent* geoComp = dynamic_cast<Interface::FITKGeoComponent*>(componentIter.next());
            if (!geoComp) continue;
            int id = geoComp->getDataObjectID();
            if (!widgetList.contains(id))
            {
                deleteGroupList.append(geoComp);
                continue;
            }
            GUIGeometryFaceGroupSelectWidget* w = widgetList.value(id);
            if (!w) continue;
            QList<int> memberList = w->getPickDataIDs();
            geoComp->setDataObjectName(w->getGroupName());
            geoComp->setMember(memberList);
        }
        //释放不需要的分组
        for (Interface::FITKGeoComponent* group : deleteGroupList)
            componentManager->removeDataObj(group);
        //添加新数据
        for (QList<GUIGeometryFaceGroupSelectWidget*>::iterator iter = newGroupList.begin(); iter != newGroupList.end(); ++iter)
        {
            if (nullptr == *iter)continue;
            QList<int> memberList = (*iter)->getPickDataIDs();
            if (memberList.isEmpty()) continue;
            Interface::FITKGeoComponent* geoCom = new Interface::FITKGeoComponent(Interface::FITKModelEnum::FITKModelSetType::FMSSurface);
            geoCom->setDataObjectName((*iter)->getGroupName());
            geoCom->setMember(memberList);
            componentManager->appendDataObj(geoCom);
        }
        this->accept();
    }

    void GUIGeometryGroupDialog::on_pushButton_Cancel_clicked()
    {
        this->reject();
    }

    void GUIGeometryGroupDialog::on_pushButton_Add_clicked()
    {
        if (_obj == nullptr)return;
        int rowNum = _ui->tableWidget->rowCount();
        _ui->tableWidget->setRowCount(rowNum + 1);
        //获取名称
        QString groupName = this->getNewName();

        GUIGeometryFaceGroupSelectWidget* widget = new GUIGeometryFaceGroupSelectWidget(_ui->tableWidget);
        widget->setGroupData(groupName);
        widget->updateWidget();
        _ui->tableWidget->setCellWidget(rowNum, 0, widget);

        connect(widget, SIGNAL(sigOkClickedFinish()), this, SLOT(slotFaceWidgetOkClicked()));
        connect(widget, SIGNAL(sigCancelClicked()), this, SLOT(slotFaceWidgetCancelClicked()));
        connect(widget, SIGNAL(sigDeleteClicked()), this, SLOT(slotFaceWidgetDeleteClicked()));

        ////更新位置
        //updateFaceWidgetCurrentPos();
    }

    void GUIGeometryGroupDialog::on_pushButton_Clear_clicked()
    {
        if (_obj == nullptr)return;
        //清理tablewidget
        _ui->tableWidget->clear();
        _ui->tableWidget->setRowCount(0);
        //清理拾取数据和渲染高亮
        clearGraphHight();
    }

    void GUIGeometryGroupDialog::on_tableWidget_cellClicked(int row, int column)
    {
        //清理拾取的高亮和数据
        clearGraphHight();
        if (_obj == nullptr)return;
        //获取当前的分组窗口
        GUIGeometryFaceGroupSelectWidget* widget = dynamic_cast<GUIGeometryFaceGroupSelectWidget*>(_ui->tableWidget->cellWidget(row, column));
        if (widget == nullptr)return;
        QList<int> memberList = widget->getPickDataIDs();
        //全部分组窗口的拾取选择按钮关闭
        this->setAllFaceGroupSelectState(false);
        //设置当前的分组窗口拾取选择按钮开启
        widget->setSelectState(true);
        //获取拾取操作器
        Graph::GraphPickProvider* pickD = Graph::GraphPickProvider::getInstance();
        if (pickD == nullptr) return;
        pickD->addDataManually(Interface::FITKModelEnum::FMSSurface, _obj->getDataObjectID(), memberList);
        //拾取模式设置
        Graph::GraphDataPickInfoStru pinfo;
        pinfo._pickObjType = Graph::PickObjType::PickGeoFace;
        pinfo._pickMethod = Graph::PickMethod::PMIndividually;
        Graph::GraphDataPickInfo::SetPickInfo(pinfo);
        //更新渲染
        EventOper::GraphEventOperator* graphOper = FITKOPERREPO->getOperatorT<EventOper::GraphEventOperator>("GraphPreprocess");
        if (graphOper == nullptr)return;
        graphOper->reRender();
    }

    void GUIGeometryGroupDialog::initTableWidget()
    {
        _ui->tableWidget->setRowCount(0);
        _ui->tableWidget->setColumnCount(1);

        _ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        //充满表格
        _ui->tableWidget->horizontalHeader()->setStretchLastSection(true);
        //设置只能单选
        _ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
        //隐藏行表头
        _ui->tableWidget->verticalHeader()->setVisible(false);
        //隐藏列表头
        _ui->tableWidget->horizontalHeader()->setVisible(false);
    }

    void GUIGeometryGroupDialog::setDataToWidget()
    {
        if (!_obj) return;
        //获取几何分组管理器
        Interface::FITKAbsGeoShapeAgent* shapeAgent = _obj->getShapeAgent();
        if (!shapeAgent) return;
        Interface::FITKGeoComponentManager* componentManager = shapeAgent->getGeoComponentManager();
        if (!componentManager) return;
        //遍历获取数据
        QListIterator<Core::FITKAbstractDataObject*> componentIter = componentManager->getIterator();
        while (componentIter.hasNext())
        {
            Interface::FITKGeoComponent* geoComp = dynamic_cast<Interface::FITKGeoComponent*>(componentIter.next());
            if (!geoComp) continue;
            QList<int> ids = geoComp->getMember();
            QString name = geoComp->getDataObjectName();
            //创建分组窗口栏
            GUIGeometryFaceGroupSelectWidget* item = new GUIGeometryFaceGroupSelectWidget(_ui->tableWidget);
            item->setGroupData(name, geoComp->getDataObjectID());
            item->setPickDataIDs(ids);
            item->updateWidget();
            //添加到tableWidget
            int rowNum = _ui->tableWidget->rowCount();
            _ui->tableWidget->setRowCount(rowNum + 1);
            _ui->tableWidget->setCellWidget(rowNum, 0, item);
            //关联信号
            connect(item, SIGNAL(sigOkClickedFinish()), this, SLOT(slotFaceWidgetOkClicked()));
            connect(item, SIGNAL(sigCancelClicked()), this, SLOT(slotFaceWidgetCancelClicked()));
            connect(item, SIGNAL(sigDeleteClicked()), this, SLOT(slotFaceWidgetDeleteClicked()));
        }
    }

    void GUIGeometryGroupDialog::clearGraphHight()
    {
        //清理拾取的高亮和数据
        Graph::GraphPickProvider::getInstance()->clearPickedData();
        //拾取信息设置
        Graph::GraphDataPickInfoStru pinfo;
        pinfo._pickObjType = Graph::PickObjType::PickNone;
        pinfo._pickMethod = Graph::PickMethod::PMNone;
        Graph::GraphDataPickInfo::SetPickInfo(pinfo);
    }

    void GUIGeometryGroupDialog::setAllFaceGroupSelectState(bool state)
    {
        //遍历获取面组设置面组的选择状态
        for (int i = 0; i < _ui->tableWidget->rowCount(); i++) {
            GUIGeometryFaceGroupSelectWidget* widget = dynamic_cast<GUIGeometryFaceGroupSelectWidget*>(_ui->tableWidget->cellWidget(i, 0));
            if (widget == nullptr)continue;
            widget->setSelectState(state);
        }
    }

    QString GUIGeometryGroupDialog::getNewName()
    {
        QString groupName = QString("%1_Group").arg(_obj->getDataObjectName());
        //处理原始名称，获取前缀
        QRegExp regex(QString("%1-\\d+").arg(groupName.toLower()));
        int maxid = 0;
        for (int i = 0; i < _ui->tableWidget->rowCount(); i++) {
            GUIGeometryFaceGroupSelectWidget* widget = dynamic_cast<GUIGeometryFaceGroupSelectWidget*>(_ui->tableWidget->cellWidget(i, 0));
            if (widget == nullptr) continue;
            QString name = widget->getGroupName();
            int pos = regex.lastIndexIn(name.toLower());
            if (pos != 0) continue;
            //获取到最大id
            QString ds = name.mid(groupName.size() + pos + 1, name.size() - 1);
            int d = ds.toInt();
            maxid = d > maxid ? d : maxid;
        }
        return QString("%1-%2").arg(groupName).arg(maxid + 1);
    }

    void GUIGeometryGroupDialog::reject()
    {
        //清理拾取数据和渲染高亮
        clearGraphHight();
        this->QDialog::reject();
    }

    void GUIGeometryGroupDialog::slotFaceWidgetOkClicked()
    {
        //获取当前选择的面组
        int currentRow = _ui->tableWidget->currentRow();
        GUIGeometryFaceGroupSelectWidget* widget = dynamic_cast<GUIGeometryFaceGroupSelectWidget*>(_ui->tableWidget->cellWidget(currentRow, 0));
        if (widget == nullptr) return;
        QSet<int> memberList = widget->getPickDataIDs().toSet();
        //遍历其他面组数据过滤重复面
        for (int i = 0; i < _ui->tableWidget->currentRow(); ++i)
        {
            GUIGeometryFaceGroupSelectWidget* w = dynamic_cast<GUIGeometryFaceGroupSelectWidget*>(_ui->tableWidget->cellWidget(i, 0));
            if (w == nullptr || w == widget) continue;
            QSet<int> oriMemberList = w->getPickDataIDs().toSet();
            QList<int> newMemberList = oriMemberList.subtract(memberList).toList();
            w->setPickDataIDs(newMemberList);
            w->updateWidget();
        }
        //设置当前无选择的面组
        _ui->tableWidget->setCurrentCell(-1, -1);
    }

    void GUIGeometryGroupDialog::slotFaceWidgetCancelClicked()
    {
        //清除高亮
        clearGraphHight();
        //获取当前选择的面组
        int currentRow = _ui->tableWidget->currentRow();
        GUIGeometryFaceGroupSelectWidget* widget = dynamic_cast<GUIGeometryFaceGroupSelectWidget*>(_ui->tableWidget->cellWidget(currentRow, 0));
        if (widget == nullptr) return;
        //设置当前状态
        widget->setSelectState(false);
        //设置当前无选择的面组
        _ui->tableWidget->setCurrentCell(-1, -1);
    }

    void GUIGeometryGroupDialog::slotFaceWidgetDeleteClicked()
    {
        //获取当前选择的面组
        int currentRow = _ui->tableWidget->currentRow();
        GUIGeometryFaceGroupSelectWidget* widget = dynamic_cast<GUIGeometryFaceGroupSelectWidget*>(_ui->tableWidget->cellWidget(currentRow, 0));
        if (widget == nullptr) return;
        //清理当前面组
        _ui->tableWidget->removeRow(currentRow);
        //_ui->tableWidget->setRowCount(0);
        //清除高亮
        clearGraphHight();
        //设置当前无选择的面组
        _ui->tableWidget->setCurrentCell(-1, -1);
    }
}

