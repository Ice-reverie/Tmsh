#include "ControlPanel.h"
#include "ui_ControlPanel.h"
#include "FITK_Component/FITKWidget/FITKTabWidget.h"
#include "GUIWidget/TreeWidgetModel.h"
#include "GUIWidget/TreeWidgetMesh.h"
#include <QSplitter>

namespace GUI
{
    ControlPanel::ControlPanel(QWidget* parent /*= nullptr*/)
        :QDockWidget(parent)
    {
        //创建UI
        _ui = new Ui::ControlPanel;
        _ui->setupUi(this);
        //隐藏关闭窗口
        this->setFeatures(QDockWidget::DockWidgetFloatable | QDockWidget::DockWidgetMovable);

        // 子部件垂直排布
        QSplitter *spliterLayout = new QSplitter(Qt::Vertical, this);
        // 添加tab页
        Comp::FITKTabWidget* tab = new Comp::FITKTabWidget(Comp::FITKTabWidgetType::FITKTab_None, this);
        //模型页
        _pages[PageModel] = new TreeWidgetModel(this);
        tab->addTab(_pages[PageModel], tr("Model"));
        // 网格页
        _pages[PageMesh] = new TreeWidgetMesh(this);
        tab->addTab(_pages[PageMesh], tr("Mesh"));
       
        spliterLayout->addWidget(tab);

        // 设置大小
        spliterLayout->setSizes({ 12000, 6000 });
        this->setWidget(spliterLayout);

    }

    ControlPanel::~ControlPanel()
    {
        if (_ui == nullptr) delete _ui;
    }

    GUI::TreeWidgetPage* ControlPanel::getTreeWidget(TreeWidgetType t)
    {
        //不存在则返回nullptr
        return _pages.value(t);
    }

    void ControlPanel::setGraphWidget(Comp::FITKGraph3DWindowVTK* graphWidget)
    {
        auto meshPage = qobject_cast<TreeWidgetMesh*>(_pages.value(PageMesh));
        if (meshPage)
        {
            meshPage->setGraphWidget(graphWidget);
        }
    }
}


