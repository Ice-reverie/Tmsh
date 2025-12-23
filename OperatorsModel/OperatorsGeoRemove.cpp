#include "OperatorsGeoRemove.h"

#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKGeoCommandList.h"
#include "ModelData/GeometryManager.h"
#include "OperatorsInterface/GraphEventOperator.h"
#include "GUIFrame/MainWindow.h"
#include "GUIFrame/ControlPanel.h"
#include "GUIWidget/GUIEnumType.h"
#include <QTreeWidgetItem>

namespace ModelOper
{
    bool OperatorsGeoRemove::execGUI()
    {
        _geoId = this->getSelectedGeoId();
        return _geoId >= 0;
    }

    bool OperatorsGeoRemove::execProfession()
    {
        if (_geoId < 0) return false;

        // 删除几何数据
        Interface::FITKGeoCommandList* geometryData = FITKAPP->getGlobalData()->getGeometryData<ModelData::GeometryManager>();
        if (geometryData == nullptr) return false;
        geometryData->removeDataByID(_geoId);

        // 更新渲染
        if (auto graphOper = FITKOPERREPO->getOperatorT<EventOper::GraphEventOperator>("GraphPreprocess")) {
            graphOper->updateGraph(_geoId, true);
            graphOper->reRender();
        }

        // 刷新树
        GUI::MainWindow* mw = GUI::MainWindow::GetMainWindowFromFramework();
        if (mw) {
            GUI::ControlPanel* cp = mw->getControlPanel();
            if (cp) {
                GUI::TreeWidgetPage* tw = cp->getTreeWidget(GUI::TreeWidgetType::PageModel);
                if (tw) {
                    tw->updatePage();
                }
            }
        }
        return true;
    }

    int OperatorsGeoRemove::getSelectedGeoId()
    {
        if (_emitter == nullptr) return -1;
        void* pItem = nullptr;
        this->argValue<void*>("SelectedItems", pItem);
        this->clearArgs();
        QList<QTreeWidgetItem*>* itemsList = reinterpret_cast<QList<QTreeWidgetItem*>*>(pItem);
        if (itemsList == nullptr || itemsList->isEmpty()) return -1;
        return itemsList->at(0)->data(0, Qt::UserRole).toInt();
    }
}
