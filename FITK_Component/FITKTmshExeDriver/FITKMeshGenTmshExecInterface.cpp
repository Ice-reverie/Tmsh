#include "FITKMeshGenTmshExecInterface.h"
#include "FITKMeshGenerateProcessorTmshExec.h"
#include "FITKMeshSizeGeneratorTmshExec.h"
#include "FITKMeshAlgorithmGeneratorTmshExec.h"
#include "FITKMesherDriverTmshExec.h"

#include "FITK_Interface/FITKInterfaceMeshGen/FITKMeshGenInterface.h"
#include "FITK_Interface/FITKInterfaceMeshGen/FITKZonePoints.h"
#include "FITK_Interface/FITKInterfaceMeshGen/FITKGlobalMeshSizeInfo.h"
#include "FITK_Interface/FITKInterfaceMeshGen/FITKGlobalMeshGenerateAlgorithmInfo.h"
#include "FITK_Kernel/FITKCore/FITKDirFileTools.h"
#include "FITK_Kernel/FITKCore/FITKLibInfo.h"

#include <QApplication>
#include <QFile>

FITKLIBINFOREGISTER(FITKTmshExeDriver, FITKTMSHEXEDRIVERVERSION);

namespace Tmsh
{
    namespace
    {
        constexpr const char* kMesherKey = "TmshExec";
    }

    FITKMeshGenTmshExecInterface::FITKMeshGenTmshExecInterface()
    {
        // 确保可执行程序目录存在（由发布包提供 tmsh 可执行文件）
        QString tmshExecPath = QApplication::applicationDirPath() + QString("/../Tmsh");
        if (!Core::CreateDir(tmshExecPath))
        {
            qWarning("FITKMeshGenTmshExecInterface: Failed to create directory %s", qPrintable(tmshExecPath));
        }

        //获取工厂
        Interface::FITKMeshGenInterface* mf = Interface::FITKMeshGenInterface::getInstance();
        if (nullptr == mf) return;

        mf->regMeshSizeGenerator(new FITKMeshSizeGeneratorTmshExec, kMesherKey);
        mf->regMesherDriver(new FITKMesherDriverTmshExec, kMesherKey);
        mf->regMeshProcessor(new FITKMeshGenerateProcessorTmshExec, kMesherKey);
        mf->regMeshAlgorithmGenerator(new FITKMeshAlgorithmGeneratorTmshExec, kMesherKey);
    }


    QString FITKMeshGenTmshExecInterface::getComponentName()
    {
        return QString("FITKMeshGenTmshExec");
    }

    bool FITKMeshGenTmshExecInterface::exec(const int indexPort)
    {
        Q_UNUSED(indexPort);
        return false;
    }

    QList<int> FITKMeshGenTmshExecInterface::getUnResetDataObj()
    {
        QList<int> ids;
        Interface::FITKMeshGenInterface* mgInter = Interface::FITKMeshGenInterface::getInstance();
        if (!mgInter)
        {
            return ids;
        }

        Interface::FITKZonePointManager* zptMgr = mgInter->getZonePointManager(kMesherKey);
        if (zptMgr)
        {
            ids.push_back(zptMgr->getDataObjectID());
        }

        Interface::FITKGlobalMeshSizeInfo* msi = mgInter->getGlobalMeshSizeInfo(kMesherKey);
        if (msi)
        {
            ids.push_back(msi->getDataObjectID());
        }

        Interface::FITKGlobalMeshGenerateAlgorithmInfo* mai = mgInter->getGlobalMeshGenerateAlgorithmInfo(kMesherKey);
        if (mai)
        {
            ids.push_back(mai->getDataObjectID());
        }

        return ids;
    }
}

