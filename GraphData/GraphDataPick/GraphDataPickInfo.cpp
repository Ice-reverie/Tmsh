#include "GraphDataPickInfo.h"

#include "OperatorsInterface/GraphInteractionOperator.h"

#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Kernel/FITKAppFramework/FITKRunTimeSetting.h"
#include "FITK_Kernel/FITKCore/FITKOperatorRepo.h"

namespace Graph
{
    void GraphDataPickInfo::ClearPickInfo()
    {
        auto RTsetting = FITKAPP->getGlobalData()->getRunTimeSetting();
        if (!RTsetting)
        {
            return;
        }

        // 从运行时设置中获取变量值。
        PickObjType pot = (PickObjType)RTsetting->getValueT<int>(PickObjTypeStr);
        PickMethod pm = (PickMethod)RTsetting->getValueT<int>(PickMethodStr);

        // 移除运行参数。
        RTsetting->removeVarient(PickObjTypeStr);
        RTsetting->removeVarient(PickMethodStr);
        RTsetting->removeVarient(AutoClearStr);

        // 关闭拾取方法。
        EventOper::GraphInteractionOperator* operPick = Core::FITKOperatorRepo::getInstance()->
            getOperatorT<EventOper::GraphInteractionOperator>("GraphPick");
        if (operPick)
        {
            // 修改对象可拾取性。（性能优化，防止状态销毁重复遍历）
            if (pot != PickObjType::PickNone || pm != PickMethod::PMNone)
            {
                operPick->setActorStateByPickInfo((int)PickObjType::PickNone, (int)PickMethod::PMNone);
            }

            // 清除拾取高亮效果。
            operPick->clear();
        }
    }

    void GraphDataPickInfo::SetPickInfo(GraphDataPickInfoStru info, int dataObjId)
    {
        AppFrame::FITKRunTimeSetting* RTsetting = FITKAPP->getGlobalData()->getRunTimeSetting();
        if (RTsetting == nullptr){
            return;
        }

        // 设置运行参数。
        RTsetting->setValue(PickObjTypeStr, (int)info._pickObjType);
        RTsetting->setValue(PickMethodStr, (int)info._pickMethod);
        RTsetting->setValue(AutoClearStr, (bool)info._autoClear);

        // 开启拾取方法。
        EventOper::GraphInteractionOperator* operPick = Core::FITKOperatorRepo::getInstance()->getOperatorT<EventOper::GraphInteractionOperator>("GraphPick");
        if (operPick){
            // 清除拾取高亮效果。
            operPick->setActorStateByPickInfo((int)info._pickObjType, (int)info._pickMethod, dataObjId);
        }
    }

    GraphDataPickInfoStru GraphDataPickInfo::GetPickInfo()
    {
        GraphDataPickInfoStru pickInfo;

        auto RTsetting = FITKAPP->getGlobalData()->getRunTimeSetting();
        if (!RTsetting)
        {
            return pickInfo;
        }

        // 从运行时设置中获取变量值。
        pickInfo._pickObjType = (PickObjType)RTsetting->getValueT<int>(PickObjTypeStr);
        pickInfo._pickMethod = (PickMethod)RTsetting->getValueT<int>(PickMethodStr);
        pickInfo._autoClear = RTsetting->getValueT<bool>(AutoClearStr);

        return pickInfo;
    }
}

