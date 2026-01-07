/**
 * 
 * @file ControlPanel.h
 * @brief 控制面板
 * @author LiBaojun (libaojunqd@foxmail.com)
 * @date 2025-03-05
 * 
 */
#ifndef _MESH_APP_CONTROL_PANEL_H___
#define _MESH_APP_CONTROL_PANEL_H___
 
#include "GUIFrameAPI.h"
#include <QDockWidget>
#include "FITK_Kernel/FITKCore/FITKAbstractGUIObject.h"
#include "GUIWidget/GUIEnumType.h"

namespace Comp
{
    class FITKGraph3DWindowVTK;
}

namespace Ui
{
    class ControlPanel;
}


namespace GUI
{
    enum TreeWidgetType;
    class TreeWidgetPage;

    /**
     * @brief 控制面板
     * @author LiBaojun (libaojunqd@foxmail.com)
     * @date 2025-03-05
     */
    class GUIFRAMEAPI ControlPanel : public QDockWidget, 
         public Core::FITKAbstractGUIObject
    {
    public:
        /**
         * @brief Construct a new Control Panel object
         * @param[i]  parent         父对象
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2025-03-05
         */
        explicit ControlPanel(QWidget* parent = nullptr);
        /**
         * @brief Destroy the Control Panel object
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2025-03-05
         */
        virtual ~ControlPanel();
        /**
        * @brief 根据类型获取页面
        * @param[i]  t              类型
        * @return TreeWidgetPage*
        * @author wangning (2185896382@qq.com)
        * @date 2025-03-26
        */
        TreeWidgetPage* getTreeWidget(TreeWidgetType t);

        /**
        * @brief 注入三维渲染窗口，避免子页面依赖GUIFrame
        * @param[i] graphWidget 渲染窗口
        */
        void setGraphWidget(Comp::FITKGraph3DWindowVTK* graphWidget);
    
    private:
        /**
         * @brief UI对象
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2025-03-05
         */
        Ui::ControlPanel* _ui{};
        /**
         * @brief 树形菜单页
         * @author wangning (2185896382@qq.com)
         * @date 2025-03-26
         */
        QHash<TreeWidgetType, TreeWidgetPage*> _pages{};
    };
}


#endif
