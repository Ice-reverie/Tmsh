/*****************************************************************//**
 * @file    FITKAbsGeoModelExtractCenter.cpp
 * @brief   Extract Center Command Class Implementation
 * @date    2025-03-27
 * @author  libaojun
 *********************************************************************/

#include "FITKAbsGeoModelExtractCenter.h"

namespace Interface
{
    FITKAbsGeoModelExtractCenter::~FITKAbsGeoModelExtractCenter()
    {
    }

    FITKGeoEnum::FITKGeometryComType FITKAbsGeoModelExtractCenter::getGeometryCommandType()
    {
        return FITKGeoEnum::FITKGeometryComType::FGTExtractCenter;
    }

    void FITKAbsGeoModelExtractCenter::setShape(const VirtualShape& shape)
    {
        _shape = shape;
    }

    VirtualShape FITKAbsGeoModelExtractCenter::getShape() const
    {
        return _shape;
    }

    void FITKAbsGeoModelExtractCenter::setShapeType(ShapeType type)
    {
        _shapeType = type;
    }

    FITKAbsGeoModelExtractCenter::ShapeType FITKAbsGeoModelExtractCenter::getShapeType() const
    {
        return _shapeType;
    }

    void FITKAbsGeoModelExtractCenter::setCenter(const double* center)
    {
        _center[0] = center[0];
        _center[1] = center[1];
        _center[2] = center[2];
    }

    void FITKAbsGeoModelExtractCenter::getCenter(double* center) const
    {
        center[0] = _center[0];
        center[1] = _center[1];
        center[2] = _center[2];
    }

    
}


