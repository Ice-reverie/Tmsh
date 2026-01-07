#include "TreeWidgetModel.h"
#include "TreeWidgetModelHelper.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKGeoCommandList.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoShapeAgent.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsVirtualTopo.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKVirtualTopoManager.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Kernel/FITKCore/FITKDataRepo.h"
#include "FITK_Kernel/FITKCore/FITKOperatorRepo.h"
#include "ModelData/GeometryManager.h"
#include "OperatorsInterface/GraphEventOperator.h"
#include "TreeItemWidgetModel.h"
#include <QMenu>
#include <QHeaderView>
#include <QSignalBlocker>

namespace GUI
{
    TreeWidgetModel::TreeWidgetModel(QWidget * parent)
        :TreeWidgetPage(parent)
    {
        this->blockSignals(true);
        this->setColumnCount(3);
        this->header()->setStretchLastSection(false);
        // 让名称所在列自适应填满，避免颜色方块覆盖文字
        this->header()->setSectionResizeMode(0, QHeaderView::Stretch);
        this->header()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
        this->header()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
        this->setIndentation(10);      // tighter tree indent to shrink gap before names
        this->initTree();
        this->expandAll();
        this->blockSignals(false);
        this->header()->hide();
    }

    TreeWidgetModel::~TreeWidgetModel()
    {
    }

    TreeWidgetType TreeWidgetModel::getTreeWidgetType()
    {
        return PageModel;
    }

    void TreeWidgetModel::updatePage()
    {
        QSignalBlocker blocker(this);
        QTreeWidget::clear();
        this->initTree();
    }

    void TreeWidgetModel::initTree()
    {
        //几何树根节点
        QTreeWidgetItem* geometryTreeRoot = this->CreateTreeRootItem(this, tr("Model"), (int)ModelTreeItemsType::MTModelRoot, "", -1);

        ModelData::GeometryManager* geometryMgr = FITKAPP->getGlobalData()->
            getGeometryData<ModelData::GeometryManager>();
        if (geometryMgr == nullptr) return;
        Interface::FITKGeoCommandList* geometryData = dynamic_cast<Interface::FITKGeoCommandList*>( geometryMgr);
        if (geometryData == nullptr) return;

        int num = geometryData->getDataCount();
  
        for (int i = 0; i < num; ++i)
        {       
            Interface::FITKAbsGeoCommand* geoCmd = geometryData->getDataByIndex(i);
            if (!geoCmd)continue;
            //几何的子节点
            QString name = geoCmd->getDataObjectName();
            int id = geometryData->getIDByIndex(i);

            QTreeWidgetItem* geometry = this->CreateTreeItem(geometryTreeRoot, "", (int)ModelTreeItemsType::MTModel, "");
            geometry->setData(0, Qt::UserRole, id);
            TreeItemWidgetModel* itemWidget = new TreeItemWidgetModel(name, geoCmd, geometry, this);

            this->setItemWidget(geometry, 0, itemWidget);
            _modelAll.append(geometry);

            TreeWidgetModelHelper::appendSolidItems(this, geometry, geoCmd);
            TreeWidgetModelHelper::appendShellItems(this, geometry, geoCmd);
        }
        geometryTreeRoot->setExpanded(true);

        //材料根节点
    }

    void TreeWidgetModel::on_itemClicked(QTreeWidgetItem* item, int column)
    {
        return;
    }

    void TreeWidgetModel::on_itemStateChanged(QTreeWidgetItem* item, bool state, int column)
    {
        Q_UNUSED(column);
        if (item == nullptr) return;

        ModelTreeItemsType itemType = (ModelTreeItemsType)item->type();
        if (itemType != ModelTreeItemsType::MTSolid && itemType != ModelTreeItemsType::MTShell) return;

        handleTopoCheckChanged(item, state);
    }

    void TreeWidgetModel::on_itemContextMenu(const QList<QTreeWidgetItem*>& items, QMenu* menu)
    {
        if (items.isEmpty()) return;
        QTreeWidgetItem* item = items.at(0);
        if (item == nullptr) return;

        ModelTreeItemsType itemType = (ModelTreeItemsType)item->type();

        switch (itemType)
        {
        case MTModelRoot:
        {
            QAction* act = menu->addAction(tr("Create Model"));
            act->setObjectName("actionTreeCreateModel");
            break;
        }
        case  MTModel:
        {
            QAction* act = menu->addAction(QObject::tr("Group"));
            act->setObjectName("actionTreeGroupModel");
            act = menu->addAction(tr("Remove"));
            act->setObjectName("actionTreeRemoveModel");
            act = menu->addAction(QObject::tr("Rename"));
            act->setObjectName("actionTreeRenameModel");
            break;
        }
        case MTSolid:
        case MTShell:
        {
            break;
        }
        case  MTMaterialRoot:
        {
            break;
        }

        default:
            break;

        }
    }

    void TreeWidgetModel::handleTopoCheckChanged(QTreeWidgetItem* item, bool state)
    {
        if (item == nullptr) return;
        int topoId = item->data(0, Qt::UserRole).toInt();
        int geoId = item->data(0, Qt::UserRole + 1).toInt();
        if (topoId <= 0 || geoId <= 0) return;

        // 找到当前几何命令对应的虚拓扑，再设置可见性
        if (auto geoCmd = Core::FITKDataRepo::getInstance()->getTDataByID<Interface::FITKAbsGeoCommand>(geoId))
        {
            if (auto shapeAgent = geoCmd->getShapeAgent())
            {
                if (auto vtMgr = shapeAgent->getVirtualTopoManager())
                {
                    if (auto solidTopo = vtMgr->findTopo(topoId))
                    {
                        // 同步实体及其子拓扑（面/边/点）可见性，确保渲染响应
                        TreeWidgetModelHelper::setTopoEnableRecursive(solidTopo, state);
                        TreeWidgetModelHelper::setSubToposEnable(solidTopo, state);
                    }

                    // 如果全部子实体/壳体被隐藏，顺便把整体命令隐藏；有任意开启则打开命令
                    auto hasVisibleTopo = [](Interface::FITKShapeVirtualTopoManager* mgr) {
                        if (!mgr) return false;
                        for (int i = 0; i < mgr->getDataCount(); ++i)
                        {
                            Interface::FITKAbsVirtualTopo* topo = mgr->getDataByIndex(i);
                            if (topo && topo->isEnable()) return true;
                        }
                        return false;
                    };
                    bool hasVisible =
                        hasVisibleTopo(vtMgr->getShapeVirtualTopoManager(Interface::FITKGeoEnum::VTopoShapeType::VSSolid)) ||
                        hasVisibleTopo(vtMgr->getShapeVirtualTopoManager(Interface::FITKGeoEnum::VTopoShapeType::VSShell));
                    // 全部隐藏时，兜底将所有虚拓扑禁用，避免残留面/边
                    if (!hasVisible)
                    {
                        TreeWidgetModelHelper::setAllToposEnable(vtMgr, false);
                    }
                    geoCmd->enable(hasVisible);
                }
            }
        }

        if (auto graphOper = FITKOPERREPO->getOperatorT<EventOper::GraphEventOperator>("GraphPreprocess"))
        {
            // 使用强制刷新，确保渲染数据同步（updateGraph 不会自动 fitView）
            graphOper->updateGraph(geoId, true);
        }
    }

}
