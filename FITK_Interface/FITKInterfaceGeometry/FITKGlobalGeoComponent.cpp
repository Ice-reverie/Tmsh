#include "FITKGlobalGeoComponent.h"

namespace Interface
{
    FITKGlobalGeoComponent::FITKGlobalGeoComponent(const Interface::FITKModelEnum::FITKModelSetType type) :
        m_geoType(type)
    {
        // 创建指定类型几何分组数据。
    }
}
