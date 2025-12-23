#include "TreeItemWidgetModel.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoCommand.h"
#include "OperatorsInterface/GraphEventOperator.h"
#include "FITK_Kernel/FITKCore/FITKOperatorRepo.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKGeoCommandProp.h"
#include <QWidget>  
#include <QLabel>
#include <QPushButton>
#include <QCheckBox>
#include <QHBoxLayout>
#include <QColorDialog>

namespace GUI
{
    TreeItemWidgetModel::TreeItemWidgetModel(QString name, Interface::FITKAbsGeoCommand* geo, QTreeWidgetItem* hostItem, QWidget* parent)
        :QWidget(parent), _geo(geo), _hostItem(hostItem)
    {
        if (!geo) return;
        // 设置自动删除
        setAttribute(Qt::WA_DeleteOnClose);

        QHBoxLayout* layout = new QHBoxLayout(this);
        QCheckBox* checkGeo = new QCheckBox(this);
        checkGeo->setChecked(_geo->isEnable());
        _label = new QLabel(name, this);
        _label->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        QPushButton* button3 = new QPushButton(this);
        button3->setObjectName("color");
        button3->setFixedSize(18, 18); // 方块色块
        QColor color = _geo->getCommandProp()->getColor();
        button3->setStyleSheet(QString("border: 1px solid #666; background-color: rgb(%1, %2, %3);").arg(color.red()).arg(color.green()).arg(color.blue()));

        layout->addWidget(checkGeo);
        layout->addWidget(_label);
        layout->addWidget(button3);
        layout->setContentsMargins(0,0,6,0);
        layout->setSpacing(4);
        this->setLayout(layout);
        this->setMinimumSize(100, 24);

        connect(checkGeo, &QCheckBox::toggled, this, &TreeItemWidgetModel::onGeoToggled);
        connect(button3, &QPushButton::clicked, this, &TreeItemWidgetModel::onButton3Clicked);
    }

    TreeItemWidgetModel::~TreeItemWidgetModel()
    {
    }

    void TreeItemWidgetModel::setText(QString name)
    {
        _label->setText(name);
    }

    void TreeItemWidgetModel::onGeoToggled(bool checked)
    {
        if (!_geo) return;
        _geo->enable(checked);
        //刷新渲染
        EventOper::GraphEventOperator* operGraph = FITKOPERREPO->getOperatorT<EventOper::GraphEventOperator>("GraphPreprocess");
        operGraph->updateGraph(_geo->getDataObjectID());
    }

    void TreeItemWidgetModel::onButton3Clicked()
    {
        if (!_geo) return;
        //修改之前的颜色  
        QColor oriC= _geo->getCommandProp()->getColor();
        //颜色选择
        QColor color = QColorDialog::getColor(oriC, this, tr("Select Geo Color"));
        if (!color.isValid()) return;
        //修改颜色
        _geo->getCommandProp()->setColor(color);
        this->findChild<QPushButton*> ("color")->setStyleSheet(QString("border: 1px solid #666; background-color: rgb(%1, %2, %3);").arg(color.red()).arg(color.green()).arg(color.blue()));
        //刷新渲染
        if (auto operGraph = FITKOPERREPO->getOperatorT<EventOper::GraphEventOperator>("GraphPreprocess")) {
            // 先更新模型基础色，再刷新子拓扑颜色：graph层会根据颜色缓存决定哪些单元沿用基础色
            operGraph->updateColor(_geo->getDataObjectID(), color, true /*forceUpdate*/);
        }
        // 同步树中未自定义颜色的子节点色块显示
        syncChildColorButtons(color);
    }

    void TreeItemWidgetModel::syncChildColorButtons(const QColor& color)
    {
        if (!_hostItem || !color.isValid()) return;

        constexpr int kCustomColorRole = Qt::UserRole + 5;
        auto updateItemBtn = [&](QTreeWidgetItem* item) {
            if (!item) return;
            // 跳过已自定义颜色的节点
            if (item->data(0, kCustomColorRole).toBool()) return;
            if (auto tree = item->treeWidget()) {
                QWidget* cellWidget = tree->itemWidget(item, 2);
                // colorBtn 可能被包裹在 colorWrap 里，需向下寻找
                QPushButton* btn = qobject_cast<QPushButton*>(cellWidget);
                if (!btn && cellWidget) {
                    btn = cellWidget->findChild<QPushButton*>("colorBtn");
                }
                if (!btn) return;
                btn->setStyleSheet(QString("border: 1px solid #666; background-color: rgb(%1, %2, %3);")
                    .arg(color.red()).arg(color.green()).arg(color.blue()));
            }
        };

        // 遍历当前几何下的子节点（壳/实体等）
        const int childCount = _hostItem->childCount();
        for (int i = 0; i < childCount; ++i) {
            QTreeWidgetItem* child = _hostItem->child(i);
            if (!child) continue;
            // 如果是壳体/实体根，遍历其子项
            for (int j = 0; j < child->childCount(); ++j) {
                updateItemBtn(child->child(j));
            }
            // 也允许根直接承载颜色按钮的情况
            updateItemBtn(child);
        }
    }
}
