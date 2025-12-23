#include "OperatorsSave.h"

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

#include <QFileDialog>

namespace ModelOper
{
    bool OperatorsSave::execGUI()
    {
        if (!_emitter) return false;
        //从运行设置中获取保存的文件
        AppFrame::FITKRunTimeSetting* rs = FITKAPP->getGlobalData()->getRunTimeSetting();
        //获取保存文件的路径
        QString saveFilePath;
        QString actionName = _emitter->objectName();
        if (actionName == "actionSave" && rs != nullptr)
        {
            saveFilePath = rs->getValueT<QString>("ProjectFile");
        }
        if (saveFilePath.isEmpty() || !QFile::exists(saveFilePath))
        {
            //获取工作目录
            QString wk;
            auto settings = FITKAPP->getAppSettings();
            if (settings)
                wk = settings->getWorkingDir();
            //保存工程文件界面
            QFileDialog dlg(FITKAPP->getGlobalData()->getMainWindow(), QObject::tr("SaveAs Project File"), wk, "HDF5(*.hdf5)");
            dlg.setAcceptMode(QFileDialog::AcceptSave);  //打开模式
            dlg.show();
            //阻塞，否则键盘事件处理会出问题
            bool accept = false;
            QEventLoop loop;
            connect(&dlg, &QFileDialog::accepted, [&] {loop.quit(); accept = true;  });
            connect(&dlg, &QFileDialog::rejected, [&] {loop.quit(); accept = false;  });
            loop.exec();
            if (!accept) return false;
            //获取保存文件的路径
            QStringList files = dlg.selectedFiles(); //选择的文件名称
            if (files.isEmpty()) return false;
            saveFilePath = files.at(0);
            if (saveFilePath.isEmpty())
                return false;
            //添加后缀
            if (!saveFilePath.toLower().endsWith(".hdf5"))
                saveFilePath += ".hdf5";
        }
        this->setArgs("FileName", saveFilePath);
        return true;
    }

    bool OperatorsSave::execProfession()
    {
        //获取保存文件的路径
        QString fileName;
        if (!this->argValue<QString>("FileName", fileName)) return false;
        //输出信息
        AppFrame::FITKMessageNormal(QString(tr("Saving project file %1 ...")).arg(fileName));

        //获取插件数据
        QList<AppFrame::FITKAbstractPlugin*> pluginList;
        //获取插件管理器
        AppFrame::FITKPluginsManager* pluginManager = FITKAPP->getPluginsManager();
        if (pluginManager)
        {
            //获取插件
            int count = pluginManager->getLibraryCount();
            for (int i = 0; i < count; ++i)
            {
                AppFrame::FITKAbstractPlugin* plugin = pluginManager->getPlugin(i);
                if (!plugin) continue;
                pluginList.append(plugin);
            }
        }

        //读写组件获取
        IO::HDF5IOInterface* h5IO = FITKAPP->getComponents()->
            getComponentTByName<IO::HDF5IOInterface>("MeshHDF5IO");

        //等待线程结束
        Core::FITKThreadPool::getInstance()->wait();
        if (h5IO == nullptr) return false;

        //传入参数
        h5IO->setPluginData(pluginList);
        h5IO->setFileName(fileName);
        h5IO->runInThread();
        //关联结束读取槽函数
        connect(h5IO, SIGNAL(ioThreadFinishedSig()), this, SLOT(ioThreadFinishedSlot()), Qt::UniqueConnection);
        //执行写出
        h5IO->exec(2);
        ////保存执行的脚本
        //QString scr = QString("Global.Files.SaveFile('%1')").arg(fileName);
        //this->saveScript(scr);
        return h5IO->isSuccess();
    }

    void OperatorsSave::ioThreadFinishedSlot()
    {
        //获取工程文件
        QString fileName;
        if (!this->argValue<QString>("FileName", fileName)) return;
        this->clearArgs();
        //获取软件运行时参数，并设置工程文件路径
        AppFrame::FITKRunTimeSetting* rts = FITKAPP->getGlobalData()->getRunTimeSetting();
        if (rts == nullptr) return;
        rts->setValue("ProjectFile", fileName);
        //保存打开的历史文件
        AppFrame::FITKAppHistoryFiles* historyFiles = FITKAPP->getGlobalData()->getHistoryFiles();
        if (historyFiles == nullptr) return;
        historyFiles->addFile(fileName, AppFrame::FITKAppHistoryFileInfo::HistoryFileType::HFTProj);
    }
}

