#include "FITKOCCReferencePoint.h"
#include <BRepBuilderAPI_MakeVertex.hxx>
#include <gp_Pnt.hxx>

namespace OCC {
    FITKOCCReferencePoint::FITKOCCReferencePoint() :OCCShapeAgent(this)
    {
        _shapeAgent = _occShapeAgent;
    }
    bool FITKOCCReferencePoint::update()
    {
        BRepBuilderAPI_MakeVertex vertex(gp_Pnt(m_pos[0], m_pos[1], m_pos[2]));
        _occShapeAgent->updateShape(vertex);
        return true;
    }

    Interface::FITKGeoEnum::FITKDatumType FITKOCCReferencePoint::getDatumType()
    {
        return Interface::FITKGeoEnum::FITKDatumType::FDTPoint;
    }
}

