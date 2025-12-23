#include "FITKGmshMshIOInterface.h"
#include "FITKGmshMshIOReader.h"
#include "FITK_Interface/FITKInterfaceModel/FITKComponentManager.h"
#include "FITK_Kernel/FITKCore/FITKLibInfo.h"
#include <QThread>

FITKLIBINFOREGISTER(FITKGmshMshIO, FITKGMSHMSHIOVERSION);

namespace Gmsh
{
    QString FITKGmshMshIOInterface::getComponentName()
    {
        return "GmshMshIO";
    }

    void FITKGmshMshIOInterface::setFileName(const QString& fileName)
    {
        _fileName = fileName;
    }

    bool FITKGmshMshIOInterface::exec(const int indexPort)
    {
        //如果正在执行，则退出，不能同时执行多个任务
        _success = false;
        if (this->isRunning()) return false;

        switch (indexPort)
        {
        case 1:  //读取
        {
            FITKGmshMshIOReader* readerIO = new FITKGmshMshIOReader;
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


    void FITKGmshMshIOInterface::runInThread(const bool & inThread /*= true*/)
    {
        _runInThread = inThread;
    }

    void FITKGmshMshIOInterface::setReadFilterDim(QList<int> dim)
    {
        _filterDim = dim;
    }

    void FITKGmshMshIOInterface::setFilterLowDimElement(bool isFilter)
    {
        _isFilterElement = isFilter;
    }

    bool FITKGmshMshIOInterface::isRunning()
    {
        return _io != nullptr;
    }

    bool FITKGmshMshIOInterface::isSuccess()
    {
        return _success;
    }
}


