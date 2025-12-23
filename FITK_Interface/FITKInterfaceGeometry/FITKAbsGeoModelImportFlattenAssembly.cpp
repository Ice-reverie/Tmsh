#include "FITKAbsGeoModelImportFlattenAssembly.h"

namespace Interface
{
    FITKGeoEnum::FITKGeometryComType FITKAbsGeoModelImportFlattenAssembly::getGeometryCommandType()
    {
        // 获取几何命令类型。
        return FITKGeoEnum::FGTImportFlattenAssembly;
    }
}