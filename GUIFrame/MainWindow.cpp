#include "MainWindow.h"

#include "ControlPanel.h"
#include "ConsoleWidget.h"
#include "RenderWidget.h"
#include "ActionEventHandler.h"
#include "FITK_Component/FITKRenderWindowVTK/FITKGraph3DWindowVTK.h"

#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include <SARibbonBar.h>
#include <SARibbonApplicationButton.h>
#include <SARibbonQuickAccessBar.h>
#include <SARibbonTabBar.h>

#include <QString>
#include <QMenu>
#include <QDebug>
#include <QToolBar>
#include <QHash>
#include <QApplication>
#include <QCoreApplication>
#include <QPainter>
#include <QColor>
#include <QSplitter>
#include <QGridLayout>
#include <QStatusBar>
#include <QLabel>

#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QDesktopServices>
#include <QUrl>
#include <QDir>


namespace GUI
{
    MainWindow::MainWindow(QWidget *parent) : SARibbonMainWindow(parent)
    {

        _ribbonBar = this->ribbonBar();

        m_ActionHandler = new ActionEventHandler;
        //使用Office2013风格
        sa_set_ribbon_theme(_ribbonBar, SARibbonTheme::RibbonThemeOffice2013);

        //设置顶部线条颜色
        _ribbonBar->setTabBarBaseLineColor(QColor(186, 201, 219));

        _ribbonBar->setRibbonStyle(SARibbonBar::RibbonStyleLooseThreeRow);
        _ribbonBar->setFont(QFont("Arial", 9));

        setWindowTitle("FastCAE-Mesh");
        _ribbonBar->setWindowTitleTextColor(Qt::black);

        init();

        _ribbonBar->setCurrentIndex(0);
    }

    MainWindow::~MainWindow()
    {
        if (m_ActionHandler) delete m_ActionHandler;
        if (_controlPanel) delete _controlPanel;
    }  

    MainWindow* MainWindow::GetMainWindowFromFramework()
    {
        //访问全局数据
        AppFrame::FITKGlobalData* data = FITKAPP->getGlobalData();
        if (data == nullptr) return nullptr;
        QWidget* w = data->getMainWindow();
        //强制类型转换
        return dynamic_cast<MainWindow*>(w);
    }

    ActionEventHandler * MainWindow::getActionEventHandle() const
    {
        return m_ActionHandler;
    }


    QAction* MainWindow::createAction(const QString &text, const QString &objectName,
        const QIcon& icon/*= QIcon()*/, const QString& tips /*= ""*/)
    {
        // 实例化一个action
        QAction* tempAction = new QAction(this);
        tempAction->setText(text);
        // 设置action的ObjectName
        tempAction->setObjectName(objectName);
        // 设置action的ToolTip
        tempAction->setToolTip(tips);
        // 设置action的图标
        tempAction->setIcon(icon);

        this->connectActionEventHandler(tempAction);

        return tempAction;
    }

    void MainWindow::connectActionEventHandler(QAction* act, bool c /*= true*/)
    {
        if (act == nullptr || m_ActionHandler == nullptr) return;
        if (c)
            connect(act, SIGNAL(triggered()), m_ActionHandler, SLOT(execOperator()));
        else
            disconnect(act, SIGNAL(triggered()), m_ActionHandler, SLOT(execOperator()));
    }

    void MainWindow::init()
    {

        initCentralWidget();

        initApplicationButton();
        initHome();
        initHelp();
    }

    void MainWindow::initCentralWidget()
    {

        _controlPanel = new ControlPanel(this);
        this->addDockWidget(Qt::LeftDockWidgetArea, _controlPanel);

        QWidget* centralWidget = new QWidget(this);
        _consoleWidget = new ConsoleWidget(this);
        _renderWidget = new RenderWidget(this);
        _controlPanel->setGraphWidget(dynamic_cast<Comp::FITKGraph3DWindowVTK*>(_renderWidget->getRenderWin()));

        QSplitter* verLayout = new QSplitter(Qt::Vertical);
        verLayout->setMouseTracking(true);
        verLayout->setHandleWidth(5);
        verLayout->addWidget(_renderWidget);
        verLayout->addWidget(_consoleWidget);
        verLayout->setSizes(QList<int>{20000, 5000});


        auto mainLayout = new QGridLayout();
        mainLayout->setObjectName("CentralGridLayout");
        mainLayout->setContentsMargins(5, 5, 5, 10);
        mainLayout->addWidget(verLayout);
        centralWidget->setLayout(mainLayout);

        setCentralWidget(centralWidget);
    }

    void MainWindow::initApplicationButton()
    {
        //文件部分添加
        QAbstractButton* fileAppButton = _ribbonBar->applicationButton();
        fileAppButton->setText(tr("File"));
        fileAppButton->setFixedWidth(60);

        QMenu* menu = nullptr;
        QAction* action = nullptr;

        //导入几何文件
        action = createAction(tr("Import Geometry"), "actionImportGeometry");
        action->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+G", nullptr));
        fileAppButton->addAction(action);

        //工作目录
        action = createAction(tr("Working Dir"), "actionWorkingDir");
        fileAppButton->addAction(action);
    }

