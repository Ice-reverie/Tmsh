#include "TreeWidgetModelHelper.h"

#include "TreeWidgetModel.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoCommand.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoShapeAgent.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsVirtualTopo.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsVirtualTopoMapper.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKGeoCommandProp.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKVirtualTopoManager.h"
#include "FITK_Kernel/FITKCore/FITKDataRepo.h"
#include "FITK_Kernel/FITKCore/FITKOperatorRepo.h"
#include "OperatorsInterface/GraphEventOperator.h"
#include <QCheckBox>
#include <QColorDialog>
#include <QHBoxLayout>
#include <QPushButton>
#include <QVariant>
#include <QWidget>

using Interface::FITKAbsGeoCommand;
using Interface::FITKAbsVirtualTopo;
using Interface::FITKVirtualTopoManager;

namespace GUI
{
    void TreeWidgetModelHelper::setTopoEnableRecursive(FITKAbsVirtualTopo* topo, bool enableState)
    {
        if (!topo) return;
        topo->enable(enableState);
        for (int i = 0; i < topo->getSubTopoCount(); ++i)
        {
            setTopoEnableRecursive(topo->getSubTopo(i), enableState);
        }
    }

    void TreeWidgetModelHelper::setAllToposEnable(FITKVirtualTopoManager* vtMgr, bool enableState)
    {
        if (!vtMgr) return;
        QList<Interface::FITKGeoEnum::VTopoShapeType> types = vtMgr->getTopoTypes();
        for (auto t : types)
        {
            if (auto mgr = vtMgr->getShapeVirtualTopoManager(t))
            {
                for (int i = 0; i < mgr->getDataCount(); ++i)
                {
                    setTopoEnableRecursive(mgr->getDataByIndex(i), enableState);
                }
            }
        }
        for (int i = 0; i < vtMgr->getNumberOfRootObjs(); ++i)
        {
            setTopoEnableRecursive(vtMgr->getRootObj(i), enableState);
        }
    }

    void TreeWidgetModelHelper::setSubToposEnable(FITKAbsVirtualTopo* topo, bool enableState)
    {
        if (!topo) return;

        Interface::FITKVirtualTopoMapper mapper;

        mapper.mapTopo(topo, Interface::FITKGeoEnum::VTopoShapeType::VSFace, false);
        for (int i = 0; i < mapper.length(); ++i)
        {
            setTopoEnableRecursive(mapper.virtualTopo(i), enableState);
        }

        mapper.mapTopo(topo, Interface::FITKGeoEnum::VTopoShapeType::VSWire, false);
        for (int i = 0; i < mapper.length(); ++i)
        {
            setTopoEnableRecursive(mapper.virtualTopo(i), enableState);
        }

        mapper.mapTopo(topo, Interface::FITKGeoEnum::VTopoShapeType::VSEdge, false);
        for (int i = 0; i < mapper.length(); ++i)
        {
            setTopoEnableRecursive(mapper.virtualTopo(i), enableState);
        }

        mapper.mapTopo(topo, Interface::FITKGeoEnum::VTopoShapeType::VSPoint, false);
        for (int i = 0; i < mapper.length(); ++i)
        {
            setTopoEnableRecursive(mapper.virtualTopo(i), enableState);
        }
    }

    bool TreeWidgetModelHelper::hasSolidAncestor(FITKAbsVirtualTopo* topo)
    {
        if (!topo) return false;
        for (int i = 0; i < topo->getParentTopoCount(); ++i)
        {
            FITKAbsVirtualTopo* parent = topo->getParentTopo(i);
            if (!parent) continue;
            if (parent->getShapeType() == Interface::FITKGeoEnum::VTopoShapeType::VSSolid)
            {
                return true;
            }
            if (hasSolidAncestor(parent)) return true;
        }
        return false;
    }

