#include "FITKAbsGeoModelOperImprintSolid.h"
#include "FITK_Interface/FITKInterfaceModel/FITKAbstractModel.h"

namespace Interface
{

    FITKGeoEnum::FITKGeometryComType FITKAbsGeoModelOperImprintSolid::getGeometryCommandType()
    {
        return FITKGeoEnum::FITKGeometryComType::FGTImprintSolid;
    }


    void FITKAbsGeoModelOperImprintSolid::add(VirtualShape geoCommandShape)
    {
        m_tempVShapes.append(geoCommandShape);
    }

    void FITKAbsGeoModelOperImprintSolid::set(VirtualShape geoCommandShape)
    {
        m_tempVShapes.clear();
        add(geoCommandShape);

    }

    void FITKAbsGeoModelOperImprintSolid::add(QList<VirtualShape> geoCommandShapes)
    {
        m_tempVShapes.append(geoCommandShapes);
    }

    void FITKAbsGeoModelOperImprintSolid::set(QList<VirtualShape> geoCommandShapes)
    {
        m_tempVShapes = geoCommandShapes;
    }

    void FITKAbsGeoModelOperImprintSolid::remove(int index)
    {
        m_tempVShapes.removeAt(index);
    }

    void FITKAbsGeoModelOperImprintSolid::clear()
    {
        m_tempVShapes.clear();
    }

    void FITKAbsGeoModelOperImprintSolid::setType(GeoSolidOperType t)
    {
        _faceOperType = t;
    }

    FITKAbsGeoModelOperImprintSolid::GeoSolidOperType FITKAbsGeoModelOperImprintSolid::getType()
    {
        return _faceOperType;
    }

    QList<VirtualShape> FITKAbsGeoModelOperImprintSolid::getVShapes()
    {
        return m_tempVShapes;
    }

    QList<int> FITKAbsGeoModelOperImprintSolid::getModelOperFacesID()
    {
        QList<int> _geoCommands;

        for (int i = 0; i < m_tempVShapes.size(); i++)
        {
            _geoCommands.append(m_tempVShapes[i].CmdId);
        }
        return _geoCommands;
    }

    void FITKAbsGeoModelOperImprintSolid::setTolerance(double tolerance)
    {
        _tolerance = tolerance;
    }

    double FITKAbsGeoModelOperImprintSolid::getTolerance()
    {
        return _tolerance;
    }
}
