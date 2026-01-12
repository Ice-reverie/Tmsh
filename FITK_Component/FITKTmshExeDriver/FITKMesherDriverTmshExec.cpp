#include "FITKMesherDriverTmshExec.h"

#include "FITKTmshExecProgramInputInfo.h"
#include "FITKTmshExecProgramDriver.h"
#include "GUITmshSettings.h"

#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Kernel/FITKAppFramework/FITKProgramTaskManager.h"
#include "FITK_Kernel/FITKAppFramework/FITKMessage.h"
#include "FITK_Kernel/FITKCore/FITKDirFileTools.h"

#include "FITK_Interface/FITKInterfaceGeometry/FITKGeoInterfaceFactory.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoModelExportTopos.h"
#include "FITK_Interface/FITKInterfaceMeshGen/FITKMeshGenInterface.h"
#include "FITK_Interface/FITKInterfaceMeshGen/FITKGlobalMeshSizeInfo.h"
#include "FITK_Interface/FITKInterfaceMeshGen/FITKGlobalMeshGenerateAlgorithmInfo.h"
#include "FITKTmshGlobalMeshGenerateAlgorithmInfo.h"

#include <QDir>
#include <QCoreApplication>
#include <QFile>
#include <QFileInfo>

namespace Tmsh
{
    namespace
    {
        constexpr const char* kMesherKey = "TmshExec";

        QString tmshOutputDir()
        {
#ifdef Q_OS_LINUX
            // 与 FITKTmshExecProgramDriver 中的默认布局一致：applicationDirPath()/tmsh/
            return QDir::cleanPath(QCoreApplication::applicationDirPath() + QString("/tmsh"));
#else
            // 输出到 applicationDirPath() 上一级目录的 Tmsh 目录中
            return QDir::cleanPath(QCoreApplication::applicationDirPath() + QString("/../Tmsh"));
#endif
        }
    }

    FITKMesherDriverTmshExec::FITKMesherDriverTmshExec()
    {
    }

    FITKMesherDriverTmshExec::~FITKMesherDriverTmshExec()
    {
    }

    void FITKMesherDriverTmshExec::startMesher(QStringList info /*= QStringList()*/)
    {
        Q_UNUSED(info);
        _pipelineEnabled = false;

        int method = 0;
        if (!this->getValue("Method").isNull())
        {
            method = this->getValueT<int>("Method");
        }

        if (method == 0)
        {
            const int stage = this->getValue("RunStage").isNull() ? 1 : this->getValueT<int>("RunStage");
            this->buildSingleStageArgs();
        }
        else if (method == 1)
        {
            this->tmshMeshGenSetting();
        }
    }

    void FITKMesherDriverTmshExec::stopMesher(QStringList info /*= QStringList()*/)
    {
        Q_UNUSED(info);
        // TODO: stop logic if needed
    }

    void FITKMesherDriverTmshExec::tmshMeshGenSetting()
    {
        if (_settingsDialog)
        {
            _settingsDialog->raise();
            _settingsDialog->activateWindow();
            return;
        }

        _settingsDialog = new GUI::GUITmshSettings(this, FITKAPP->getGlobalData()->getMainWindow());
        connect(_settingsDialog, &QDialog::accepted, this, &FITKMesherDriverTmshExec::writerModelFileDriven);
        connect(_settingsDialog, &QObject::destroyed, this, [this]() { _settingsDialog = nullptr; });
        _settingsDialog->show();
    }

    void FITKMesherDriverTmshExec::writerModelFileDriven()
    {
        QVariant v = {};
        v = this->getValue("virtualTopos");
        QList<Interface::VirtualShape> topos = v.value<QList<Interface::VirtualShape>>();
        if (topos.size() == 0) return;

        const QString meshPath = tmshOutputDir();
        QDir meshDir(meshPath);
        if (!meshDir.exists())
        {
            meshDir.mkpath(meshPath);
        }

        const QString shapeFile = meshDir.filePath("geometryFile.brep");
        QFile::remove(shapeFile);

        // MeshFile 若调用方未提供，则给出默认输出路径
        if (this->getValue("MeshFile").isNull() || this->getValueT<QString>("MeshFile").isEmpty())
        {
            this->setValue("MeshFile", meshDir.filePath("mesh.mesh"));
        }
        // 若输出文件在 meshPath 下，移除旧文件避免干扰新结果（不要清空整个目录，避免误删可执行程序等）
        const QString meshFile = this->getValueT<QString>("MeshFile");
        if (!meshFile.isEmpty() && QFileInfo(meshFile).absolutePath() == meshDir.absolutePath())
        {
            QFile::remove(meshFile);
        }

        Interface::FITKMeshGenInterface* mf = Interface::FITKMeshGenInterface::getInstance();
        if (!mf) return;
        Interface::FITKGlobalMeshGenerateAlgorithmInfo* meshGenAlgInfo = mf->getGlobalMeshGenerateAlgorithmInfo(kMesherKey);
        if (!meshGenAlgInfo) return;

        Interface::FITKInterfaceGeometryFactory* geoFactory = Interface::FITKInterfaceGeometryFactory::getInstance();
        if (!geoFactory) return;
        Interface::FITKAbsGeoModelExportTopos* geoExport = dynamic_cast<Interface::FITKAbsGeoModelExportTopos*>(
            geoFactory->createCommand(Interface::FITKGeoEnum::FITKGeometryComType::FGTExportTopos));
        if (!geoExport) return;

        if (meshGenAlgInfo->getMeshGenerateDimension() == 2)
        {
            geoExport->setEnableStitch(meshGenAlgInfo->getGeometryStitch());
        }
        geoExport->setExportTopos(topos);
        geoExport->setFileName(shapeFile);
        if (geoExport->update() == false) return;

        this->setValue("ShapeFile", shapeFile);
        this->startTmshMeshGenerationOnce();
    }

