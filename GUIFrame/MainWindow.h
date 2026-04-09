/**
 *
 * @file MainWindow.h
 * @brief 主界面声明
 * @author LiBaojun (libaojunqd@foxmail.com)
 * @date 2025-03-05
 *
 */
#ifndef __MAINWINDOW_H__
#define __MAINWINDOW_H__

#include <QMainWindow>
#include <QIcon>
#include "GUIFrameAPI.h"

#include "FITK_Kernel/FITKCore/FITKAbstractGUIObject.h"
#include <SARibbonMainWindow.h>
#include <SARibbonPannelItem.h>

#include <QFont>

class SARibbonBar;
class SARibbonPannel;
class SARibbonCategory;

namespace Ui
{
    class MainWindow;
}

namespace GUI
{
    class ActionEventHandler;
    class ControlPanel;
    class ConsoleWidget;
    class RenderWidget;

    /**
     * @brief 主界面声明
     * @author LiBaojun (libaojunqd@foxmail.com)
     * @date 2025-03-05
     */
    class GUIFRAMEAPI MainWindow : public SARibbonMainWindow, public Core::FITKAbstractGUIObject
    {
        Q_OBJECT;
    public:
        /**
         * @brief Construct a new Main Window object
         * @param[i]  parent         父对象
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2025-03-05
         */
        explicit MainWindow(QWidget *parent = nullptr);
        /**
         * @brief Destroy the Main Window object
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2025-03-05
         */
        virtual ~MainWindow();
        /**
       * @brief 从框架中获取主界面对象
       * @return MainWindow*
       * @author LiBaojun (libaojunqd@foxmail.com)
       * @date 2024-09-24
       */
        static MainWindow* GetMainWindowFromFramework();

        /**
         * @brief  获取界面的action消息处理器
         * @return 消息处理器
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @data   2024-06-12
         */
        ActionEventHandler* getActionEventHandle() const;
        /**
         * @brief 创建按钮并关联事件处理器
         * @param[i]  text          按钮显示字符
         * @param[i]  objectName    名称
         * @param[i]  icon          图标
         * @param[i]  Tip           提示信息
         * @return QAction*
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2025-03-05
         */
        QAction* createAction(const QString &text, const QString &objectName, const QIcon & icon = QIcon(), const QString& Tip = "");
        /**
         * @brief 是否关联事件处理器
         * @param[i]  act           action对象
         * @param[i]  connect       是否关联
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2025-03-05
         */
        void connectActionEventHandler(QAction* act, bool connect = true);
        /**
         * @brief action追加到panel
         * @param[i]  panel         面板
         * @param[i]  action         按钮
         * @param[i]  actionType     按钮大小
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2025-03-05
         */
        void panelAddAction(SARibbonPannel* pannel, QAction* action, SARibbonPannelItem::RowProportion actionType = SARibbonPannelItem::Large);
        /**
          * @brief 添加页面
          * @param[i]  pageText      页面标题
          * @return SARibbonCategory*
          * @author LiBaojun (libaojunqd@foxmail.com)
          * @date 2025-03-05
          */
        SARibbonCategory* addPage(const QString& pageText);
        /**
          * @brief 添加面板
          * @param[i]  page          页面
          * @param[i]  text          面板标题
          * @return SARibbonPannel*
          * @author LiBaojun (libaojunqd@foxmail.com)
          * @date 2025-03-05
          */
        SARibbonPannel* addPanel(SARibbonCategory* page, const QString& text);
        /**
          * @brief 移除页面
          * @param[i]  page          页面
          * @author LiBaojun (libaojunqd@foxmail.com)
          * @date 2025-03-05
          */
        void removePage(SARibbonCategory* page);
        /**
         * @brief    获取渲染界面
         * @return   RenderWidget*             渲染界面
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-03-12
         */
        RenderWidget* getRenderWidget();
        /**
        * @brief 获取控制面板
        * @return ControlPanel*
        * @author LiBaojun (libaojunqd@foxmail.com)
        * @date 2025-02-05
        */
        ControlPanel* getControlPanel();

    private:
        /**
         * @brief 初始化
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2025-03-05
         */
        void init();
        /**
         * @brief 初始化中心窗口
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2025-03-05
         */
        void initCentralWidget();
        /**
         * @brief 初始化应用按钮
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2025-03-05
         */
        void initApplicationButton();
        /**
         * @brief 初始化Home Panel
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2025-03-05
         */
        void initHome();
        /**
         * @brief 初始化Help Panel
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2025-03-05
         */
        void initHelp();

        /**
         * @brief 查找action
         * @param[i]  actionName     action名称
         * @return QAction* action对象
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-05-30
         */
        QAction* getAction(const QString actionName);
    private:
        /**
         * @brief Ribbon菜单
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2025-03-05
         */
        SARibbonBar* _ribbonBar = nullptr;
        /**
         * @brief 帮助标签页，所有插件的标签页在这个页面之前插入
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2025-03-05
         */
        SARibbonCategory* _helpPage{};
        /**
         * @brief 按钮处理器
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2025-03-05
         */
        ActionEventHandler* m_ActionHandler{};
        /**
         * @brief 控制面板
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2025-03-05
         */
        ControlPanel* _controlPanel{};
        /**
         * @brief 控制台
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2025-03-05
         */
        ConsoleWidget* _consoleWidget{};
        /**
         * @brief 渲染窗口
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2025-03-05
         */
        RenderWidget* _renderWidget{};

    private slots:
        void showLicenseFile();

    };
}

#endif // !__MAINWINDOW_H__
