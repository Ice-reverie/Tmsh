/**********************************************************************
 * @file   GUIGeometryFaceGroupSelectWidget.h
 * @brief  几何面组选择面组件
 * @author liuzhonghua (liuzhonghuaszch@163.com)
 * @date   2025-04-16
 *********************************************************************/
#ifndef GUIGEOMETRYFACEGROUPSELECTWIDGET_H
#define GUIGEOMETRYFACEGROUPSELECTWIDGET_H

#include <QWidget>

namespace Ui {
    class GUIGeometryFaceGroupSelectWidget;
}

namespace GUI
{
    /**
     * @brief  几何面组选择面组件
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date   2025-04-16
     */
    class GUIGeometryFaceGroupSelectWidget :public QWidget
    {
        Q_OBJECT;
    public:
        /**
         * @brief Construct a new Comp Face Group Select Widget object
         * @param[i]  parent         My Param doc
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-04-16
         */
        GUIGeometryFaceGroupSelectWidget(QWidget* parent);
        /**
         * @brief Destroy the Comp Face Group Select Widget object
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-04-16
         */
        ~GUIGeometryFaceGroupSelectWidget();
        /**
         * @brief init
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-08-08
         */
        void init();
        /**
         * @brief    设置分组数据
         * @param[i] groupName 分组名称
         * @param[i] groupID 分组ID 
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-04-16
         */
        void setGroupData(QString groupName, int groupID = -1);
        /**
         * @brief    获取分组名称
         * @return   QString
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-04-16
         */
        QString getGroupName();
        /**
         * @brief    获取分组的ID
         * @return   int
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-04-16
         */
        int getGroupID();
        /**
         * @brief    设置拾取数据ID链表
         * @param[i] ids 
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-04-16
         */
        void setPickDataIDs(QList<int> ids);
        /**
         * @brief    获取拾取数据ID链表
         * @return   QList<int>
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-04-16
         */
        QList<int> getPickDataIDs();
        /**
         * @brief    更新窗口
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-04-16
         */
        void updateWidget();
        /**
         * @brief    设置选择状态
         * @param[i] state 
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-04-16
         */
        void setSelectState(bool state);

    private slots:
        /**
         * @brief  on pushButton OK clicked slot
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-04-16
         */
        void on_pushButton_OK_clicked();
        /**
         * @brief    on pushButton Cancel clicked slot
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-04-16
         */
        void on_pushButton_Cancel_clicked();
        /**
         * @brief    on pushButton Delete clicked slot
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-04-16
         */
        void on_pushButton_Delete_clicked();

    signals:
        /**
         * @brief  选择模式确定按钮信号
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-04-16
         */
        void sigOkClickedFinish();
        /**
         * @brief    选择模式取消按钮信号
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-04-17
         */
        void sigCancelClicked();
        /**
         * @brief  选择删除按钮信号
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-08-08
         */
        void sigDeleteClicked();

    private:
        /**
         * @brief ui
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-04-16
         */
        Ui::GUIGeometryFaceGroupSelectWidget* _ui = nullptr;
        /**
         * @brief  分组数据
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-04-16
         */
        QPair<QString, int> _groupData{ QString(), -1 };
        /**
         * @brief  拾取的数据ID
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-04-16
         */
        QList<int> _pickDataIDs;
    };
}

#endif
