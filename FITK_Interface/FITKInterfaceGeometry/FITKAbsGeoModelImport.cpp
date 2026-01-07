#include "FITKAbsGeoModelImport.h"

namespace Interface
{
    FITKGeoEnum::FITKGeometryComType FITKAbsGeoModelImport::getGeometryCommandType()
    {
        // 返回默认导入文件类型。
        return FITKGeoEnum::FGTImport;
    }
}

