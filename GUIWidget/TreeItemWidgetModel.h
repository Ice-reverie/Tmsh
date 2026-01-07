/**
 *
 * @file TreeItemWidgetModel.h
 * @brief 树状节点Model的子节点
 * @author wangning (2185896382@qq.com)
 * @date 2025-03-27
 *
 */

#ifndef TREEITEMWIDGETMODEL_H
#define TREEITEMWIDGETMODEL_H

#include <QTreeWidgetItem>

class QLabel;

namespace Interface
{
    class FITKAbsGeoCommand;
}   
namespace GUI
{
    /**
     * @brief 树状节点Model的子节点
     * @author wangning (2185896382@qq.com)
     * @date 2025-03-27
     */
    class TreeItemWidgetModel :public QWidget
    {
        Q_OBJECT

    public:
        /**
         * @brief 构造函数
         * @param name 标签显示的名称
         * @param parent 父窗口
         * @param geo 几何命令对象
         * @author wangning (2185896382@qq.com)
         * @date 2025-03-27
         */
        TreeItemWidgetModel(QString name, Interface::FITKAbsGeoCommand* geo, QTreeWidgetItem* hostItem, QWidget* parent = nullptr);
        /**
         * @brief 析构函数
         * @author wangning (2185896382@qq.com)
         * @date 2025-03-27
         */
        virtual ~TreeItemWidgetModel();
        /**
         * @brief 设置标签名称
         * @param name 标签显示的名称
         * @author wangning (2185896382@qq.com)
         * @date 2025-03-27
         */
        void setText(QString name);

    signals:
        /**
         * @brief 按钮color点击信号
         * @author wangning (2185896382@qq.com)
         * @date 2025-03-27
         */
        void button3Clicked();

    private slots:
        /**
         * @brief geo可见性勾选槽函数
         * @author wangning (2185896382@qq.com)
         * @date 2025-03-27
         */
        void onGeoToggled(bool checked);
        /**
         * @brief 按钮color点击槽函数
         * @author wangning (2185896382@qq.com)
         * @date 2025-03-27
         */
        void onButton3Clicked();

    private:
        /**
        * @brief 显示的标签  
        * @author wangning (2185896382@qq.com)
        * @date 2025-03-27
        */
        QLabel* _label{}; 
        /**
        * @brief 几何命令对象
        * @author wangning (2185896382@qq.com)
        * @date 2025-03-27
        */
        Interface::FITKAbsGeoCommand*  _geo;
        /**
        * @brief 当前挂载的树节点（用于同步子节点颜色按钮）
        */
        QTreeWidgetItem* _hostItem{ nullptr };
        /**
         * @brief 更新子节点颜色按钮（未自定义颜色的节点同步根色）
         */
        void syncChildColorButtons(const QColor& color);
    };
}
#endif // TREEITEMWIDGETMODEL_H
