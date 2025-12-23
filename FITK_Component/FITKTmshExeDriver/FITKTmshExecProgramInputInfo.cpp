#include "FITKTmshExecProgramInputInfo.h"

namespace TmshExe
{
    FITKTmshExecProgramInputInfo::FITKTmshExecProgramInputInfo()
    {
    }

    FITKTmshExecProgramInputInfo::~FITKTmshExecProgramInputInfo()
    {
    }

    void FITKTmshExecProgramInputInfo::setArgs(const QStringList& args)
    {
        _args = args;
    }

    void FITKTmshExecProgramInputInfo::clearArgs()
    {
        _args.clear();
    }

    QStringList FITKTmshExecProgramInputInfo::args()
    {
        return _args;
    }

    void FITKTmshExecProgramInputInfo::setMonitorFiles(QStringList& monitorFiles)
    {
        Q_UNUSED(monitorFiles);
    }

    void FITKTmshExecProgramInputInfo::setMonitorPath(QStringList& monitorpath)
    {
        Q_UNUSED(monitorpath);
    }
}

