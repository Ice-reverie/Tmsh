#include "FITKAbsGeoSampleCurve.h"

namespace Interface
{
    FITKAbsGeoSampleCurve::~FITKAbsGeoSampleCurve()
    {
        // 析构函数的具体实现（如果有需要的话）
    }

    void FITKAbsGeoSampleCurve::setSampleMethod(SampleMethod method)
    {
        _sampleMethod = method;
    }

    FITKAbsGeoSampleCurve::SampleMethod FITKAbsGeoSampleCurve::getSampleMethod() const
    {
        return _sampleMethod;
    }

    void FITKAbsGeoSampleCurve::setSamplePointCount(int count)
    {
        _samplePointCount = count;
    }

    int FITKAbsGeoSampleCurve::getSamplePointCount() const
    {
        return _samplePointCount;
    }

    void FITKAbsGeoSampleCurve::setSampleCurve(const VirtualShape& curve)
    {
        _sampleCurve = curve;
    }

    VirtualShape FITKAbsGeoSampleCurve::getSampleCurve() const
    {
        return _sampleCurve;
    }

    FITKGeoEnum::FITKGeometryComType FITKAbsGeoSampleCurve::getGeometryCommandType()
    {
        // 返回几何命令类型，具体实现根据实际需求进行修改
        return FITKGeoEnum::FITKGeometryComType::FGTSampleCurve; // 替换为实际的几何命令类型
    }
}



