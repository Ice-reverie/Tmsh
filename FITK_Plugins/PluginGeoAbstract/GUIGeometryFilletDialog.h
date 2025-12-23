/**********************************************************************
 * @file   GUIGeometryFilletDialog.h
 * @brief  创建倒圆
 * @author guqingtao (15598887859@163.com)
 * @date   2025-04-02
 *********************************************************************/
#ifndef GUIGEOMETRYFILLETDIALOG_H
#define GUIGEOMETRYFILLETDIALOG_H

#include "FITK_Kernel/FITKCore/FITKAbstractGUIDialog.h"
#include "GraphData/GraphDataPick/GraphDataPickInfo.h"
#include "PluginGeoAbstractAPI.h"
#include <QList>
namespace Ui { class GUIGeometryFilletDialog; }
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
     * @brief  创建倒圆
     * @author guqingtao (15598887859@163.com)
     * @date   2025-04-02
     */
    class PluginGeoAbstractAPI GUIGeometryFilletDialog : public Core::FITKDialog
    {
        Q_OBJECT

    public:
        /**
         * @brief  构造
         * @author guqingtao (15598887859@163.com)
         * @date   2025-04-02
         */
        GUIGeometryFilletDialog(Core::FITKActionOperator* oper,QWidget* parent = nullptr);
        /**
         * @brief    析构
         * @return   void
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-04-02
         */
        ~GUIGeometryFilletDialog();
    private:
        /**
         * @brief    初始化
         * @return   void
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-04-02
         */
        void init();
        /**
         * @brief    设置拾取方式
         * @param[i] objType 
         * @param[i] methodType 
         * @return   void
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-04-02
         */
        void enablePickSig(Graph::PickObjType objType, Graph::PickMethod methodType);
        /**
         * @brief    创建倒圆
         * @return   void
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-04-02
         */
        void createFilletData();
        /**
         * @brief    处理拾取到的数据
         * @return   void
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-04-02
         */
        void pickFinishedOper();
        /**
         * @brief    获取数据名称
         * @return   QString
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-04-02
         */
        QString getNameText();

    private slots:
        /**
         * @brief  OK按钮
         * @author guqingtao (15598887859@163.com)
         * @date   2025-04-02
         */
        void on_pushButton_OK_clicked();
        /**
         * @brief    取消按钮
         * @return   void
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-04-02
         */
        void on_pushButton_Cancel_clicked();
        /**
         * @brief    拾取倒圆边的按钮
         * @return   void
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-04-02
         */
        void on_edge_pick_clicked();
    private:
        /**
         * @brief  ui
         * @author guqingtao (15598887859@163.com)
         * @date   2025-04-02
         */
        Ui::GUIGeometryFilletDialog *_ui{};
        /**
         * @brief  几何数据对象
         * @author guqingtao (15598887859@163.com)
         * @date   2025-04-02
         */
        Interface::FITKAbsGeoCommand* _obj{};
        /**
         * @brief  操作器
         * @author guqingtao (15598887859@163.com)
         * @date   2025-04-02
         */
        Core::FITKActionOperator* _oper{};
        /**
         * @brief  几何数据管理器
         * @author guqingtao (15598887859@163.com)
         * @date   2025-04-02
         */
        Interface::FITKGeoCommandList* _geometryData{};
        /**
         * @brief  被倒圆的边虚拓扑列表
         * @author guqingtao (15598887859@163.com)
         * @date   2025-04-02
         */
        QList<Interface::VirtualShape> _edgeVirtualTopos{};
        /**
         * @brief  数据ID
         * @author guqingtao (15598887859@163.com)
         * @date   2025-04-02
         */
        int _objID = -1;
    };
}
#endif // GUIGEOMETRYFILLETDIALOG_H
