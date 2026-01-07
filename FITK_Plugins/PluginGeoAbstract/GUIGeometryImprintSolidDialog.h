/**********************************************************************
 * @file   GUIGeometryImprintSolidDialog.h
 * @brief  压印体
 * @author guqingtao (15598887859@163.com)
 * @date   2025-04-03
 *********************************************************************/
#ifndef GUIGEOMETRYIMPRINTSOLIDDIALOG_H
#define GUIGEOMETRYIMPRINTSOLIDDIALOG_H

#include "FITK_Kernel/FITKCore/FITKAbstractGUIDialog.h"
#include "GraphData/GraphDataPick/GraphDataPickInfo.h"
#include "PluginGeoAbstractAPI.h"
#include <QList>
namespace Ui { class GUIGeometryImprintSolidDialog; }
namespace Interface {
    class FITKAbsGeoCommand;
    class FITKGeoCommandList;
    class VirtualShape;
}
namespace Core
{
    class FITKActionOperator;
}
namespace GUI {
    /**
     * @brief  压印体
     * @author guqingtao (15598887859@163.com)
     * @date   2025-04-03
     */
    class PluginGeoAbstractAPI GUIGeometryImprintSolidDialog : public Core::FITKDialog
    {
        Q_OBJECT

    public:
        /**
         * @brief  构造
         * @author guqingtao (15598887859@163.com)
         * @date   2025-04-03
         */
        GUIGeometryImprintSolidDialog(Core::FITKActionOperator* oper,QWidget* parent = nullptr);
        /**
         * @brief    析构
         * @return   void
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-04-03
         */
        ~GUIGeometryImprintSolidDialog();
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
         * @brief    创建压印体
         * @return   void
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-04-03
         */
        void createImprintSolidData();
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
         * @brief    拾取体的按钮
         * @return   void
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-04-03
         */
        void on_solid_pick_clicked();
    private:
        /**
         * @brief  ui
         * @author guqingtao (15598887859@163.com)
         * @date   2025-04-03
         */
        Ui::GUIGeometryImprintSolidDialog *_ui{};
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
         * @brief  操作对象列表
         * @author guqingtao (15598887859@163.com)
         * @date   2025-04-03
         */
        QList<Interface::VirtualShape> _solidVirtualTopos{};
        /**
         * @brief  数据ID
         * @author guqingtao (15598887859@163.com)
         * @date   2025-04-03
         */
        int _objID = -1;
    };
}
#endif // GUIGEOMETRYIMPRINTSOLIDDIALOG_H
