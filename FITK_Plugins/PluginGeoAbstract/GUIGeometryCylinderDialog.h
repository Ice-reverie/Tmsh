/**********************************************************************
 * @file   GUIGeometryCylinderDialog.h
 * @brief  创建圆柱体
 * @author guqingtao (15598887859@163.com)
 * @date   2025-04-01
 *********************************************************************/
#ifndef GUIGEOMETRYCYLINDERDIALOG_H
#define GUIGEOMETRYCYLINDERDIALOG_H

#include "FITK_Kernel/FITKCore/FITKAbstractGUIDialog.h"
#include "GraphData/GraphDataPick/GraphDataPickInfo.h"
#include "PluginGeoAbstractAPI.h"
#include <array>
namespace Ui { class GUIGeometryCylinderDialog; }
namespace Interface {
    class FITKAbsGeoCommand;
    class FITKGeoCommandList;
}
namespace Core
{
    class FITKActionOperator;
}
namespace GUI {
    /**
     * @brief  创建圆柱体
     * @author guqingtao (15598887859@163.com)
     * @date   2025-04-01
     */
    class PluginGeoAbstractAPI GUIGeometryCylinderDialog : public Core::FITKDialog
    {
        Q_OBJECT

    public:
        /**
         * @brief  构造
         * @author guqingtao (15598887859@163.com)
         * @date   2025-04-01
         */
        GUIGeometryCylinderDialog(Core::FITKActionOperator* oper,QWidget* parent = nullptr);
        /**
         * @brief    析构
         * @return   void
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-04-01
         */
        ~GUIGeometryCylinderDialog();
    private:
        /**
         * @brief    初始化
         * @return   void
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-04-01
         */
        void init();
        /**
         * @brief    设置拾取方式
         * @param[i] objType 
         * @param[i] methodType 
         * @return   void
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-04-01
         */
        void enablePickSig(Graph::PickObjType objType, Graph::PickMethod methodType);
        /**
         * @brief    创建圆柱体
         * @return   void
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-04-01
         */
        void createCylinderData();
        /**
         * @brief    处理拾取到的数据
         * @return   void
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-04-01
         */
        void pickFinishedOper();
        /**
         * @brief    获取数据名称
         * @return   QString
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-04-01
         */
        QString getNameText();

    private slots:
        /**
         * @brief  OK按钮
         * @author guqingtao (15598887859@163.com)
         * @date   2025-04-01
         */
        void on_pushButton_OK_clicked();
        /**
         * @brief    取消按钮
         * @return   void
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-04-01
         */
        void on_pushButton_Cancel_clicked();
        /**
         * @brief    拾取点的按钮
         * @return   void
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-04-01
         */
        void on_point1_pick_clicked();
    private:
        /**
         * @brief  ui
         * @author guqingtao (15598887859@163.com)
         * @date   2025-04-01
         */
        Ui::GUIGeometryCylinderDialog *_ui{};
        /**
         * @brief  几何数据对象
         * @author guqingtao (15598887859@163.com)
         * @date   2025-04-01
         */
        Interface::FITKAbsGeoCommand* _obj{};
        /**
         * @brief  操作器
         * @author guqingtao (15598887859@163.com)
         * @date   2025-04-01
         */
        Core::FITKActionOperator* _oper{};
        /**
         * @brief  几何数据管理器
         * @author guqingtao (15598887859@163.com)
         * @date   2025-04-01
         */
        Interface::FITKGeoCommandList* _geometryData{};
        /**
         * @brief  数据ID
         * @author guqingtao (15598887859@163.com)
         * @date   2025-04-01
         */
        int _objID = -1;
    };
}
#endif // GUIGEOMETRYCYLINDERDIALOG_H
