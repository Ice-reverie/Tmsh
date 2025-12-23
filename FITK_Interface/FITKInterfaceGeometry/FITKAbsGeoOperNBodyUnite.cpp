#include "FITKAbsGeoOperNBodyUnite.h"

namespace Interface
{
    const QList<VirtualShape> & FITKAbsGeoOperNBodyUnite::getShapes()
    {
        // 获取形状列表。
        return m_shapes;
    }

    void FITKAbsGeoOperNBodyUnite::addShape(VirtualShape & shape)
    {
        // 判断拓扑类型。
        if (shape.Type != Interface::FITKGeoEnum::VSSolid)
        {
            return;
        }

        // 去除重复。
        for (const VirtualShape & iShape : m_shapes)
        {
            if (iShape.CmdId == shape.CmdId &&
                (iShape.VirtualTopoId == shape.VirtualTopoId || 
                iShape.VirtualTopoIndex == shape.VirtualTopoIndex))
            {
                return;
            }
        }

        m_shapes.push_back(shape);
    }

    void FITKAbsGeoOperNBodyUnite::clearShapes()
    {
        m_shapes.clear();
    }

    FITKGeoEnum::FITKGeometryComType FITKAbsGeoOperNBodyUnite::getGeometryCommandType()
    {
        // 获取几何命令类型。
        return FITKGeoEnum::FGTNBodyUnite;
    }
}