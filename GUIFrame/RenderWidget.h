/**
 * 
 * @file RenderWidget.h
 * @brief 渲染窗口
 * @author LiBaojun (libaojunqd@foxmail.com)
 * @date 2025-03-05
 * 
 */
#ifndef _FITK_RENDERWIDGET_MESH_APP_H___
#define _FITK_RENDERWIDGET_MESH_APP_H___
 
#include "GUIFrameAPI.h"
#include "FITK_Kernel/FITKCore/FITKAbstractGUI.h"

namespace GUI
{
    /**
     * @brief 渲染窗口
     * @author LiBaojun (libaojunqd@foxmail.com)
     * @date 2025-03-05
     */
    class GUIFRAMEAPI RenderWidget : public  Core::FITKWidget
    {
    public:
        /**
         * @brief Construct a new Render Widget object
         * @param[i]  parent         父对象
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2025-03-05
         */
        explicit  RenderWidget(QWidget* parent = nullptr);
        /**
         * @brief Destroy the Render Widget object
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2025-03-05
         */
        virtual ~RenderWidget();
        /**
         * @brief    获取渲染窗口
         * @return   QWidget*                 渲染窗口
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-03-12
         */
        QWidget* getRenderWin();
    private:
        /**
         * @brief 渲染窗口
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2025-03-05
         */
         QWidget* _renderWin{};
    };
}
#endif