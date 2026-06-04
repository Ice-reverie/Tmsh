#include "FITKMesherDriverTmshExec.h"
#include "FITKTmshExecProgramInputInfo.h"
#include "FITKTmshExecProgramDriver.h"
#include "GUITmshSettings.h"
#include "GUITetGenSettings.h"

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
namespace {
    constexpr const char* kMesherKey = "TmshExec";

    QString tmshOutputDir()
    {
#ifdef Q_OS_LINUX
        return QDir::cleanPath(QCoreApplication::applicationDirPath() + QString("/tmsh"));
#else
        return QDir::cleanPath(QCoreApplication::applicationDirPath() + QString("/../Tmsh"));
#endif
    }
}

FITKMesherDriverTmshExec::FITKMesherDriverTmshExec() = default;
FITKMesherDriverTmshExec::~FITKMesherDriverTmshExec() = default;


void FITKMesherDriverTmshExec::startMesher(QStringList info)
{
    Q_UNUSED(info)

    int method = 0;
    if (!this->getValue("Method").isNull())
    {
        method = this->getValueT<int>("Method");
    }
    if (method == 0)
    {
        this->buildSingleStageArgs();
    }
    else if (method == 1)
    {
        this->tmshMeshGenSetting();
    }
    else if (method == 2)
    {
        this->tetgenMeshGenSetting();
    }
}

void FITKMesherDriverTmshExec::stopMesher(QStringList info)
{
    Q_UNUSED(info)
    //TODO
}


void FITKMesherDriverTmshExec::tmshMeshGenSetting()
{
    if (_settingsDialog) {
        _settingsDialog->raise();
        _settingsDialog->activateWindow();
        return;
    }
    _settingsDialog = new GUI::GUITmshSettings(this, FITKAPP->getGlobalData()->getMainWindow());
    connect(_settingsDialog, &QDialog::accepted,
            this, &FITKMesherDriverTmshExec::writerModelFileTmshSingle);
    connect(_settingsDialog, &QObject::destroyed,
            this, [this]() { _settingsDialog = nullptr; });
    _settingsDialog->show();
}

void FITKMesherDriverTmshExec::tetgenMeshGenSetting()
{
    if (_tetgenSettingsDialog) {
        _tetgenSettingsDialog->raise();
        _tetgenSettingsDialog->activateWindow();
        return;
    }
    _tetgenSettingsDialog =
        new GUI::GUITetGenSettings(this, FITKAPP->getGlobalData()->getMainWindow());
    connect(_tetgenSettingsDialog, &QDialog::accepted,
            this, &FITKMesherDriverTmshExec::writerModelFileTetGenSingle);
    connect(_tetgenSettingsDialog, &QObject::destroyed,
            this, [this]() { _tetgenSettingsDialog = nullptr; });
    _tetgenSettingsDialog->show();
}

void FITKMesherDriverTmshExec::writerModelFileTmshSingle()
{
    exportGeometryAndSetPaths();
    startMeshGenerationOnce();
}

void FITKMesherDriverTmshExec::writerModelFileTetGenSingle()
{
    exportGeometryAndSetPaths();
    startMeshGenerationOnce();
}

void FITKMesherDriverTmshExec::exportGeometryAndSetPaths()
{
    QVariant v = this->getValue("virtualTopos");
    QList<Interface::VirtualShape> topos = v.value<QList<Interface::VirtualShape>>();
    
    const QString meshPath = tmshOutputDir();
    QDir meshDir(meshPath);
    if (!meshDir.exists()) meshDir.mkpath(meshPath);

    QString inputMeshFile = this->getValueT<QString>("InputMeshFile");
    QString shapeFile;
    
    if (!inputMeshFile.isEmpty() && QFileInfo(inputMeshFile).exists())
    {
        shapeFile = inputMeshFile;
    }
    else
    {
        if (topos.isEmpty()) return;
        
        shapeFile = meshDir.filePath("geometryFile.brep");
        QFile::remove(shapeFile);
        
        Interface::FITKMeshGenInterface* mf = Interface::FITKMeshGenInterface::getInstance();
        if (!mf) return;
        Interface::FITKGlobalMeshGenerateAlgorithmInfo* algInfo =
            mf->getGlobalMeshGenerateAlgorithmInfo(kMesherKey);
        if (!algInfo) return;

        Interface::FITKInterfaceGeometryFactory* geoFactory =
            Interface::FITKInterfaceGeometryFactory::getInstance();
        if (!geoFactory) return;
        Interface::FITKAbsGeoModelExportTopos* geoExport =
            dynamic_cast<Interface::FITKAbsGeoModelExportTopos*>(
                geoFactory->createCommand(Interface::FITKGeoEnum::FITKGeometryComType::FGTExportTopos));
        if (!geoExport) return;

        if (algInfo->getMeshGenerateDimension() == 2)
            geoExport->setEnableStitch(algInfo->getGeometryStitch());
        geoExport->setExportTopos(topos);
        geoExport->setFileName(shapeFile);
        if (!geoExport->update()) return;
    }

    if (this->getValue("MeshFile").isNull() ||
        this->getValueT<QString>("MeshFile").isEmpty()) {
        this->setValue("MeshFile", meshDir.filePath("mesh.mesh"));
    }
    const QString meshFile = this->getValueT<QString>("MeshFile");
    if (!meshFile.isEmpty() &&
        QFileInfo(meshFile).absolutePath() == meshDir.absolutePath()) {
        QFile::remove(meshFile);
    }

    this->setValue("ShapeFile", shapeFile);
}

