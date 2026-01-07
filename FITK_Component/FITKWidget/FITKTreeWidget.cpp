#include "FITKTreeWidget.h"
#include "FITK_Kernel/FITKCore/FITKOperatorRepo.h"
#include "FITK_Kernel/FITKCore/FITKActionOperator.h"
#include "FITK_Kernel/FITKCore/FITKTreeWidgetOperator.h"
#include <QMenu>
#include <QVariant>

namespace Comp
{

    FITKTreeWidget::FITKTreeWidget(QWidget* parent /*= nullptr*/)
        :QTreeWidget(parent)
    {
        //关联类内槽函数
        connect(this, SIGNAL(itemClicked(QTreeWidgetItem*, int)), this, SLOT(slot_itemClicked(QTreeWidgetItem*, int)));
        connect(this, SIGNAL(itemDoubleClicked(QTreeWidgetItem*, int)), this, SLOT(slot_itemDoubleClicked(QTreeWidgetItem*, int)));
        connect(this, SIGNAL(itemChanged(QTreeWidgetItem*, int)), this, SLOT(slot_itemStateChanged(QTreeWidgetItem*, int)));
        //右键菜单
        this->setContextMenuPolicy(Qt::CustomContextMenu);
        connect(this, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(slot_customContextMenuRequested(const QPoint&)));
    }

    QTreeWidgetItem * FITKTreeWidget::getTreeWidgetItemByType(QTreeWidget* parent, int type)
    {
        if (parent == nullptr) {
            return nullptr;
        }

        QTreeWidgetItemIterator item(parent);
        while (*item){
            if ((*item)->type() == type) {
                return (*item);
            }
            item++;
        }
        return nullptr;
    }

    QTreeWidgetItem* FITKTreeWidget::CreateTreeRootItem(QTreeWidget* parent, const QString & text, int type /*= 0*/, const QString& icon /*= ""*/, int state /*= -1*/)
    {
        //创建新的对象
        QTreeWidgetItem* item = new QTreeWidgetItem(parent, type);
        //设置参数
        item->setText(0, text);
        item->setIcon(0, QIcon(icon));
        if (state != -1)
            item->setCheckState(0, (state ? Qt::CheckState::Checked : Qt::CheckState::Unchecked));
        if (parent)
            parent->addTopLevelItem(item);
        return item;
    }

    QTreeWidgetItem* FITKTreeWidget::CreateTreeItem(QTreeWidgetItem* parent, const QString & text, int type /*= 0*/, const QString& icon /*= ""*/, int state /* = -1*/)
    { 
        //创建新的对象
        QTreeWidgetItem* item = new QTreeWidgetItem(parent, type);
        //设置参数
        item->setText(0, text);
        if(!icon.isEmpty())
            item->setIcon(0, QIcon(icon));
        if (state != -1)
            item->setCheckState(0, (state ? Qt::CheckState::Checked : Qt::CheckState::Unchecked));
        if(parent)
            parent->addChild(item);
        return item;
    }

    Core::FITKAbstractOperator* FITKTreeWidget::GetOperator(const QString& operName)
    {
        //从操作器仓库获取
        return Core::FITKOperatorRepo::getInstance()->getOperator(operName);
    }

    Core::FITKTreeWidgetOperator* FITKTreeWidget::getOperator()
    {
        return nullptr;
    }

    void FITKTreeWidget::on_itemClicked(QTreeWidgetItem *item, int column)
    {
        //虚函数
        Core::FITKTreeWidgetOperator* oper = this->getOperator();
        if (oper == nullptr) return;
        oper->on_itemClicked(this, item, column);
    }

    void FITKTreeWidget::on_itemDoubleClicked(QTreeWidgetItem *item, int column)
    {
        Core::FITKTreeWidgetOperator* oper = this->getOperator();
        if (oper == nullptr) return;
        oper->on_itemDoubleClicked(this, item, column);
        //虚函数
    }

    void FITKTreeWidget::on_itemStateChanged(QTreeWidgetItem *item, bool state, int column)
    {
        //虚函数
    }

    void FITKTreeWidget::on_itemContextMenu(const QList<QTreeWidgetItem*>& item, QMenu* menu)
    {
        //虚函数
    }

    void FITKTreeWidget::slot_itemClicked(QTreeWidgetItem *item, int column)
    {
        //虚函数实现
        this->on_itemClicked(item, column);
    }

    void FITKTreeWidget::slot_itemDoubleClicked(QTreeWidgetItem *item, int column)
    {
        //虚函数实现
        this->on_itemDoubleClicked(item, column);
    }

    void FITKTreeWidget::slot_itemStateChanged(QTreeWidgetItem *item, int column)
    {
        if (item == nullptr) {
            return;
        }

        // QTreeWidget::itemChanged 会在任意数据变化时触发（setData/setText 等），不一定是勾选框变化。
        // 仅当 item 本身支持 Qt 内置勾选框时，才转发到 on_itemStateChanged。
        if ((item->flags() & Qt::ItemIsUserCheckable) == 0) {
            return;
        }

        bool state = (item->checkState(column) == Qt::CheckState::Checked);
        this->on_itemStateChanged(item, state, column);
    }

    void FITKTreeWidget::slot_customContextMenuRequested(const QPoint& p)
    {
        Q_UNUSED(p);
        
        QList<QTreeWidgetItem*> itemList = this->selectedItems();
        if (itemList.isEmpty()|| nullptr == itemList.first()) return;
        //创建菜单
        QMenu menu(this);
        //添加action
        this->on_itemContextMenu(itemList, &menu);
        //关联全部Action槽函数
        QList<QAction*> acts = menu.findChildren<QAction*>();
        for (QAction* act :acts)
        {
            connect(act, SIGNAL(triggered()), this, SLOT(slot_treeActionTriggered()));
        }
        //显示菜单
        menu.exec(QCursor::pos());
    }

    void FITKTreeWidget::slot_treeActionTriggered()
    {
        QObject* sender = this->sender();
        if (sender == nullptr) return;
        //操作器仓库获取
        QString name = sender->objectName();
        if (name.isEmpty()) return;
        Core::FITKActionOperator* oper =
            Core::FITKOperatorRepo::getInstance()->getOperatorT<Core::FITKActionOperator>(name);
        if (oper == nullptr) return;
        QList<QTreeWidgetItem*> items = this->selectedItems();
        oper->setEmitter(sender);
        oper->setArgs("SelectedItems",QVariant::fromValue((void*)(&items)));
        oper->actionTriggered();
    }

}
 
