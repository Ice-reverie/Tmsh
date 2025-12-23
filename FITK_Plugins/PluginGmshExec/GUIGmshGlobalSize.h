/**********************************************************************
 * @file   GUIGmshGlobalSize.h
 * @brief  Gmsh网格尺寸信息
 * @author guqingtao (15598887859@163.com)
 * @date   2025-06-05
 *********************************************************************/
#ifndef GUIGMSHGLOBALSIZE_H
#define GUIGMSHGLOBALSIZE_H

#include "FITK_Kernel/FITKCore/FITKAbstractGUIDialog.h"
#include "PluginGmshExecAPI.h"

namespace Ui { class GUIGmshGlobalSize; }

namespace Core
{
    class FITKActionOperator;
}
namespace GUI {
    /**
     * @brief  Gmsh网格尺寸信息
     * @author guqingtao (15598887859@163.com)
     * @date   2025-06-05
     */
    class  PluginGmshExecAPI GUIGmshGlobalSize : public Core::FITKDialog
    {
        Q_OBJECT

    public:
        /**
         * @brief  构造
         * @author guqingtao (15598887859@163.com)
         * @date   2025-06-05
         */
        GUIGmshGlobalSize(Core::FITKActionOperator* oper,QWidget* parent = nullptr);
        /**
         * @brief    析构
         * @return   void
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-06-05
         */
        ~GUIGmshGlobalSize();
    private:
        /**
         * @brief    初始化
         * @return   void
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-06-05
         */
        void init();

    private slots:
        /**
         * @brief  OK按钮
         * @author guqingtao (15598887859@163.com)
         * @date   2025-06-05
         */
        void on_pushButton_OK_clicked();
        /**
         * @brief    Cancel按钮
         * @return   void
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-06-05
         */
        void on_pushButton_Cancel_clicked();

    private:
        /**
         * @brief  ui
         * @author guqingtao (15598887859@163.com)
         * @date   2025-06-05
         */
        Ui::GUIGmshGlobalSize *_ui{};
        /**
         * @brief  操作器
         * @author guqingtao (15598887859@163.com)
         * @date   2025-06-05
         */
        Core::FITKActionOperator* _oper{};
    };
}
#endif 
