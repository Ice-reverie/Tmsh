#include "HDF5Reader.h"
#include "FITK_Kernel/FITKAppFramework/FITKMessage.h"
#include "FITK_Kernel/FITKAppFramework/FITKAbstractPlugin.h"
#include "FITK_Interface/FITKInterfaceIO/FITKHDF5FileTool.h"

#include <H5Cpp.h>

namespace IO
{
//    using namespace Acoustics;
    HDF5Reader::HDF5Reader(HDF5IOInterface* interface, bool & success)
        :_compInterface(interface), _isSuccess(success)
    {

    }

    void HDF5Reader::run()
    {
        QString err;
        //创建hdf5工程
        if (this->createContext(err) == false)
        {
            consoleMessage(3, QString("Create hdf5 Context err ! %1").arg(err));
            return;
        }
        //检查版本
        if (checkVersion() == false)
        {
            consoleMessage(3, tr("Not a valid engineering document"));//不是有效的工程文件
            return;
        }
        //读取插件的工程数据
        _isSuccess &= this->readPluginData();
        //读取完成
        consoleMessage(1, tr("Project file Read finish"));
    }

    void HDF5Reader::setPluginData(QList<AppFrame::FITKAbstractPlugin*> pluginList)
    {
        _pluginList = pluginList;
    }

    void HDF5Reader::consoleMessage(int level, const QString& str)
    {
        switch (level)
        {
        case 1:AppFrame::FITKMessageNormal(str); break;
        case 2: AppFrame::FITKMessageWarning(str); break;
        case 3:AppFrame::FITKMessageError(str); break;
        default: AppFrame::FITKMessageError(str); break;
        }
    }

    bool HDF5Reader::checkVersion()
    {
        if (_h5File==nullptr)
        {
            return false;
        }
        FITKHDF5FileTool*  tool = this->getHDF5FileTool();
        if (!tool) return false;
        std::tuple<bool, H5::Group> Ret = tool->openGp("Version");//打开版本组
        if (std::get<0>(Ret) == false) { return false; }
        H5::Group VersionGroup = std::get<1>(Ret);

        std::string vers;
        std::string type;
        tool->readGroupStr(VersionGroup, "version", vers);
        tool->readGroupStr(VersionGroup, "project_type", type);
        
        QString version = QString::fromStdString(vers);
        bool ok = false;
        double Dversion=version.toDouble(&ok);
        if (ok==false)
        {
            return false;
        }
        if (type!="AppMesh"|| Dversion !=_version)
        {
            return false;
        }
        return true;
    }

    bool HDF5Reader::readPluginData()
    {
        bool ok = true;
        for (QList<AppFrame::FITKAbstractPlugin*>::iterator iter = _pluginList.begin(); iter != _pluginList.end(); ++iter)
        {
            if (*iter == nullptr) continue;
            QString errInfo;
            bool wOK = (*iter)->read(this, errInfo);
            if (!wOK)
                this->consoleMessage(3, errInfo);
            ok &= wOK;
        }
        return ok;
    }
}
