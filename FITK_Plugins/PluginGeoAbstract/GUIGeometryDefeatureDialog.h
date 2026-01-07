/**********************************************************************
 * @file   GUIGeometryDefeatureDialog.h
 * @brief  删除特征
 * @author guqingtao (15598887859@163.com)
 * @date   2025-04-02
 *********************************************************************/
#ifndef GUIGEOMETRYDEFEATUREDIALOG_H
#define GUIGEOMETRYDEFEATUREDIALOG_H

#include "FITK_Kernel/FITKCore/FITKAbstractGUIDialog.h"
#include "GraphData/GraphDataPick/GraphDataPickInfo.h"
#include "PluginGeoAbstractAPI.h"
#include <QList>
namespace Ui { class GUIGeometryDefeatureDialog; }
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
     * @brief  删除特征
     * @author guqingtao (15598887859@163.com)
     * @date   2025-04-02
     */
    class PluginGeoAbstractAPI GUIGeometryDefeatureDialog : public Core::FITKDialog
    {
        Q_OBJECT

    public:
        /**
         * @brief  构造
         * @author guqingtao (15598887859@163.com)
         * @date   2025-04-02
         */
        GUIGeometryDefeatureDialog(Core::FITKActionOperator* oper,QWidget* parent = nullptr);
        /**
         * @brief    析构
         * @return   void
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-04-02
         */
        ~GUIGeometryDefeatureDialog();
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
         * @brief    删除特征
         * @return   void
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-04-02
         */
        void createDefeatureData();
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
         * @brief    拾取特征移除面的按钮
         * @return   void
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-04-02
         */
        void on_face_pick_clicked();
    private:
        /**
         * @brief  ui
         * @author guqingtao (15598887859@163.com)
         * @date   2025-04-02
         */
        Ui::GUIGeometryDefeatureDialog *_ui{};
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
         * @brief  被删除特征的面虚拓扑列表
         * @author guqingtao (15598887859@163.com)
         * @date   2025-04-02
         */
        QList<Interface::VirtualShape> _faceVirtualTopos{};
        /**
         * @brief  数据ID
         * @author guqingtao (15598887859@163.com)
         * @date   2025-04-02
         */
        int _objID = -1;
    };
}
#endif // GUIGEOMETRYDEFEATUREDIALOG_H