    void TreeWidgetModelHelper::appendSolidItems(TreeWidgetModel* treeWidget, QTreeWidgetItem* geoItem, FITKAbsGeoCommand* geoCmd)
    {
        if (treeWidget == nullptr || geoItem == nullptr || geoCmd == nullptr) return;

        Interface::FITKAbsGeoShapeAgent* shapeAgent = geoCmd->getShapeAgent();
        if (!shapeAgent) return;

        FITKVirtualTopoManager* vtMgr = shapeAgent->getVirtualTopoManager();
        if (!vtMgr) return;

        Interface::FITKShapeVirtualTopoManager* solidMgr = vtMgr->getShapeVirtualTopoManager(Interface::FITKGeoEnum::VTopoShapeType::VSSolid);
        if (!solidMgr || solidMgr->getDataCount() <= 0) return;

        QTreeWidgetItem* solidRoot = Comp::FITKTreeWidget::CreateTreeItem(geoItem, treeWidget->tr("Solids"),
            (int)GUI::ModelTreeItemsType::MTSolidRoot, "", -1);
        solidRoot->setExpanded(true);

        for (int i = 0; i < solidMgr->getDataCount(); ++i)
        {
            FITKAbsVirtualTopo* solidTopo = solidMgr->getDataByIndex(i);
            if (!solidTopo) continue;

            int solidId = solidTopo->getDataObjectID();
            int solidIndex = solidTopo->getIndexLabel();
            if (solidIndex < 0) solidIndex = i;

            QString solidName = treeWidget->tr("Solid %1").arg(solidIndex + 1);

            QTreeWidgetItem* solidItem = Comp::FITKTreeWidget::CreateTreeItem(solidRoot, "",
                (int)GUI::ModelTreeItemsType::MTSolid, "", -1);
            solidItem->setData(0, Qt::UserRole, solidId);
            solidItem->setData(0, Qt::UserRole + 1, geoCmd->getDataObjectID());
            solidItem->setToolTip(1, treeWidget->tr("Solid ID: %1").arg(solidId));
            solidItem->setFlags(solidItem->flags() & ~Qt::ItemIsUserCheckable);

            QColor defaultColor;
            if (geoCmd->getCommandProp()) {
                defaultColor = geoCmd->getCommandProp()->getColor();
            }
            solidItem->setData(0, kCustomColorRole, false);

            auto* check = new QCheckBox(treeWidget);
            check->setChecked(solidTopo->isEnable());
            treeWidget->setItemWidget(solidItem, 0, check);
            QObject::connect(check, &QCheckBox::toggled, treeWidget, [treeWidget, solidItem](bool on) {
                treeWidget->handleTopoCheckChanged(solidItem, on);
            });

            solidItem->setText(1, solidName);

            auto* colorBtn = new QPushButton(treeWidget);
            colorBtn->setFixedSize(16, 16);
            colorBtn->setObjectName("colorBtn");
            colorBtn->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
            auto setBtnColor = [](QPushButton* btn, const QColor& c) {
                if (!btn || !c.isValid()) return;
                btn->setStyleSheet(QString("border: 1px solid #666; background-color: rgb(%1,%2,%3);")
                    .arg(c.red()).arg(c.green()).arg(c.blue()));
            };
            setBtnColor(colorBtn, defaultColor);
            auto* colorWrap = new QWidget(treeWidget);
            auto* colorLayout = new QHBoxLayout(colorWrap);
            colorLayout->setContentsMargins(6, 0, 0, 0);
            colorLayout->setSpacing(0);
            colorLayout->addWidget(colorBtn, 0, Qt::AlignLeft);
            treeWidget->setItemWidget(solidItem, 2, colorWrap);
            QObject::connect(colorBtn, &QPushButton::clicked, treeWidget, [treeWidget, solidItem, colorBtn]() {
                int topoId = solidItem->data(0, Qt::UserRole).toInt();
                int geoId = solidItem->data(0, Qt::UserRole + 1).toInt();
                if (topoId <= 0 || geoId <= 0) return;
                QColor defaultColor;
                if (auto geoCmd = Core::FITKDataRepo::getInstance()->getTDataByID<FITKAbsGeoCommand>(geoId)) {
                    if (geoCmd->getCommandProp()) defaultColor = geoCmd->getCommandProp()->getColor();
                }
                QColor color = QColorDialog::getColor(defaultColor, treeWidget, treeWidget->tr("Select Color"));
                if (!color.isValid()) return;
                if (auto graphOper = FITKOPERREPO->getOperatorT<EventOper::GraphEventOperator>("GraphPreprocess")) {
                    if (auto geoCmd = Core::FITKDataRepo::getInstance()->getTDataByID<FITKAbsGeoCommand>(geoId)) {
                        if (auto shapeAgent = geoCmd->getShapeAgent()) {
                            if (auto vtMgr = shapeAgent->getVirtualTopoManager()) {
                                if (auto topo = vtMgr->findTopo(topoId)) {
                                    setTopoEnableRecursive(topo, true);
                                    setSubToposEnable(topo, true);
                                }
                            }
                        }
                    }
                    graphOper->updateGraph(geoId, true);
                    graphOper->updateColor(geoId, color, QVariant(topoId));
                }
                if (colorBtn) {
                    colorBtn->setStyleSheet(QString("border: 1px solid #666; background-color: rgb(%1,%2,%3);")
                        .arg(color.red()).arg(color.green()).arg(color.blue()));
                }
                solidItem->setData(0, kCustomColorRole, true);
            });
        }
    }

