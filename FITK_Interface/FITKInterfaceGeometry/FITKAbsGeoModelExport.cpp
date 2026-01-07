#include "FITKAbsGeoModelExport.h"

namespace Interface
{

    FITKAbsGeoModelExport::~FITKAbsGeoModelExport()
    {

    }

    void FITKAbsGeoModelExport::setFileName(const QString& fileName)
    {
        _exportFileName = fileName;
    }

    QString FITKAbsGeoModelExport::getFileName() const
    {
        return _exportFileName;
    }

    void FITKAbsGeoModelExport::addExportCommandID(int id)
    {
        if (_exportIds.contains(id))
        {
            return;
        }

        _exportIds.push_back(id);
    }

    void FITKAbsGeoModelExport::removeExportCommandID(int id)
    {
        _exportIds.removeOne(id);
    }

    FITKGeoEnum::FITKGeometryComType FITKAbsGeoModelExport::getGeometryCommandType()
    {
        return FITKGeoEnum::FITKGeometryComType::FGTExport;
    }

}