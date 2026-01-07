#include "FITKAbsGeoImportedPart.h" 

// Global data
#include "FITK_Kernel/FITKCore/FITKDataRepo.h"

namespace Interface
{
    // 获取数据类型。
    FITKGeoEnum::FITKGeometryComType FITKAbsGeoImportedPart::getGeometryCommandType()
    {
        return FITKGeoEnum::FITKGeometryComType::FGTImportPart;
    }
}