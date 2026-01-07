#include "FITKTmshExecProgramDriver.h"

#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKMessage.h"

#include <QCoreApplication>
#include <QDir>
#include <QFileInfo>
#include <QDebug>

namespace TmshExe
{
    FITKTmshExecProgramDriver::FITKTmshExecProgramDriver()
    {
#ifdef Q_OS_LINUX
        QString solverPath = QCoreApplication::applicationDirPath() + QString("/tmsh/tmsh");
#else
        QString solverPath = QDir::cleanPath(QCoreApplication::applicationDirPath() + QString("/../Tmsh/g3d_test.exe"));
#endif

        if (!QFile::exists(solverPath))
        {
            AppFrame::FITKMessageError(QString("FITKTmshExecProgramDriver: tmsh executable not found : %1").arg(solverPath));
            qDebug() << QString("FITKTmshExecProgramDriver: tmsh executable not found : %1").arg(solverPath);
        }

        this->setExecProgram(solverPath);

        // 工作目录改为可执行程序所在目录（Windows: applicationDirPath()/../Tmsh）
        const QString workDir = QFileInfo(solverPath).absolutePath();
        this->setWorkingDirectory(workDir);
    }

    FITKTmshExecProgramDriver::~FITKTmshExecProgramDriver()
    {
    }

    bool FITKTmshExecProgramDriver::isExistExeProgram()
    {
        return QFileInfo(this->getExecProgram()).isFile();
    }

    int FITKTmshExecProgramDriver::getProgramType()
    {
        return 1;
    }

    QString FITKTmshExecProgramDriver::getProgramName()
    {
        return "FITKTmshExecProgramDriver";
    }
}
