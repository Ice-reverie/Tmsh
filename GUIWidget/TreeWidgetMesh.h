#ifndef TREEWIDGETMESH_H
#define TREEWIDGETMESH_H

#include "GUIEnumType.h"

class QMenu;
class QTreeWidgetItem;

namespace Comp
{
    class FITKGraph3DWindowVTK;
}

namespace GUI
{
    /**
     * @brief 网格管理树页：显示/隐藏/删除网格
     */
    class GUIWidgetAPI TreeWidgetMesh : public TreeWidgetPage
    {
        Q_OBJECT
    public:
        explicit TreeWidgetMesh(QWidget* parent = nullptr);
        ~TreeWidgetMesh() override = default;

        TreeWidgetType getTreeWidgetType() override { return PageMesh; }
        void updatePage() override;
        /**
         * @brief 注入三维渲染窗口，避免在此模块直接依赖GUIFrame
         */
        void setGraphWidget(Comp::FITKGraph3DWindowVTK* graphWidget);

    protected:
        void initTree();
        void on_itemClicked(QTreeWidgetItem* item, int column) override;
        void on_itemStateChanged(QTreeWidgetItem* item, bool state, int column) override;
        void on_itemContextMenu(const QList<QTreeWidgetItem*>& items, QMenu* menu) override;

    private:
        void toggleMeshVisible(QTreeWidgetItem* item, bool on);
        void deleteMesh(QTreeWidgetItem* item);

    private:
        Comp::FITKGraph3DWindowVTK* _graphWidget{ nullptr };
    };
}

#endif // TREEWIDGETMESH_H