void FITKMesherDriverTmshExec::startMeshGenerationOnce()
{
    QStringList args = buildSingleStageArgs();
    if (args.isEmpty()) {
        emit mesherFinished();
        return;
    }

    auto* inputInfo = new TmshExe::FITKTmshExecProgramInputInfo;
    inputInfo->setArgs(args);

    auto* mgr = FITKAPP->getProgramTaskManager();
    auto* exe = dynamic_cast<TmshExe::FITKTmshExecProgramDriver*>(
        mgr->createProgram(1, "FITKTmshExecProgramDriver", inputInfo));
    if (!exe || !exe->isExistExeProgram()) {
        emit mesherFinished();
        return;
    }
    connect(exe, &TmshExe::FITKTmshExecProgramDriver::sig_Finish,
            this, &FITKMesherDriverTmshExec::onSingleStageFinished);
    exe->start();
}

QStringList FITKMesherDriverTmshExec::buildSingleStageArgs() const
{
    QStringList args;
    const QString shapeFile = this->getValueT<QString>("ShapeFile");
    const QString meshFile  = this->getValueT<QString>("MeshFile");
    if (shapeFile.isEmpty() || meshFile.isEmpty()) return {};

    args << "--in" << shapeFile
         << "--out" << meshFile;

    auto* algInfo = Interface::FITKMeshGenInterface::getInstance()
                        ->getGlobalMeshGenerateAlgorithmInfo(kMesherKey);
    if (!algInfo) return args;

    args << "--mesh_dim" << QString::number(algInfo->getMeshGenerateDimension());

    auto* tmshAlg = dynamic_cast<Tmsh::FITKTmshGlobalMeshGenerateAlgorithmInfo*>(algInfo);
    if (!tmshAlg) return args;

    if (tmshAlg->hasTangencyTolerance())
        args << "--tangent_tol" << QString::number(tmshAlg->getTangencyTolerance(), 'g', 16);
    if (tmshAlg->hasMinLength())
        args << "--min_length" << QString::number(tmshAlg->getMinLength(), 'g', 16);
    if (tmshAlg->hasMaxLength())
        args << "--max_length" << QString::number(tmshAlg->getMaxLength(), 'g', 16);
    if (tmshAlg->hasMeshDim())
        args << "--mesh_dim" << QString::number(tmshAlg->getMeshDim(), 'g', 16);
    // if (tmshAlg->hasTetgenSwitches())
        // args << "--tetgen_switches" << QString::number(tmshAlg->getTetgenSwitches(), 'g', 16);
    if (tmshAlg->hasNormalTol())
        args << "--normal_tol" << QString::number(tmshAlg->getNormalTol(), 'g', 16);
    if (tmshAlg->hasMinAngle())
        args << "--min_angle" << QString::number(tmshAlg->getMinAngle(), 'g', 16);
    if (tmshAlg->hasUseMultiThreading())
        args << "--use_multi_threading" << QString::number(tmshAlg->getUseMultiThreading(), 'g', 16);
    /*
    if (tmshAlg->hasRefineIter())
        args << "--refine_iter" << QString::number(tmshAlg->getRefineIter(), 'g', 16);
    if (tmshAlg->hasRefineSmoothIter())
        args << "--refine_smooth_iter" << QString::number(tmshAlg->getRefineSmoothIter(), 'g', 16);
    if (tmshAlg->hasAdaptIter())
        args << "--adapt_iter" << QString::number(tmshAlg->getAdaptIter(), 'g', 16);
    if (tmshAlg->hasAdaptSmoothIter())
        args << "--adapt_smooth_iter" << QString::number(tmshAlg->getAdaptSmoothIter(), 'g', 16);
    */
    return args;
    //TODO 传参内容分离
}

void FITKMesherDriverTmshExec::onSingleStageFinished()
{
    emit mesherFinished();
}

}
