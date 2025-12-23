#include "FITKGmshExecProgramDriver.h"
#include "FITKGmshGeoScriptWrite.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppSettings.h"
#include "FITK_Kernel/FITKAppFramework/FITKMessage.h"
#include <QDir>
#include <QDebug>

namespace GmshExe
{
    FITKGmshExecProgramDriver::FITKGmshExecProgramDriver()
    {
        //获取求解器路径
        QString solverPath = QCoreApplication::applicationDirPath() + QString("/gmsh/gmsh.exe");
        if (!QFile::exists(solverPath))
        {
            AppFrame:: FITKMessageError(QString("FITKGmshExecProgramDriver: gmsh executable not found : % 1").arg(solverPath));
            qDebug() << QString("FITKGmshExecProgramDriver: gmsh executable not found : % 1").arg(solverPath);
        }

        this->setExecProgram(solverPath);

        //设置工作路径
        QString workDir = FITKAPP->getTempDir(false, "Gmsh");
        this->setWorkingDirectory(workDir);
    }

    FITKGmshExecProgramDriver::~FITKGmshExecProgramDriver()
    {

    }

    bool FITKGmshExecProgramDriver::isExistExeProgram()
    {
        return QFileInfo(this->getExecProgram()).isFile();
    }

    int FITKGmshExecProgramDriver::getProgramType()
    {
        return 1;
    }

    QString FITKGmshExecProgramDriver::getProgramName()
    {
        return "FITKGmshExecProgramDriver";
    }

    bool FITKGmshExecProgramDriver::writeConfigFile(QString inputFile, QString geoScriptFile, QString meshFile)
    {
        //写出geo脚本文件
        QFile::remove(geoScriptFile);
        //导出的网格文件
        QFile::remove(meshFile);
        FITKGmshGeoScriptWrite geoWrite;
        geoWrite.setFileName(geoScriptFile);
        geoWrite.setShapeFromFile(inputFile);
        geoWrite.setMeshFile(meshFile);
        geoWrite.run();
        //判断是否写出成功
        QFileInfo geoScriptFileInfo(geoScriptFile);
        if (!geoScriptFileInfo.isFile()) return false;
        return true;
    }
}
