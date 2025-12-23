/**********************************************************************
 * @file   WorkingDirDlg.h
 * @brief  工作路径
 * @author guqingtao (15598887859@163.com)
 * @date   2025-04-07
 *********************************************************************/
#ifndef _WORKING_DIR_DLG__H___
#define _WORKING_DIR_DLG__H___
 
#include "GUIFrameAPI.h"
#include "FITK_Kernel/FITKCore/FITKAbstractGUI.h"

namespace Ui
{
    class WorkingDirDlg;
}

namespace GUI
{
    /**
     * @brief  工作路径
     * @author guqingtao (15598887859@163.com)
     * @date   2025-04-07
     */
    class GUIFRAMEAPI WorkingDirDlg : public Core::FITKDialog
    {
        Q_OBJECT
    public:
        /**
         * @brief  Construct a new Working Dir Dlg object
         * @author guqingtao (15598887859@163.com)
         * @date   2025-04-07
         */
        explicit WorkingDirDlg(QWidget* parent= nullptr);
        /**
         * @brief    Destroy the Working Dir Dlg object
         * @return   void
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-04-07
         */
        virtual ~WorkingDirDlg();
        
    private slots:
        /**
         * @brief  浏览按钮
         * @author guqingtao (15598887859@163.com)
         * @date   2025-04-07
         */
        void on_BrowseButton_clicked();
        /**
         * @brief    确认按钮
         * @return   void
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-04-07
         */
        void on_OKButton_clicked();
        /**
         * @brief    退出按钮
         * @return   void
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-04-07
         */
        void on_QuitButton_clicked();
        
    private:
        /**
         * @brief    初始化
         * @return   void
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-04-07
         */
        void init();

    private:
        /**
         * @brief  ui
         * @author guqingtao (15598887859@163.com)
         * @date   2025-04-07
         */
        Ui::WorkingDirDlg* _ui{};
    };
}


#endif
