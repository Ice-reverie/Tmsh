#include "FITKTmshMshIOInterface.h"
#include "FITKTmshMshIOReader.h"
#include "FITK_Interface/FITKInterfaceModel/FITKComponentManager.h"
#include "FITK_Kernel/FITKCore/FITKLibInfo.h"
#include <QThread>

FITKLIBINFOREGISTER(FITKTmshMshIO, FITKTmshMSHIOVERSION);

namespace Tmsh
{
    QString FITKTmshMshIOInterface::getComponentName()
    {
        return "TmshMshIO";
    }

    void FITKTmshMshIOInterface::setFileName(const QString& fileName)
    {
        _fileName = fileName;
    }

    bool FITKTmshMshIOInterface::exec(const int indexPort)
    {
        //如果正在执行，则退出，不能同时执行多个任务
        _success = false;
        if (this->isRunning()) return false;

        switch (indexPort)
        {
        case 1:  //读取
        {
            FITKTmshMshIOReader* readerIO = new FITKTmshMshIOReader;
            readerIO->setReadFilterDim(_filterDim);
            readerIO->setFilterLowDimElement(_isFilterElement);
            readerIO->setComponentManager(dynamic_cast<Interface::FITKComponentManager*>(this->getDataObject("ComponentManager")));
            _io = readerIO;
        }
            break;
        case 2:  //写出
            
        default: break;
        }
        //参数错误，直接退出
        if (_io == nullptr) return false;
        _io->setFileName(_fileName);
        _io->setDataObject(this->getDataObject("MeshData"));
        _io->setResultMark(&_success);
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


    void FITKTmshMshIOInterface::runInThread(const bool & inThread /*= true*/)
    {
        _runInThread = inThread;
    }

    void FITKTmshMshIOInterface::setReadFilterDim(QList<int> dim)
    {
        _filterDim = dim;
    }

    void FITKTmshMshIOInterface::setFilterLowDimElement(bool isFilter)
    {
        _isFilterElement = isFilter;
    }

    bool FITKTmshMshIOInterface::isRunning()
    {
        return _io != nullptr;
    }

    bool FITKTmshMshIOInterface::isSuccess()
    {
        return _success;
    }
}


