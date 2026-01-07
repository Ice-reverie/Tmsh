#include "FITKAbsGeoInstance.h" 

// Global data
#include "FITK_Kernel/FITKCore/FITKDataRepo.h"

// Command
#include "FITKAbsGeoPart.h"

namespace Interface
{
    // 获取数据类型。
    FITKGeoEnum::FITKGeometryComType FITKAbsGeoInstance::getGeometryCommandType()
    {
        return FITKGeoEnum::FITKGeometryComType::FGTInstance;
    }

    bool FITKAbsGeoInstance::update()
    {
        return true;
    }

    void FITKAbsGeoInstance::setPartCommandID(int id)
    {
        // 保存命令ID。
        m_partCmdId = id;
    }

    int FITKAbsGeoInstance::getPartCommandID()
    {
        return m_partCmdId;
    }

    FITKAbsGeoPart* FITKAbsGeoInstance::getPartCommand()
    {
        // 将ID转化为数据对象。
        return FITKDATAREPO->getTDataByID<FITKAbsGeoPart>(m_partCmdId);
    }
}