#include "OperatorsOpen.h"

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

#include "OperatorsInterface/GraphEventOperator.h"
#include "HDF5IO/HDF5IOInterface.h"
#include "GUIFrame/MainWindow.h"

#include <QFile>
#include <QFileDialog>
#include <QAction>

namespace ModelOper
{
    bool OperatorsOpen::execGUI()
    {
        //主界面
        QWidget* mw = FITKAPP->getGlobalData()->getMainWindow();
        //获取工作目录
        QString wk;
        AppFrame::FITKAppSettings* settings = FITKAPP->getAppSettings();
        if (settings)
            wk = settings->getWorkingDir();
        QFileDialog dlg(mw, QObject::tr("Open Project File"), wk, "HDF5(*.hdf5)");
        dlg.setAcceptMode(QFileDialog::AcceptOpen);  //打开模式
        dlg.show();
        //阻塞，否则键盘事件处理会出问题
        bool accept = false;
        QEventLoop loop;
        connect(&dlg, &QFileDialog::accepted, [&] {loop.quit(); accept = true;  });
        connect(&dlg, &QFileDialog::rejected, [&] {loop.quit(); accept = false;  });
        loop.exec();
        if (!accept) return false;
        //获取文件
        QStringList files = dlg.selectedFiles(); //选择的文件名称
        if (files.isEmpty()) return false;
        QString fileName = files.at(0);
        if (fileName.isEmpty())
            return false;
        //存储信息
        if (_emitter == nullptr) return false;

        this->setArgs("FileName", fileName);
        return true;
    }

    bool OperatorsOpen::execProfession()
    {
        //获取打开文件的路径
        QString fileName;
        if (!this->argValue<QString>("FileName", fileName)) return false;

        //输出信息
        AppFrame::FITKMessageNormal(QString(tr("Reading project file %1 ...")).arg(fileName));

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
        //执行读取
        h5IO->exec(1);

        ////保存执行的脚本
        //QString scr = QString("Global.Files.ImportHDF5File('%1')").arg(fileName);
        //this->saveScript(scr);
        return h5IO->isSuccess();
    }

    void OperatorsOpen::ioThreadFinishedSlot()
    {
        //获取工程文件
        QString fileName;
        if (!this->argValue<QString>("FileName", fileName)) return;
        this->clearArgs();
        //获取软件运行时参数，并设置工程文件路径
        AppFrame::FITKRunTimeSetting* rts = FITKAPP->getGlobalData()->getRunTimeSetting();
        if (rts == nullptr) return;
        rts->setValue("ProjectFile", fileName);
        //执行渲染层刷新操作刷新操作
        EventOper::GraphEventOperator* operGraph = FITKOPERREPO->getOperatorT<EventOper::GraphEventOperator>("GraphPreprocess");
        if (operGraph) operGraph->updateAllGraph();
        //保存打开的历史文件
        AppFrame::FITKAppHistoryFiles* historyFiles = FITKAPP->getGlobalData()->getHistoryFiles();
        if (historyFiles == nullptr) return;
        historyFiles->addFile(fileName, AppFrame::FITKAppHistoryFileInfo::HistoryFileType::HFTProj);
    }
}