    void FITKMesherDriverTmshExec::startTmshMeshGenerationOnce()
    {
        QStringList args = this->buildSingleStageArgs();
        if (args.isEmpty()) { emit mesherFinished(); return; }

        TmshExe::FITKTmshExecProgramInputInfo* inputInfo =
            new TmshExe::FITKTmshExecProgramInputInfo;
        inputInfo->setArgs(args);

        AppFrame::FITKProgramTaskManeger* mgr = FITKAPP->getProgramTaskManager();
        TmshExe::FITKTmshExecProgramDriver* exe =
            dynamic_cast<TmshExe::FITKTmshExecProgramDriver*>(
                mgr->createProgram(1, "FITKTmshExecProgramDriver", inputInfo));
        if (!exe || !exe->isExistExeProgram()) { emit mesherFinished(); return; }

        connect(exe, &TmshExe::FITKTmshExecProgramDriver::sig_Finish,
            this, &FITKMesherDriverTmshExec::onSingleStageFinished);
        exe->start();
    }

    QStringList FITKMesherDriverTmshExec::buildSingleStageArgs() const
    {
        QStringList args;
        const QString shapeFile = this->getValueT<QString>("ShapeFile");
        const QString meshFile = this->getValueT<QString>("MeshFile");
        if (shapeFile.isEmpty() || meshFile.isEmpty()) return {};

        args << "--in" << shapeFile
            << "--out" << meshFile;

        auto* algInfo = Interface::FITKMeshGenInterface::getInstance()
            ->getGlobalMeshGenerateAlgorithmInfo(kMesherKey);
        if (!algInfo) return args;

        int dim = algInfo->getMeshGenerateDimension();
        args << "--mesh_dim" << QString::number(dim);

        auto* tmshAlg = dynamic_cast<Tmsh::FITKTmshGlobalMeshGenerateAlgorithmInfo*>(algInfo);
        if (!tmshAlg) return args;

        if (tmshAlg->hasTangencyTolerance())
            args << "--tangent_tol" << QString::number(tmshAlg->getTangencyTolerance(), 'g', 16);
        if (tmshAlg && tmshAlg->hasMinLength())
        {
            args << "--min_length" << QString::number(tmshAlg->getMinLength(), 'g', 16);
        }
        if (tmshAlg && tmshAlg->hasMaxLength())
        {
            args << "--max_length" << QString::number(tmshAlg->getMaxLength(), 'g', 16);
        }
        if (tmshAlg && tmshAlg->hasMeshDim())
        {
            args << "--mesh_dim" << QString::number(tmshAlg->getMeshDim(), 'g', 16);
        }
        if (tmshAlg && tmshAlg->hasTetgenSwitches())
        {
            args << "--tetgen_switches" << QString::number(tmshAlg->getTetgenSwitches(), 'g', 16);
        }
        if (tmshAlg && tmshAlg->hasNormalTol())
        {
            args << "--normal_tol" << QString::number(tmshAlg->getNormalTol(), 'g', 16);
        }
        if (tmshAlg && tmshAlg->hasMinAngle())
        {
            args << "--min_angle" << QString::number(tmshAlg->getMinAngle(), 'g', 16);
        }
        if (tmshAlg && tmshAlg->hasUseMultiThreading())
        {
            args << "--use_multi_threading" << QString::number(tmshAlg->getUseMultiThreading(), 'g', 16);
        }
        if (tmshAlg && tmshAlg->hasRefineIter())
        {
            args << "--refine_iter" << QString::number(tmshAlg->getRefineIter(), 'g', 16);
        }
        if (tmshAlg && tmshAlg->hasRefineSmoothIter())
        {
            args << "--refine_smooth_iter" << QString::number(tmshAlg->getRefineSmoothIter(), 'g', 16);
        }
        if (tmshAlg && tmshAlg->hasAdaptIter())
        {
            args << "--adapt_iter" << QString::number(tmshAlg->getAdaptIter(), 'g', 16);
        }
        if (tmshAlg && tmshAlg->hasAdaptSmoothIter())
        {
            args << "--adapt_smooth_iter" << QString::number(tmshAlg->getAdaptSmoothIter(), 'g', 16);
        }

        return args;
    }

    void FITKMesherDriverTmshExec::onSingleStageFinished()
    {
        emit mesherFinished();
    }


}
