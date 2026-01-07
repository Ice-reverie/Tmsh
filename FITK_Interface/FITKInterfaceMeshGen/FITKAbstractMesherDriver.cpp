#include "FITKAbstractMesherDriver.h"

namespace Interface
{
    FITKAbstractMesherDriver::~FITKAbstractMesherDriver()
    {

    }


    void FITKAbstractMesherDriver::setDataObject(Core::FITKAbstractDataObject* data)
    {
        //临时数据
        _data = data;
    }

    void FITKAbstractMesherDriver::setArgs(const QStringList& args)
    {
        //启动参数
        _args = args;
    }

    QStringList FITKAbstractMesherDriver::getArgs() const
    {
        return _args;
    }

    void FITKAbstractMesherDriver::startMesher(QStringList info /*= QStringList()*/)
    {
        //纯虚函数
    }

    void FITKAbstractMesherDriver::stopMesher(QStringList info /*= QStringList()*/)
    {
        //纯虚函数
    }

    void FITKAbstractMesherDriver::insertDataObject(QString key, Core::FITKAbstractDataObject* value)
    {
        _dataObject.insert(key, value);
    }
}


