/**
 * 
 * @file FITKTreeTableWidget.h
 * @brief 树状的表格结构UI
 * @author LiBaojun (libaojunqd@foxmail.com)
 * @date 2024-12-28
 * 
 */
#ifndef _FITK_TREE_TABLE_WIDGET_H___
#define _FITK_TREE_TABLE_WIDGET_H___
 

#include "FITKWidgetAPI.h"
#include <QTableWidget>

namespace Comp
{
    /**
     * @brief 树状表格UI
     * @author LiBaojun (libaojunqd@foxmail.com)
     * @date 2024-12-28
     */
    class FITKWidgetAPI FITKTreeTableWidget : public QTableWidget
    {
    public:
        /**
         * @brief Construct a new FITKTreeTableWidget object
         * @param[i]  parent         父对象
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-12-28
         */
        explicit FITKTreeTableWidget(QWidget* parent = nullptr);
        /**
         * @brief Destroy the FITKTreeTableWidget object
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-12-28
         */
        virtual ~FITKTreeTableWidget();
        /**
         * @brief 追加树形结构
         * @param[i]  text           文本提示
         * @param[i]  expandable     是否有下级对象
         * @parami[]  level          层级。0 是最高层级
         * @param[i]  mergeLine      是否合并行
         * @return int               返回行号
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-12-28
         */
        int appendTreeLevelItem(const QString& text, const bool expandable, const int level = 0, const bool mergeLine = false);
        /**
         * @brief 设置单元格文本
         * @param[i]  row            行号
         * @param[i]  col            列号
         * @param[i]  text           文本
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-12-28
         */
        void setItemText(const int row, const int col, const QString& text);


    };
}


#endif
