/**********************************************************************
 * @file   GUIGeometryCirPatternDialog.h
 * @brief  环形阵列
 * @author guqingtao (15598887859@163.com)
 * @date   2025-04-03
 *********************************************************************/
#ifndef GUIGEOMETRYCIRPATTERNDIALOG_H
#define GUIGEOMETRYCIRPATTERNDIALOG_H

#include "FITK_Kernel/FITKCore/FITKAbstractGUIDialog.h"
#include "GraphData/GraphDataPick/GraphDataPickInfo.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoOperBool.h"

#include "PluginGeoAbstractAPI.h"
namespace Ui { class GUIGeometryCirPatternDialog; }
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
     * @brief  环形阵列对话框
     * @author guqingtao (15598887859@163.com)
     * @date   2025-04-03
     */
    class PluginGeoAbstractAPI GUIGeometryCirPatternDialog : public Core::FITKDialog
    {
        Q_OBJECT

    public:
        //拾取标志
        enum class PickFlag {
            None = 1,
            Target,
            AxisDirection,
        };
        /**
         * @brief  构造
         * @author guqingtao (15598887859@163.com)
         * @date   2025-04-03
         */
        GUIGeometryCirPatternDialog(Core::FITKActionOperator* oper,QWidget* parent = nullptr);
        /**
         * @brief    析构
         * @return   void
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-04-03
         */
        ~GUIGeometryCirPatternDialog();
    private:
        /**
         * @brief    初始化
         * @return   void
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-04-03
         */
        void init();
        /**
         * @brief    设置拾取方式
         * @param[i] objType 
         * @param[i] methodType 
         * @return   void
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-04-03
         */
        void enablePickSig(Graph::PickObjType objType, Graph::PickMethod methodType);
        /**
         * @brief    创建环形阵列
         * @return   void
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-04-03
         */
        void createCirPatternData();
        /**
         * @brief    处理拾取到的数据
         * @return   void
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-04-03
         */
        void pickFinishedOper();
        /**
         * @brief    获取数据名称
         * @return   QString
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-04-03
         */
        QString getNameText();
        /**
         * @brief    检查数据
         * @return   bool
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-04-03
         */
        bool checkData();
    private slots:
        /**
         * @brief  OK按钮
         * @author guqingtao (15598887859@163.com)
         * @date   2025-04-03
         */
        void on_pushButton_OK_clicked();
        /**
         * @brief    取消按钮
         * @return   void
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-04-03
         */
        void on_pushButton_Cancel_clicked();
        /**
         * @brief    拾取目标对象的按钮
         * @return   void
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-04-03
         */
        void on_Target_pick_clicked();
        /**
         * @brief    拾取旋转轴
         * @return   void
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-04-03
         */
        void on_Direction_pick_clicked();
        /**
         * @brief    阵列方式选择
         * @param[i] index 
         * @return   void
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-04-03
         */
        void on_comboBox_First_Parameter_currentIndexChanged(int index);
        void on_comboBox_Second_Parameter_currentIndexChanged(int index);
    private:
        /**
         * @brief  ui
         * @author guqingtao (15598887859@163.com)
         * @date   2025-04-03
         */
        Ui::GUIGeometryCirPatternDialog *_ui{};
        /**
         * @brief  几何数据对象
         * @author guqingtao (15598887859@163.com)
         * @date   2025-04-03
         */
        Interface::FITKAbsGeoCommand* _obj{};
        /**
         * @brief  操作器
         * @author guqingtao (15598887859@163.com)
         * @date   2025-04-03
         */
        Core::FITKActionOperator* _oper{};
        /**
         * @brief  几何数据管理器
         * @author guqingtao (15598887859@163.com)
         * @date   2025-04-03
         */
        Interface::FITKGeoCommandList* _geometryData{};
        /**
         * @brief  目标对象
         * @author guqingtao (15598887859@163.com)
         * @date   2025-04-03
         */
        Interface::VirtualShape _virtualTarget;
        /**
         * @brief  旋转轴
         * @author guqingtao (15598887859@163.com)
         * @date   2025-04-03
         */
        Interface::VirtualShape _virtualAxis;
        /**
         * @brief  数据ID
         * @author guqingtao (15598887859@163.com)
         * @date   2025-04-03
         */
        int _objID = -1;
        /**
         * @brief  拾取标志位
         * @author guqingtao (15598887859@163.com)
         * @date   2025-04-03
         */
        PickFlag _pickFlag = PickFlag::None;
    };
}
#endif // GUIGEOMETRYCIRPATTERNDIALOG_H
