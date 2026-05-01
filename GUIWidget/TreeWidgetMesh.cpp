#include "TreeWidgetMesh.h"

#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Interface/FITKInterfaceModel/FITKAbstractMesh.h"
#include "ModelData/MeshManager.h"
#include "ModelData/MeshData.h"
#include "ModelData/MeshKernel.h"
#include "GraphData/GraphDataProvider/GraphDataProviderManager.h"
#include "GraphData/GraphDataProvider/GraphDataProviderModel.h"
#include "GraphData/GraphDataAdaptor/GraphObjectBase.h"
#include "FITK_Component/FITKRenderWindowVTK/FITKGraph3DWindowVTK.h"
#include "FITK_Kernel/FITKCore/FITKDataRepo.h"

#include <QMenu>
#include <QHeaderView>
#include <QSignalBlocker>
#include <QCheckBox>
#include <QPushButton>

namespace GUI
{
    TreeWidgetMesh::TreeWidgetMesh(QWidget* parent)
        : TreeWidgetPage(parent)
    {
        this->blockSignals(true);
        this->setColumnCount(2);
        this->header()->hide();
        this->header()->setStretchLastSection(false);
        this->header()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
        this->header()->setSectionResizeMode(1, QHeaderView::Stretch);
        this->initTree();
        this->expandAll();
        this->blockSignals(false);
    }

    void TreeWidgetMesh::updatePage()
    {
        QSignalBlocker blocker(this);
        this->clear();
        this->initTree();
    }

    void TreeWidgetMesh::setGraphWidget(Comp::FITKGraph3DWindowVTK* graphWidget)
    {
        _graphWidget = graphWidget;
    }

    void TreeWidgetMesh::initTree()
    {
        QTreeWidgetItem* meshRoot = this->CreateTreeRootItem(this, tr("Meshes"), (int)ModelTreeItemsType::MTMaterialRoot, "", -1);
        meshRoot->setExpanded(true);

        auto* meshManager = FITKAPP->getGlobalData()->getMeshData<ModelData::MeshManager>();
        if (!meshManager) return;

        QSet<int> processedMeshIds;
        int globalMeshIndex = 0;

        for (int mDataIdx = 0; mDataIdx < meshManager->getDataCount(); ++mDataIdx)
        {
            ModelData::MeshData* meshData = meshManager->getDataByIndex(mDataIdx);
            if (!meshData) continue;

            for (int i = 0; i < meshData->getDataCount(); ++i)
            {
                ModelData::MeshKernel* kernel = meshData->getDataByIndex(i);
                if (!kernel) continue;

                // QString meshName = kernel->getDataObjectName();
                // if (meshName.isEmpty()) meshName = tr("Mesh %1").arg(i + 1);

                int meshId = kernel->getDataObjectID();  //
                if (processedMeshIds.contains(meshId)) {
                continue;
                }
                processedMeshIds.insert(meshId);

                globalMeshIndex++;
                QString meshName = kernel->getDataObjectName();
                if (meshName.isEmpty()) {
                meshName = tr("Mesh %1").arg(globalMeshIndex);
                }   //

                QTreeWidgetItem* meshItem = this->CreateTreeItem(meshRoot, "",
                    (int)GUI::ModelTreeItemsType::MTMaterialRoot, "", -1);
                meshItem->setData(0, Qt::UserRole, kernel->getDataObjectID());
                meshItem->setToolTip(1, tr("Mesh ID: %1").arg(kernel->getDataObjectID()));

                auto* check = new QCheckBox(this);
                check->setChecked(true);
                this->setItemWidget(meshItem, 0, check);
                connect(check, &QCheckBox::toggled, this, [this, meshItem](bool on) {
                    toggleMeshVisible(meshItem, on);
                });

                meshItem->setText(1, meshName);
            }
        }
    }

    void TreeWidgetMesh::toggleMeshVisible(QTreeWidgetItem* item, bool on)
    {
        if (!item) return;
        int meshId = item->data(0, Qt::UserRole).toInt();
        if (meshId <= 0) return;

        auto* graphWidget = _graphWidget;
        if (!graphWidget) return;

        auto* modelProvider = Graph::GraphDataProviderManager::getInstance()->getModelProvider(graphWidget);
        if (!modelProvider) return;

        Graph::GraphObjectBase* obj = modelProvider->getMeshKernelGraph(meshId);
        if (!obj) return;

        obj->setVisible(on);
        graphWidget->reRender();
    }

    void TreeWidgetMesh::deleteMesh(QTreeWidgetItem* item)
    {
        if (!item) return;
        int meshId = item->data(0, Qt::UserRole).toInt();
        if (meshId <= 0) return;

        // Remove from mesh manager/data
        if (auto meshManager = FITKAPP->getGlobalData()->getMeshData<ModelData::MeshManager>())
        {
            for (int mIdx = 0; mIdx < meshManager->getDataCount(); ++mIdx)
            {
                if (auto meshData = meshManager->getDataByIndex(mIdx))
                {
                    meshData->removeDataByID(meshId);
                }
            }
        }

        // Refresh UI
        updatePage();

        // Refresh graph
        if (auto graphWidget = _graphWidget)
        {
            graphWidget->reRender();
        }
    }

    void TreeWidgetMesh::on_itemClicked(QTreeWidgetItem* item, int column)
    {
        Q_UNUSED(item);
        Q_UNUSED(column);
    }

    void TreeWidgetMesh::on_itemStateChanged(QTreeWidgetItem* item, bool state, int column)
    {
        Q_UNUSED(column);
        toggleMeshVisible(item, state);
    }

    void TreeWidgetMesh::on_itemContextMenu(const QList<QTreeWidgetItem*>& items, QMenu* menu)
    {
        if (items.isEmpty()) return;
        auto* item = items.first();
        if (!item) return;
        int meshId = item->data(0, Qt::UserRole).toInt();
        if (meshId <= 0) return;
        QAction* delAct = menu->addAction(tr("Delete Mesh"));
        connect(delAct, &QAction::triggered, this, [this, item]() {
            deleteMesh(item);
        });
    }
}
