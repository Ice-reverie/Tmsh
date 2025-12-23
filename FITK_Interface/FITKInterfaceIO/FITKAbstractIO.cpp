#include "FITKAbstractIO.h"
#include "FITK_Kernel/FITKCore/FITKThreadPool.h"
#include "FITK_Kernel/FITKCore/FITKLibInfo.h"

FITKLIBINFOREGISTER(FITKInterfaceIO, FITKInterfaceIOVersion)


namespace Interface
{
    FITKAbstractIO::~FITKAbstractIO()
    {
    }

    void FITKAbstractIO::setFileName(const QString& fileName)
    {
        //记录文件名称
        _fileName = fileName;
    }

    QString FITKAbstractIO::getFileName() const
    {
        //直接返回文件名称
        return _fileName;
    }

    void FITKAbstractIO::setDataObject(Core::FITKAbstractDataObject* data)
    {
        _data = data;
    }

    void FITKAbstractIO::push2ThreadPool()
    {
        //直接在线程池运行
        Core::FITKThreadPool::getInstance()->execTask(this);
    }
}



