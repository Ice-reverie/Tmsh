#include "OperatorsGeoGroup.h"

#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKGeoCommandList.h"
#include "GUIDialog/GUIGeometryGroupDialog.h"
#include <QTreeWidgetItem>


namespace ModelOper
{
    bool OperatorsGeoGroup::execGUI()
    {
        //获取几何命令数据
        Interface::FITKAbsGeoCommand* geoCommand = this->getGeometryCommand();
        if (!geoCommand) return false;
        //创建分组界面
        GUI::GUIGeometryGroupDialog* dialog = new GUI::GUIGeometryGroupDialog(this, geoCommand, FITKAPP->getGlobalData()->getMainWindow());
        dialog->show();
        return true;
    }

    bool OperatorsGeoGroup::execProfession()
    {
        return true;
    }

    Interface::FITKAbsGeoCommand* OperatorsGeoGroup::getGeometryCommand()
    {
        if (!_emitter) return nullptr;
        int geoComID = -1;
        //获取几何命令的ID
        if (_emitter->objectName() == "actionTreeGroupModel")
        {
            //显示
            void* pItem = nullptr;
            this->argValue<void*>("SelectedItems", pItem);
            this->clearArgs();
            //获取数据
            QList<QTreeWidgetItem*>* itemsList = (QList<QTreeWidgetItem*>*)(pItem);
            if (itemsList->isEmpty()) return nullptr;
            geoComID = itemsList->at(0)->data(0, Qt::UserRole).toInt();
        }
        if (geoComID == -1) return nullptr;
        //获取几何命令管理器
        Interface::FITKGeoCommandList* geometryData = FITKAPP->getGlobalData()->getGeometryData<Interface::FITKGeoCommandList>();
        if (geometryData == nullptr) return nullptr;
        //获取几何命令数据
        return geometryData->getDataByID(geoComID);
    }
}