    void MainWindow::initHome()
    {
        QString type = tr("Home");
        SARibbonCategory* gategory = _ribbonBar->addCategoryPage(type);
        _ribbonBar->raiseCategory(gategory);

        QAction* action = nullptr;
        //项目
        SARibbonPannel* pannel = gategory->addPannel(tr("Object"));

        action = createAction(tr("New"), "actionNew", QIcon(":/icons/New_Project.svg"));
        panelAddAction(pannel, action, SARibbonPannelItem::Large);

        action = createAction(tr("Open"), "actionOpen", QIcon(":/icons/Open_File.svg"));
        panelAddAction(pannel, action, SARibbonPannelItem::Large);

        action = createAction(tr("Save"), "actionSave", QIcon(":/icons/Save.svg"));
        panelAddAction(pannel, action, SARibbonPannelItem::Large);

        action = createAction(tr("Import Geometry"), "actionImportGeometry", QIcon(":/icons/Import.svg"));
        panelAddAction(pannel, action, SARibbonPannelItem::Large);

		action = createAction(tr("Import Mesh"), "actionImportMesh", QIcon(":/icons/Import.svg"));
		panelAddAction(pannel, action, SARibbonPannelItem::Large);
        
        // 输出
        SARibbonPannel* pannelExport = gategory->addPannel(tr("Export"));
        //action = createAction(tr("Export Abaqus INP File"), "actionExportAbaqusINPFile", QIcon(":/icons/Export_INP.svg")); 
        //panelAddAction(pannelExport, action, SARibbonPannelItem::Large);
        action = createAction(tr("Export CGNS File"), "actionExportCGNSFile", QIcon(":/icons/Export_CGNS.svg"));
        panelAddAction(pannelExport, action, SARibbonPannelItem::Large);

        //组件
        SARibbonPannel* pannelComponent = gategory->addPannel(tr("component"));
        action = createAction(tr("Plugin"), "actionPlugin", QIcon(":/icons/Plugins.svg"));
        panelAddAction(pannelComponent, action, SARibbonPannelItem::Large);

    }

    void MainWindow::initHelp()
    {
        QString type = tr("Help");
        SARibbonCategory* gategory = _ribbonBar->addCategoryPage(type);
        _ribbonBar->raiseCategory(gategory);
        _helpPage = gategory;

        QAction* action = nullptr;
        SARibbonPannel* pannel = gategory->addPannel(tr("Help"));
        action = createAction(tr("License"), "actionLicense", QIcon(":/icons/License.svg"));
        panelAddAction(pannel, action, SARibbonPannelItem::Large);
        connect(action, &QAction::triggered, this, &MainWindow::showLicenseFile);
    }

    void MainWindow::showLicenseFile()
    {
        QString appDir = QCoreApplication::applicationDirPath();
        QDir dir(appDir);
        dir.cdUp();
        dir.cdUp();
        QString filePath = dir.absoluteFilePath("License.txt");

        if(!QFile::exists(filePath)) {
            QMessageBox::warning(this, tr("Error"),
                                tr("License file not found:\n%1").arg(filePath));
            return;
        }
        // 使用系统默认程序打开文件
        bool success = QDesktopServices::openUrl(QUrl::fromLocalFile(filePath));
    }

    void MainWindow::panelAddAction(SARibbonPannel * pannel, QAction * action, SARibbonPannelItem::RowProportion actionType)
    {
        //pannel中添加action
        if (pannel == nullptr || action == nullptr)return;
        pannel->addAction(action, actionType);
    }

    SARibbonCategory* MainWindow::addPage(const QString& panel)
    {
        if (_ribbonBar == nullptr) return nullptr;
        const int index = _ribbonBar->categoryIndex(_helpPage);

        SARibbonCategory* gategory = _ribbonBar->insertCategoryPage(panel, index);
        _ribbonBar->raiseCategory(gategory);
        _ribbonBar->setCurrentIndex(0);
        return gategory;
    }

    SARibbonPannel* MainWindow::addPanel(SARibbonCategory* page, const QString& text)
    {
        if (page == nullptr) return nullptr;

        SARibbonPannel* pannel = page->addPannel(text);
        return pannel;
    }

    void MainWindow::removePage(SARibbonCategory* page)
    {
        if (_ribbonBar == nullptr || page == nullptr) return;
        _ribbonBar->removeCategory(page);
    }

    RenderWidget * MainWindow::getRenderWidget()
    {
        return _renderWidget;
    }

    ControlPanel* MainWindow::getControlPanel()
    {
        return _controlPanel;
    }

    QAction* MainWindow::getAction(const QString actionName)
    {
        //获取Action
        if (actionName.isEmpty())return nullptr;
        QAction* action = this->findChild< QAction* >(actionName);
        return action;
    }
}
