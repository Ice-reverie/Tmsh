#include "OperatorsSave.h"
#include "OperatorExportGmshFile.h"

#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Kernel/FITKAppFramework/FITKComponents.h"
#include "FITK_Kernel/FITKAppFramework/FITKMessage.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppSettings.h" 
#include "FITK_Kernel/FITKAppFramework/FITKRunTimeSetting.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppHistoryFiles.h"
#include "FITK_Kernel/FITKAppFramework/FITKPluginManager.h"
#include "FITK_Kernel/FITKAppFramework/FITKAbstractPlugin.h"
#include "FITK_Kernel/FITKCore/FITKThreadPool.h"

#include "GUIFrame/MainWindow.h"
#include "HDF5IO/HDF5IOInterface.h"
#include "ModelData/MeshManager.h"
#include "ModelData/MeshData.h"
#include "ModelData/MeshKernel.h"

#include <QFileDialog>

namespace ModelOper
{
    bool OperatorsSave::execGUI()
    {
        if (!_emitter) return false;
        AppFrame::FITKRunTimeSetting* rs = FITKAPP->getGlobalData()->getRunTimeSetting();
        QString saveFilePath;
        QString actionName = _emitter->objectName();
        if (actionName == "actionSave" && rs != nullptr)
        {
            saveFilePath = rs->getValueT<QString>("ProjectFile");
        }
        if (saveFilePath.isEmpty() || !QFile::exists(saveFilePath))
        {
            QString wk;
            auto settings = FITKAPP->getAppSettings();
            if (settings)
                wk = settings->getWorkingDir();
            QFileDialog dlg(FITKAPP->getGlobalData()->getMainWindow(), QObject::tr("SaveAs Project File"), wk, "HDF5(*.hdf5);;Gmsh Mesh(*.msh)");
            dlg.setAcceptMode(QFileDialog::AcceptSave);
            dlg.show();
            bool accept = false;
            QEventLoop loop;
            connect(&dlg, &QFileDialog::accepted, [&] {loop.quit(); accept = true;  });
            connect(&dlg, &QFileDialog::rejected, [&] {loop.quit(); accept = false;  });
            loop.exec();
            if (!accept) return false;
            QStringList files = dlg.selectedFiles();
            if (files.isEmpty()) return false;
            saveFilePath = files.at(0);
            if (saveFilePath.isEmpty())
                return false;
            QString selectedFilter = dlg.selectedNameFilter();
            if (selectedFilter.contains("HDF5"))
            {
                if (!saveFilePath.toLower().endsWith(".hdf5"))
                    saveFilePath += ".hdf5";
            }
            else if (selectedFilter.contains("Gmsh"))
            {
                if (!saveFilePath.toLower().endsWith(".msh"))
                    saveFilePath += ".msh";
            }
        }
        this->setArgs("FileName", saveFilePath);
        return true;
    }

    bool OperatorsSave::execProfession()
    {
        QString fileName;
        if (!this->argValue<QString>("FileName", fileName)) return false;

        if (fileName.toLower().endsWith(".msh"))
        {
            AppFrame::FITKMessageNormal(QString(tr("Exporting Gmsh file %1 ...")).arg(fileName));
            OperatorExportGmshFile exportGmsh;
            exportGmsh.setArgs("FileName", fileName);

            ModelData::MeshManager* managerMesh = FITKAPP->getGlobalData()->getMeshData<ModelData::MeshManager>();
            if (managerMesh && managerMesh->getDataCount() > 0)
            {
                ModelData::MeshData* meshData = managerMesh->getDataByIndex(0);
                if (meshData && meshData->getDataCount() > 0)
                {
                    ModelData::MeshKernel* meshKernel = meshData->getDataByIndex(0);
                    if (meshKernel)
                        exportGmsh.setArgs("KernelID", meshKernel->getDataObjectID());
                }
            }

            bool result = exportGmsh.execProfession();
            this->clearArgs();
            return result;
        }

        AppFrame::FITKMessageNormal(QString(tr("Saving project file %1 ...")).arg(fileName));

        QList<AppFrame::FITKAbstractPlugin*> pluginList;
        AppFrame::FITKPluginsManager* pluginManager = FITKAPP->getPluginsManager();
        if (pluginManager)
        {
            int count = pluginManager->getLibraryCount();
            for (int i = 0; i < count; ++i)
            {
                AppFrame::FITKAbstractPlugin* plugin = pluginManager->getPlugin(i);
                if (!plugin) continue;
                pluginList.append(plugin);
            }
        }

        IO::HDF5IOInterface* h5IO = FITKAPP->getComponents()->
            getComponentTByName<IO::HDF5IOInterface>("MeshHDF5IO");

        Core::FITKThreadPool::getInstance()->wait();
        if (h5IO == nullptr) return false;

        h5IO->setPluginData(pluginList);
        h5IO->setFileName(fileName);
        h5IO->runInThread();
        connect(h5IO, SIGNAL(ioThreadFinishedSig()), this, SLOT(ioThreadFinishedSlot()), Qt::UniqueConnection);
        h5IO->exec(2);
        return h5IO->isSuccess();
    }

    void OperatorsSave::ioThreadFinishedSlot()
    {
        QString fileName;
        if (!this->argValue<QString>("FileName", fileName)) return;
        this->clearArgs();
        AppFrame::FITKRunTimeSetting* rts = FITKAPP->getGlobalData()->getRunTimeSetting();
        if (rts == nullptr) return;
        rts->setValue("ProjectFile", fileName);
        AppFrame::FITKAppHistoryFiles* historyFiles = FITKAPP->getGlobalData()->getHistoryFiles();
        if (historyFiles == nullptr) return;
        historyFiles->addFile(fileName, AppFrame::FITKAppHistoryFileInfo::HistoryFileType::HFTProj);
    }
}
