/**********************************************************************
 * @file   GUIGeometryRecPatternDialog.h
 * @brief  矩形阵列
 * @author guqingtao (15598887859@163.com)
 * @date   2025-04-03
 *********************************************************************/
#ifndef GUIGEOMETRYRECPATTERNDIALOG_H
#define GUIGEOMETRYRECPATTERNDIALOG_H

#include "FITK_Kernel/FITKCore/FITKAbstractGUIDialog.h"
#include "GraphData/GraphDataPick/GraphDataPickInfo.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoOperBool.h"

#include "PluginGeoAbstractAPI.h"
namespace Ui { class GUIGeometryRecPatternDialog; }
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
     * @brief  矩形阵列对话框
     * @author guqingtao (15598887859@163.com)
     * @date   2025-04-03
     */
    class PluginGeoAbstractAPI GUIGeometryRecPatternDialog : public Core::FITKDialog
    {
        Q_OBJECT

    public:
        //拾取标志
        enum class PickFlag {
            None = 1,
            Target,
            FirstDirection,
            SecondDirection
        };
        /**
         * @brief  构造
         * @author guqingtao (15598887859@163.com)
         * @date   2025-04-03
         */
        GUIGeometryRecPatternDialog(Core::FITKActionOperator* oper,QWidget* parent = nullptr);
        /**
         * @brief    析构
         * @return   void
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-04-03
         */
        ~GUIGeometryRecPatternDialog();
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
         * @brief    创建矩形阵列
         * @return   void
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-04-03
         */
        void createRecPatternData();
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
         * @brief    获取点的坐标
         * @param[i] id 
         * @return   std::array<double,3>
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-04-03
         */
        std::array<double, 3> getPointXYZ(int id);
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
         * @brief    第一方向
         * @return   void
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-04-03
         */
        void on_First_Direction_pick_clicked();
        /**
         * @brief    第二方向
         * @return   void
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-04-03
         */
        void on_Second_Direction_pick_clicked();
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
        Ui::GUIGeometryRecPatternDialog *_ui{};
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
#endif // GUIGEOMETRYRECPATTERNDIALOG_H
