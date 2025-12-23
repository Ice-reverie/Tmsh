/**
 * 
 * @file FITKTreeWidgetOperator.h
 * @brief 树形菜单事件处理抽象操作器声明
 * @author LiBaojun (libaojunqd@foxmail.com)
 * @date 2024-10-08
 * 
 */
#ifndef _FITK_TREEWIDGET_OPERATOR_H___
#define _FITK_TREEWIDGET_OPERATOR_H___
 
#include "FITKAbstractOperator.h"

class QTreeWidget;
class QTreeWidgetItem;

namespace Core
{
    /**
     * @brief 树形菜单操作器抽象基类
     * @author LiBaojun (libaojunqd@foxmail.com)
     * @date 2024-10-08
     */
    class FITKCoreAPI FITKTreeWidgetOperator : public FITKAbstractOperator
    {
    public:
        /**
         * @brief Construct a new FITKTreeWidgetOperator object
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-10-08
         */
        explicit FITKTreeWidgetOperator() = default;
        /**
         * @brief Destroy the FITKTreeWidgetOperator object
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-10-08
         */
        virtual ~FITKTreeWidgetOperator() = 0;
        /**
         * @brief 树形节点点击事件
         * @param[i]  w              树形菜单窗口
         * @param[i]  item           点击的树形菜单节点
         * @param[i]  col            点击列数
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-10-08
         */
        virtual void on_itemClicked(QTreeWidget* w, QTreeWidgetItem* item, int col = 0);
        /**
         * @brief 树形节点双击事件
         * @param[i]  w              树形菜单窗口
         * @param[i]  item           点击的树形菜单节点
         * @param[i]  col            点击列数
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-10-08
         */
        virtual void on_itemDoubleClicked(QTreeWidget* w, QTreeWidgetItem* item, int col = 0);

    private:
    };
 
}


#endif
