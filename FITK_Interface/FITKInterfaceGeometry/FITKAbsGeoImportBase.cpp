#include "FITKAbsGeoImportBase.h"

namespace Interface
{
    void FITKAbsGeoImportBase::setFileName(const QString& fileName)
    {
        // 保存文件名称。
        _importFileName = fileName;
    }

    QString FITKAbsGeoImportBase::getFileName() const
    {
        // 获取文件名称。
        return _importFileName;
    }
}

