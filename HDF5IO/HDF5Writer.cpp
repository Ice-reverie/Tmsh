#include "HDF5Writer.h"
#include "FITK_Kernel/FITKAppFramework/FITKMessage.h"
#include "FITK_Kernel/FITKAppFramework/FITKAbstractPlugin.h"
#include "FITK_Interface/FITKInterfaceIO/FITKHDF5FileTool.h"

#include <H5Cpp.h>

namespace IO
{
    HDF5Writer::HDF5Writer(HDF5IOInterface* interface, bool & isScuccess)
        :_isSuccess(isScuccess), _compInterface(interface)
    {

    }



    void HDF5Writer::run()
    {
        QString err;
        //创建hdf5工程
        if (this->createContext(err) == false)
        {
            consoleMessage(3, QString("Create hdf5 Context err: %1").arg(err));
            return;
        }
        //写出版本
        if (this->createVersion() == false)
        {
            consoleMessage(3, tr("Description Failed to write out the project file"));
            return;
        }
        //写出插件的工程数据
        _isSuccess &= this->writePluginData();
        //写出完成
        consoleMessage(1, tr("Project file Write finish"));
    }

    void HDF5Writer::setPluginData(QList<AppFrame::FITKAbstractPlugin*> pluginList)
    {
        _pluginList = pluginList;
    }

    void HDF5Writer::consoleMessage(int level, const QString& str)
    {
        switch (level)
        {
        case 1:AppFrame::FITKMessageNormal(str); break;
        case 2: AppFrame::FITKMessageWarning(str); break;
        case 3:AppFrame::FITKMessageError(str); break;
        default: AppFrame::FITKMessageError(str); break;
        }
    }

    bool HDF5Writer::createVersion()
    {
        if (_h5File == nullptr)
        {
            return false;
        }
        FITKHDF5FileTool*  tool = this->getHDF5FileTool();
        if (!tool) return false;
        std::tuple<bool, H5::Group> Ret = tool->createGp("Version");//创建版本组
        if (std::get<0>(Ret) == false) { return false; }
        H5::Group VersionGroup = std::get<1>(Ret);
        QString vesion = QString::number(_version, 'f', 1);
        tool->writeGroupAttrStr(VersionGroup, "version", vesion.toStdString());
        tool->writeGroupAttrStr(VersionGroup, "project_type", "AppMesh");
        return true;
    }

    bool HDF5Writer::writePluginData()
    {
        bool ok = true;
        for (QList<AppFrame::FITKAbstractPlugin*>::iterator iter = _pluginList.begin(); iter != _pluginList.end(); ++iter)
        {
            if (*iter == nullptr) continue;
            QString errInfo;
            bool wOK = (*iter)->write(this, errInfo);
            if (!wOK)
                this->consoleMessage(3, errInfo);
            ok &= wOK;
        }
        return ok;
    }
}