    void TreeWidgetModelHelper::appendShellItems(TreeWidgetModel* treeWidget, QTreeWidgetItem* geoItem, FITKAbsGeoCommand* geoCmd)
    {
        if (treeWidget == nullptr || geoItem == nullptr || geoCmd == nullptr) return;

        Interface::FITKAbsGeoShapeAgent* shapeAgent = geoCmd->getShapeAgent();
        if (!shapeAgent) return;

        FITKVirtualTopoManager* vtMgr = shapeAgent->getVirtualTopoManager();
        if (!vtMgr) return;

        Interface::FITKShapeVirtualTopoManager* shellMgr = vtMgr->getShapeVirtualTopoManager(Interface::FITKGeoEnum::VTopoShapeType::VSShell);
        if (!shellMgr || shellMgr->getDataCount() <= 0) return;

        QTreeWidgetItem* shellRoot = Comp::FITKTreeWidget::CreateTreeItem(geoItem, treeWidget->tr("Shells"),
            (int)GUI::ModelTreeItemsType::MTShellRoot, "", -1);
        shellRoot->setExpanded(true);

        int shellCount = 0;
        for (int i = 0; i < shellMgr->getDataCount(); ++i)
        {
            FITKAbsVirtualTopo* shellTopo = shellMgr->getDataByIndex(i);
            if (!shellTopo) continue;

            if (hasSolidAncestor(shellTopo)) continue;

            int shellId = shellTopo->getDataObjectID();
            int shellIndex = shellTopo->getIndexLabel();
            if (shellIndex < 0) shellIndex = i;

            QString shellName = treeWidget->tr("Shell %1").arg(shellIndex + 1);

            QTreeWidgetItem* shellItem = Comp::FITKTreeWidget::CreateTreeItem(shellRoot, "",
                (int)GUI::ModelTreeItemsType::MTShell, "", -1);
            shellItem->setData(0, Qt::UserRole, shellId);
            shellItem->setData(0, Qt::UserRole + 1, geoCmd->getDataObjectID());
            shellItem->setToolTip(1, treeWidget->tr("Shell ID: %1").arg(shellId));
            shellItem->setFlags(shellItem->flags() & ~Qt::ItemIsUserCheckable);
            ++shellCount;

            QColor defaultColor;
            if (geoCmd->getCommandProp()) {
                defaultColor = geoCmd->getCommandProp()->getColor();
            }

            auto* check = new QCheckBox(treeWidget);
            check->setChecked(shellTopo->isEnable());
            treeWidget->setItemWidget(shellItem, 0, check);
            QObject::connect(check, &QCheckBox::toggled, treeWidget, [treeWidget, shellItem](bool on) {
                treeWidget->handleTopoCheckChanged(shellItem, on);
            });

            shellItem->setText(1, shellName);

            auto* colorBtn = new QPushButton(treeWidget);
            colorBtn->setFixedSize(16, 16);
            colorBtn->setObjectName("colorBtn");
            colorBtn->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
            auto setBtnColor = [](QPushButton* btn, const QColor& c) {
                if (!btn || !c.isValid()) return;
                btn->setStyleSheet(QString("border: 1px solid #666; background-color: rgb(%1,%2,%3);")
                    .arg(c.red()).arg(c.green()).arg(c.blue()));
            };
            setBtnColor(colorBtn, defaultColor);
            auto* colorWrap = new QWidget(treeWidget);
            auto* colorLayout = new QHBoxLayout(colorWrap);
            colorLayout->setContentsMargins(6, 0, 0, 0);
            colorLayout->setSpacing(0);
            colorLayout->addWidget(colorBtn, 0, Qt::AlignLeft);
            treeWidget->setItemWidget(shellItem, 2, colorWrap);
            QObject::connect(colorBtn, &QPushButton::clicked, treeWidget, [treeWidget, shellItem, colorBtn]() {
                int topoId = shellItem->data(0, Qt::UserRole).toInt();
                int geoId = shellItem->data(0, Qt::UserRole + 1).toInt();
                if (topoId <= 0 || geoId <= 0) return;
                QColor defaultColor;
                if (auto geoCmd = Core::FITKDataRepo::getInstance()->getTDataByID<FITKAbsGeoCommand>(geoId)) {
                    if (geoCmd->getCommandProp()) defaultColor = geoCmd->getCommandProp()->getColor();
                }
                QColor color = QColorDialog::getColor(defaultColor, treeWidget, treeWidget->tr("Select Color"));
                if (!color.isValid()) return;
                if (auto graphOper = FITKOPERREPO->getOperatorT<EventOper::GraphEventOperator>("GraphPreprocess")) {
                    if (auto geoCmd = Core::FITKDataRepo::getInstance()->getTDataByID<FITKAbsGeoCommand>(geoId)) {
                        if (auto shapeAgent = geoCmd->getShapeAgent()) {
                            if (auto vtMgr = shapeAgent->getVirtualTopoManager()) {
                                if (auto topo = vtMgr->findTopo(topoId)) {
                                    setTopoEnableRecursive(topo, true);
                                    setSubToposEnable(topo, true);
                                }
                            }
                        }
                    }
                    graphOper->updateGraph(geoId, true);
                    graphOper->updateColor(geoId, color, QVariant(topoId));
                }
                if (colorBtn) {
                    colorBtn->setStyleSheet(QString("border: 1px solid #666; background-color: rgb(%1,%2,%3);")
                        .arg(color.red()).arg(color.green()).arg(color.blue()));
                }
                shellItem->setData(0, kCustomColorRole, true);
            });
        }

        if (shellCount == 0)
        {
            shellRoot->setHidden(true);
        }
    }
}
