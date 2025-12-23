/**********************************************************************
 * @file   GUIGeometryGroupDialog.h
 * @brief  几何分组窗口
 * @author liuzhonghua (liuzhonghuaszch@163.com)
 * @date   2025-04-16
 *********************************************************************/
#ifndef _GUIGEOMETRYGROUPDIALOG_H
#define _GUIGEOMETRYGROUPDIALOG_H

#include "GUIDialogAPI.h"
#include <QObject>
#include "FITK_Kernel/FITKCore/FITKAbstractGUIDialog.h"

namespace Core
{
    class FITKActionOperator;
}

namespace Interface
{
    class FITKAbsGeoCommand;
    class FITKGeoComponentManager;
}

namespace Ui {
class GUIGeometryGroupDialog;
}

namespace GUI
{
    /**
     * @brief  几何分组窗口类
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date   2024-12-31
     */
    class GUIDialogAPI GUIGeometryGroupDialog : public Core::FITKDialog
    {
        Q_OBJECT
    public:
        /**
         * @brief  create object
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-12-31
         */
        GUIGeometryGroupDialog(Core::FITKActionOperator* oper, Interface::FITKAbsGeoCommand* geoCommand, QWidget *parent = nullptr);
        /**
         * @brief    Destroy object
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-12-31
         */
        virtual ~GUIGeometryGroupDialog();
        /**
         * @brief    初始化
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-12-31
         */
        void init();

    protected slots:
        /**
         * @brief OK button clicked slot
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date 2024-10-23
         */
        void on_pushButton_OK_clicked();
        /**
         * @brief    Cancel button clicked slot
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-04-17
         */
        void on_pushButton_Cancel_clicked();
        /**
         * @brief Add button clicked slot
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date 2024-10-23
         */
        void on_pushButton_Add_clicked();
        /**
         * @brief Clear button clicked slot
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date 2024-10-23
         */
        void on_pushButton_Clear_clicked();
        /**
         * @brief    signal clicked tableWidget clicked slot
         * @param[i] row 
         * @param[i] column 
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-04-16
         */
        void on_tableWidget_cellClicked(int row, int column);
        /**
         * @brief 面组界面ok点击事件
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-07-16
         */
        void slotFaceWidgetOkClicked();
        /**
         * @brief    面组界面Cancel点击事件
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-04-17
         */
        void slotFaceWidgetCancelClicked();
        /**
         * @brief 面组界面delete点击事件
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-07-16
         */
        void slotFaceWidgetDeleteClicked();

    private:
        /**
         * @brief    初始化TableWidget
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-04-16
         */
        void initTableWidget();
        /**
         * @brief    设置数据到TableWidget
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-04-16
         */
        void setDataToWidget();
        /**
         * @brief 清除所有高亮
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-04-16
         */
        void clearGraphHight();
        /**
         * @brief    设置全部分组的选择状态
         * @param[i] state 
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-04-16
         */
        void setAllFaceGroupSelectState(bool state);
        /**
         * @brief    获取新名称
         * @return   QString
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-04-17
         */
        QString getNewName();
        /**
         * @brief    取消事件重写
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-04-17
         */
        virtual void reject() override;

    protected:
        /**
         * @brief ui object
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date 2024-10-23
         */
        Ui::GUIGeometryGroupDialog* _ui = nullptr;
        /**
         * @brief 操作器对象
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date 2024-10-23
         */
        Core::FITKActionOperator* _oper = nullptr;
        /**
         * @brief  几何命令
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date 2024-10-23
         */
        Interface::FITKAbsGeoCommand* _obj = nullptr;
    };
}


#endif // _GUIGeometryGroupDialog_H
