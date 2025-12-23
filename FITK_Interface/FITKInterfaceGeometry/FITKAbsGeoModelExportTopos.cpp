#include "FITKAbsGeoModelExport.h"
#include "FITKAbsGeoModelExportTopos.h"

namespace Interface
{
    FITKAbsGeoModelExportTopos::FITKAbsGeoModelExportTopos()
    {

    }

    FITKAbsGeoModelExportTopos::~FITKAbsGeoModelExportTopos()
    {

    }

    void FITKAbsGeoModelExportTopos::setFileName(const QString& fileName)
    {
        _exportFileName = fileName;
    }

    QString FITKAbsGeoModelExportTopos::getFileName() const
    {
        return _exportFileName;
    }

    void FITKAbsGeoModelExportTopos::setEnableStitch(bool enabled)
    {
        _enableStitch = enabled;
    }

    bool FITKAbsGeoModelExportTopos::getEnableStitch()
    {
        return _enableStitch;
    }

    void FITKAbsGeoModelExportTopos::setStitchTolerance(double tol)
    {
        _stitchTol = tol;
    }

    double FITKAbsGeoModelExportTopos::getStitchTolerance()
    {
        return _stitchTol;
    }

    void FITKAbsGeoModelExportTopos::setExportTopos(QList<VirtualShape> edgeTopos)
    {
        // 替换列表。
        _exportTopos = edgeTopos;
    }

    const QList<VirtualShape> & FITKAbsGeoModelExportTopos::getExportTopos()
    {
        // 获取导出拓扑列表。
        return _exportTopos;
    }

    void FITKAbsGeoModelExportTopos::appendExportTopo(const VirtualShape & topo)
    {
        // 去除重复。
        for (const VirtualShape & iShape : _exportTopos)
        {
            if (iShape.CmdId == topo.CmdId &&
                (iShape.VirtualTopoId == topo.VirtualTopoId ||
                    iShape.VirtualTopoIndex == topo.VirtualTopoIndex))
            {
                return;
            }
        }

        _exportTopos.push_back(topo);
    }

    void FITKAbsGeoModelExportTopos::clearExportTopos()
    {
        _exportTopos.clear();
    }

    FITKGeoEnum::FITKGeometryComType FITKAbsGeoModelExportTopos::getGeometryCommandType()
    {
        return FITKGeoEnum::FITKGeometryComType::FGTExportTopos;
    }
}