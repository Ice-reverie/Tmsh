#include "HDF5IOInterface.h"
#include "HDF5Reader.h"
#include "HDF5Writer.h"
#include <QThread>

namespace IO
{
    bool HDF5IOInterface::exec(const int indexPort)
    {
        //如果正在执行，则退出，不能同时执行多个任务
        _success = false;
        if (this->isRunning()) return false;

        switch (indexPort)
        {
        case 1:  //读取
        {
            HDF5Reader* reader  = new HDF5Reader(this, _success);
            reader->setVersion(1.0);
            reader->setPluginData(_pluginList);
            _io = reader;
            break;
        }
            
        case 2:  //写出
        {
            HDF5Writer* writer  = new HDF5Writer(this, _success);
            writer->setVersion(1.0);
            writer->setPluginData(_pluginList);
            _io = writer;
            break;
        }
        default: break;
        }
        //参数错误，直接退出
        if (_io == nullptr) return false;
        _io->setFileName(_fileName);
        
        //在线程中执行
        if (_runInThread)
        {
            connect(_io.getObjectPtr(), SIGNAL(taskFinishedSig(FITKThreadTask*)), this, SIGNAL(ioThreadFinishedSig()));
            _io->push2ThreadPool();
            QThread::msleep(100);
        }
        //不在线程执行
        else
        {
            _io->run();
            auto p = _io.getObjectPtr();
            if (p) delete p;
        }

        //参数传递完成，清空参数
        this->resetDataObjects();
        return _success;
    }
    void HDF5IOInterface::setFileName(const QString& fileName)
    {
        _fileName = fileName;
    }

    void HDF5IOInterface::setPluginData(QList<AppFrame::FITKAbstractPlugin*> pluginList)
    {
        _pluginList = pluginList;
    }

    void HDF5IOInterface::runInThread(const bool & inThread /*= true*/)
    {
        _runInThread = inThread;
    }

    bool HDF5IOInterface::isRunning()
    {
        return _io != nullptr;
    }

    bool HDF5IOInterface::isSuccess()
    {
        return _success;
    }

    QString HDF5IOInterface::getComponentName()
    {
        return "MeshHDF5IO";
    }

}
