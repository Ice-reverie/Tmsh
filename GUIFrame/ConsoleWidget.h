/**
 * 
 * @file ConsoleWidget.h
 * @brief 控制台窗口
 * @author LiBaojun (libaojunqd@foxmail.com)
 * @date 2025-03-05
 * 
 */
#ifndef _MESH_APP_CONSOLE_WIDGET_H___
#define _MESH_APP_CONSOLE_WIDGET_H___

#include "GUIFrameAPI.h"
#include "FITK_Kernel/FITKCore/FITKAbstractGUIObject.h"
#include <QTabWidget>

namespace Ui
{
    class ConsoleWidget;
}

namespace GUI
{
    /**
     * @brief 控制台窗口
     * @author LiBaojun (libaojunqd@foxmail.com)
     * @date 2025-03-05
     */
    class GUIFRAMEAPI ConsoleWidget : public QTabWidget, public Core::FITKAbstractGUIObject
    {
    public:
        /**
         * @brief Construct a new Console Widget object
         * @param[i]  parent         父对象
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2025-03-05
         */
        explicit ConsoleWidget(QWidget* parent = nullptr);
        /**
         * @brief Destroy the Console Widget object
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2025-03-05
         */
        virtual ~ConsoleWidget();


    private:
        /**
         * @brief UI对象
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2025-03-05
         */
        Ui::ConsoleWidget* _ui{};
    };
}


#endif
