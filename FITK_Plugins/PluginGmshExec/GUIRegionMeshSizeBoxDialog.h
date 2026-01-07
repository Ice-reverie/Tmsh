/**********************************************************************
 * @file   GUIRegionMeshSizeBoxDialog.h
 * @brief  长方体区域网格
 * @author guqingtao (15598887859@163.com)
 * @date   2025-06-07
 *********************************************************************/
#ifndef GUIREGIONMESHSIZEBOXDIALOG_H
#define GUIREGIONMESHSIZEBOXDIALOG_H

#include "FITK_Kernel/FITKCore/FITKAbstractGUIDialog.h"
#include "GraphData/GraphDataPick/GraphDataPickInfo.h"
#include "PluginGmshExecAPI.h"

namespace Ui { class GUIRegionMeshSizeBoxDialog; }

namespace Core
{
    class FITKActionOperator;
}
namespace GUI {
    /**
     * @brief  创建长方体区域网格
     * @author guqingtao (15598887859@163.com)
     * @date   2025-06-07
     */
    class PluginGmshExecAPI GUIRegionMeshSizeBoxDialog : public Core::FITKDialog
    {
        Q_OBJECT

    public:
        /**
         * @brief  构造
         * @author guqingtao (15598887859@163.com)
         * @date   2025-06-16
         */
        GUIRegionMeshSizeBoxDialog(Core::FITKActionOperator* oper,QWidget* parent = nullptr);
        /**
         * @brief    析构
         * @return   void
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-06-16
         */
        ~GUIRegionMeshSizeBoxDialog();
    private:
        /**
         * @brief    初始化
         * @return   void
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-06-16
         */
        void init();
        /**
         * @brief    设置拾取方式
         * @param[i] objType 
         * @param[i] methodType 
         * @return   void
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-06-16
         */
        void enablePickSig(Graph::PickObjType objType, Graph::PickMethod methodType);
        /**
         * @brief    处理拾取到的数据
         * @return   void
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-06-16
         */
        void pickFinishedOper();

    private slots:
        /**
         * @brief  OK按钮
         * @author guqingtao (15598887859@163.com)
         * @date   2025-06-16
         */
        void on_pushButton_OK_clicked();
        /**
         * @brief    关闭按钮
         * @return   void
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-06-16
         */
        void on_pushButton_Cancel_clicked();
        /**
         * @brief    拾取点
         * @return   void
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-06-16
         */
        void on_point1_pick_clicked();
    private:
        /**
         * @brief  ui
         * @author guqingtao (15598887859@163.com)
         * @date   2025-06-16
         */
        Ui::GUIRegionMeshSizeBoxDialog *_ui{};
        /**
         * @brief  操作器
         * @author guqingtao (15598887859@163.com)
         * @date   2025-06-16
         */
        Core::FITKActionOperator* _oper{};

    };
}
#endif 
