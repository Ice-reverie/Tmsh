/**********************************************************************
 * @file   GUIGeometryBoolDialog.h
 * @brief  bool运算
 * @author guqingtao (15598887859@163.com)
 * @date   2025-04-01
 *********************************************************************/
#ifndef GUIGEOMETRYBOOLDIALOG_H
#define GUIGEOMETRYBOOLDIALOG_H

#include "FITK_Kernel/FITKCore/FITKAbstractGUIDialog.h"
#include "GraphData/GraphDataPick/GraphDataPickInfo.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoOperBool.h"

#include "PluginGeoAbstractAPI.h"
namespace Ui { class GUIGeometryBoolDialog; }
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
     * @brief  创建bool运算对话框
     * @author guqingtao (15598887859@163.com)
     * @date   2025-04-01
     */
    class PluginGeoAbstractAPI GUIGeometryBoolDialog : public Core::FITKDialog
    {
        Q_OBJECT

    public:
        //拾取标志
        enum class PickFlag {
            None = 1,
            Target,
            Tool
        };
        /**
         * @brief  构造
         * @author guqingtao (15598887859@163.com)
         * @date   2025-04-01
         */
        GUIGeometryBoolDialog(Core::FITKActionOperator* oper, Interface::FITKAbsGeoOperBool::GeoBoolOperType type ,QWidget* parent = nullptr);
        /**
         * @brief    析构
         * @return   void
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-04-01
         */
        ~GUIGeometryBoolDialog();
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
         * @brief    bool运算
         * @return   void
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-04-01
         */
        void createBoolData();
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
         * @brief    拾取目标对象的按钮
         * @return   void
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-04-01
         */
        void on_Target_pick_clicked();
        /**
         * @brief    拾取工具对象的按钮
         * @return   void
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-04-01
         */
        void on_Tool_pick_clicked();
    private:
        /**
         * @brief  ui
         * @author guqingtao (15598887859@163.com)
         * @date   2025-04-01
         */
        Ui::GUIGeometryBoolDialog *_ui{};
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
         * @brief  目标对象
         * @author guqingtao (15598887859@163.com)
         * @date   2025-04-01
         */
        Interface::VirtualShape _virtualTarget;
        /**
         * @brief  工具对象
         * @author guqingtao (15598887859@163.com)
         * @date   2025-04-01
         */
        Interface::VirtualShape _virtualTool;
        /**
         * @brief  数据ID
         * @author guqingtao (15598887859@163.com)
         * @date   2025-04-01
         */
        int _objID = -1;
        /**
         * @brief  拾取标志位
         * @author guqingtao (15598887859@163.com)
         * @date   2025-04-02
         */
        PickFlag _pickFlag = PickFlag::None;
        /**
         * @brief  bool标志位
         * @author guqingtao (15598887859@163.com)
         * @date   2025-04-02
         */
        Interface::FITKAbsGeoOperBool::GeoBoolOperType _boolFlag = Interface::FITKAbsGeoOperBool::GeoBoolOperType::GBTNone;
    };
}
#endif // GUIGEOMETRYBOOLDIALOG_H
