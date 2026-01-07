/**
 *
 * @file TreeWidgetModel.h
 * @brief 模型页面的树
 * @author wangning (2185896382@qq.com)
 * @date 2025-03-26
 *
 */
#ifndef _MODEL_WIDGET_TREE_H___
#define _MODEL_WIDGET_TREE_H___

#include "GUIEnumType.h"

class QMenu;
class QTreeWidgetItem;

namespace GUI
{
    class TreeItemWidgetModel;

    /**
     * @brief 模型树页面
     * @author wangning (2185896382@qq.com)
     * @date 2025-03-26
     */
    class GUIWidgetAPI TreeWidgetModel : public TreeWidgetPage
    {
        Q_OBJECT
    public:
        /**
         * @brief Create
         * @author wangning (2185896382@qq.com)
         * @date 2025-03-26
         */
        explicit TreeWidgetModel(QWidget* parent = nullptr);

        /**
         * @brief Destory
         * @author wangning (2185896382@qq.com)
         * @date 2025-03-26
         */
        virtual ~TreeWidgetModel();

        /**
        * @brief 获取类型
        * @return TreeWidgetType
        * @author wangning (2185896382@qq.com)
        * @date 2025-03-26
        */
        TreeWidgetType getTreeWidgetType() override;

        /**
         * @brief 刷新模型页
         * @author wangning (2185896382@qq.com)
         * @date 2025-03-26
         */
        void updatePage() override;

        /**
         * @brief 处理实体/壳体勾选状态变更（供外部控件回调）
         */
        void handleTopoCheckChanged(QTreeWidgetItem* item, bool state);

    protected:
        /**
         * @brief 初始化树
         * @author wangning (2185896382@qq.com)
         * @date 2025-03-26
         */
        void initTree();
        /**
        * @brief 树形菜单节点点击事件，子类重写实现
        * @param[i]  item           点击对象
        * @param[i]  column         列数
        * @author wangning (2185896382@qq.com)
        * @date 2025-03-26
        */
        virtual void on_itemClicked(QTreeWidgetItem* item, int column) override;
        /**
        * @brief 节点勾选状态变更
        * @param item 节点
        * @param state 是否勾选
        * @param column 列
        * @author wangning (2185896382@qq.com)
        * @date 2025-12-08
        */
        virtual void on_itemStateChanged(QTreeWidgetItem* item, bool state, int column) override;
        /**
        * @brief 显示右键菜单
        * @param item 右键的item
        * @param menu 显示的menu
        * @author wangning (2185896382@qq.com)
        * @date 2025-03-26
        */
        virtual void on_itemContextMenu(const QList<QTreeWidgetItem*>& items, QMenu* menu) override;

    private:

        /**
         * @brief 模型数据
         * @author wangning (2185896382@qq.com)
         * @date 2025-03-26
         */
        QList<QTreeWidgetItem*> _modelAll{};
     
    };
}


#endif
