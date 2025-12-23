#include "FITKIOAdaptorFactory.h"
#include "FITKCore/FITKAbstractDataObject.h"

namespace Adaptor
{
    //静态变量初始化
    FITKIOAdaptorFactory* FITKIOAdaptorFactory::_instance = nullptr;
    QMutex FITKIOAdaptorFactory::m_mutex;

    Adaptor::FITKAbstractIOAdaptor* FITKIOAdaptorFactory::create(const QString& fileType, const QString & className)
    {
        //首先根据文件查找
        if (!_createFuns.contains(fileType)) return nullptr;
        FILETypeFuns funs = _createFuns.value(fileType);
        if (!funs.contains(className)) return nullptr;
        //找到创建函数指针
        auto f = funs.value(className);
        if (f == nullptr) return nullptr;
        return f();
    }

    Adaptor::FITKAbstractIOAdaptor* FITKIOAdaptorFactory::create(const QString& fileType, Core::FITKAbstractObject* obj)
    {
        if (obj == nullptr) return nullptr;
        //获取类名，带命名空间
        const QString className = obj->getClassName();
        //根据类名创建
        return this->create(fileType, className);
    }

    void FITKIOAdaptorFactory::initialize()
    {
    }

    void FITKIOAdaptorFactory::finalize()
    {
        _createFuns.clear();
    }

    FITKIOAdaptorFactoryDeleator::~FITKIOAdaptorFactoryDeleator()
    {
        if (FITKIOAdaptorFactory::_instance)
            delete FITKIOAdaptorFactory::_instance;
        FITKIOAdaptorFactory::_instance = nullptr;
    }

    static FITKIOAdaptorFactoryDeleator _FIOADFDELEATOR__;

}