#ifndef _PLUGIN_ACTION_OPER_MANAGER_H___
#define _PLUGIN_ACTION_OPER_MANAGER_H___

#include "PluginAbstractAPI.h"
#include <QHash>
#include <QIcon>
#include <SARibbonPannelItem.h>

class SARibbonCategory;
class SARibbonPannel;
class QAction;

namespace GUI
{
    class MainWindow;
}

namespace Plugin
{
    class PluginAbstract;

    /**
     * @brief 管理插件操作的类
     *
     * 该类负责创建、获取和移除SARibbonCategory页面，并提供向SARibbonCategory添加Panel的功能。
     * @author
     * @date 2023-10-20
     */
    class PluginAbstractAPI PluginActionOpersManager
    {
    public:
        /**
         * @brief 构造函数
         * @param p 插件抽象类指针
         * @author libaojun
         * @date 2025-03-16
         */
        explicit PluginActionOpersManager(PluginAbstract* p);

        /**
         * @brief 析构函数
         * @author libaojun
         * @date 2025-03-16
         */
        virtual ~PluginActionOpersManager();

        /**
         * @brief 创建一个新的页面
         * @param text 页面名称
         * @return SARibbonCategory* 新创建的页面
         * @author libaojun
         * @date 2025-03-16
         */
        SARibbonCategory* createPage(const QString& text);

        /**
         * @brief 获取一个已有的页面
         * @param text 页面名称
         * @return SARibbonCategory* 已有的页面
         * @author libaojun
         * @date 2025-03-16
         */
        SARibbonCategory* getPage(const QString& text);

        /**
         * @brief 移除所有页面
         * @author libaojun
         * @date 2025-03-16
         */
        void removeAllPages();

        /**
         * @brief 向指定的SARibbonCategory对象中添加Panel
         * @param category SARibbonCategory对象指针
         * @param panelName Panel的名称
         * @author libaojun
         * @date 2025-03-16
         */
        SARibbonPannel* addPanelToCategory(SARibbonCategory* category, const QString& panelName);
        /**
      * @brief 创建一个新的Action并添加到MainWindow中
      * @param text Action的文本
      * @param objectName Action的对象名称
      * @param icon Action的图标
      * @param tips Action的提示信息
      * @return QAction* 新创建的Action
      * @date 2025-03-16
      */
        QAction* createAction (const QString& text, const QString& objectName, 
            const QIcon& icon = QIcon(), const QString& tips = QString());

        /**
        * @brief 向指定的SARibbonPannel对象中添加Action
        * @param pannel SARibbonPannel对象指针
        * @param action QAction对象指针
        * @param actionType Action的类型
        * @date 2025-03-16
        */
        void addActionToPanel(SARibbonPannel* pannel, QAction* action,
            SARibbonPannelItem::RowProportion actionType);

    private:
        /**
         * @brief 移除所有Action操作器，并且打断信号槽连接
         * @date 2025-03-16
         */
        void removeActionOpers();

    private:
        PluginAbstract* _plugin{}; ///< 插件抽象类指针
        QHash<QString, SARibbonCategory*> _ribbonBarPage{}; ///< 存储页面的哈希表
        QList<QAction*> _actions{}; ///< 存储Action的列表
    };
}

#endif
