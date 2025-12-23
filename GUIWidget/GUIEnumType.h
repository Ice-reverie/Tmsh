#ifndef _GUI_ENUM_TYPE_H___
#define _GUI_ENUM_TYPE_H___

#include "FITK_Component/FITKWidget/FITKTreeWidget.h"
#include "GUIWidgetAPI.h"

namespace GUI
{
    /**
     * @brief 树形菜单页类型
     * @author wangning (2185896382@qq.com)
     * @date 2025-03-26
     */
    enum TreeWidgetType
    {
        PageNone,     ///< 无类型
        PageModel,
        PageMesh,
    };

    /**
     * @brief PageModel各节点类型
     * @author wangning (2185896382@qq.com)
     * @date 2025-03-26
     */
    enum ModelTreeItemsType
    {
        MITNone = 0,
        MTModelRoot,                       ///< 几何model
            MTModel,                       ///< 实际几何
        MTMaterialRoot,                    ///< 材料
        MTSolidRoot,                       ///< 实体集合根节点
        MTSolid,                           ///< 单个实体
        MTShellRoot,                       ///< 壳体集合根节点
        MTShell,                           ///< 单个壳体
    };


    /**
     * @brief 控制面板里面的树形菜单，抽象类
     * @author wangning (2185896382@qq.com)
     * @date 2025-03-26
     */
    class GUIWidgetAPI TreeWidgetPage :public Comp::FITKTreeWidget
    {
    public:
        /**
         * @brief Construct a new Tree Widget Page object
         * @param[i]  parent         父对象
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-10-12
         */
        explicit TreeWidgetPage(QWidget* parent = nullptr) 
            :FITKTreeWidget(parent)
        {
        }
        /**
         * @brief Destroy the Tree Widget Page object
         * @author wangning (2185896382@qq.com)
         * @date 2025-03-26
         */
        virtual ~TreeWidgetPage() {};
        /**
         * @brief 获取页类型
         * @return TreeWidgetType
         * @author wangning (2185896382@qq.com)
         * @date 2025-03-26
         */
        virtual TreeWidgetType getTreeWidgetType() 
        {
            return PageNone;
        };

        virtual void updatePage(){};
    };
}


#endif
