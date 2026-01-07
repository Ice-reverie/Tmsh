#include "PluginActionOpersManager.h"
#include "PluginAbstract.h"
#include "GUIFrame/MainWindow.h"
#include "FITK_Kernel/FITKCore/FITKOperatorRepo.h"


namespace Plugin
{
    PluginActionOpersManager::PluginActionOpersManager(PluginAbstract* p)
        :_plugin(p)
    {
    }

    PluginActionOpersManager::~PluginActionOpersManager()
    {
    }

    SARibbonCategory* PluginActionOpersManager::createPage(const QString & text)
    {
        if (_plugin== nullptr) return nullptr;
        GUI::MainWindow* mw = _plugin->getMainWindow();
        if (mw == nullptr) return nullptr;
        SARibbonCategory* ribbonBarPage = mw->addPage(text);
        if (ribbonBarPage == nullptr) return nullptr;
        _ribbonBarPage.insert(text, ribbonBarPage);
        return ribbonBarPage;
    }

    SARibbonCategory* PluginActionOpersManager::getPage(const QString & text)
    {
        return _ribbonBarPage.value(text);
    }

    void PluginActionOpersManager::removeAllPages()
    {
        if (_plugin == nullptr) return;
        GUI::MainWindow* mw = _plugin->getMainWindow();
        if (mw == nullptr) return ;
        this->removeActionOpers();
        QList<SARibbonCategory*> pages = _ribbonBarPage.values();

        for (SARibbonCategory* page :pages)
        {
            mw->removePage(page);
 //           delete page;
        }
        _ribbonBarPage.clear();
    }

    SARibbonPannel* PluginActionOpersManager::addPanelToCategory(SARibbonCategory* category, const QString& panelName)
    {
        if (_plugin == nullptr) return nullptr;
        GUI::MainWindow* mw = _plugin->getMainWindow();
        if (mw == nullptr || category == nullptr) return nullptr; 
        return mw->addPanel(category, panelName);
    }
    QAction* Plugin::PluginActionOpersManager::createAction(const QString& text, const QString& objectName, 
        const QIcon& icon, const QString& tips)
    {
        if (_plugin == nullptr) return  nullptr;
        GUI::MainWindow* mainWindow = _plugin->getMainWindow();
        if (mainWindow == nullptr)  return nullptr;
        
        return mainWindow->createAction(text, objectName, icon, tips);
    }

    void PluginActionOpersManager::addActionToPanel(SARibbonPannel* pannel, QAction* action, SARibbonPannelItem::RowProportion actionType)
    {
        if (_plugin == nullptr) return;
        
        GUI::MainWindow* mainWindow = _plugin->getMainWindow();
        if (mainWindow == nullptr) return;
        mainWindow->panelAddAction(pannel, action, actionType); 

        if (!_actions.contains(action))
            _actions.append(action);
    }

    void PluginActionOpersManager::removeActionOpers()
    {
        if (_plugin == nullptr) return;

        GUI::MainWindow* mainWindow = _plugin->getMainWindow();
        if (mainWindow == nullptr) return;

        for (QAction* ac  : _actions)
        {
            if (ac == nullptr) continue;
            mainWindow->connectActionEventHandler(ac, false);
            FITKOPERREPO->removeOperator(ac->objectName());
        }
        _actions.clear();
    }

}


