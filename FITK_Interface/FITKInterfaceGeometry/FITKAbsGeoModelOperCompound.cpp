#include "FITKAbsGeoModelOperCompound.h"

namespace Interface
{
    // 复合体添加对象。
    //@{
    FITKGeoEnum::FITKGeometryComType FITKAbsGeoModelOperCompound::getGeometryCommandType()
    {
        return FITKGeoEnum::FITKGeometryComType::FGTCompoundAppendShape;
    }

    void FITKAbsGeoModelOperCompound::setInputCmdId(Interface::VirtualShape cmdId)
    {
        m_solidCmdId = cmdId;
    }

    Interface::VirtualShape& FITKAbsGeoModelOperCompound::getInputCmdId()
    {
        return m_solidCmdId;
    }

    void FITKAbsGeoModelOperCompound::setShapeCmdIds(QList<Interface::VirtualShape> solidCmdIds)
    {
        m_solidCmdIds = solidCmdIds;
    }

    QList<Interface::VirtualShape>& FITKAbsGeoModelOperCompound::getShapeCmdIds()
    {
        return m_solidCmdIds;
    }
    //@}
}
