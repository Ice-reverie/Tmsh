#include "FITKDataFactory.h"
#include "FITKAbstractDataObject.h"
#include <QMutexLocker>

namespace Core
{
    //静态变量初始化
    FITKDataFactory* FITKDataFactory::_instance = nullptr;
    QMutex FITKDataFactory::m_mutex;

    FITKAbstractObject* FITKDataFactory::create(const QString& key)
    {
        // 线程锁
        QMutexLocker locker(&m_mutex);
        //注册的函数
        if (!_itemDataFuns.contains(key)) return nullptr;
        auto f = _itemDataFuns.value(key);
        //执行函数
        if (f == nullptr) return nullptr;
        auto data = f();
        return data;
    }

    void FITKDataFactory::initialize()
    {
        //初始化，无需操作
    }

    void FITKDataFactory::finalize()
    {

    }

    FITKDataFactoryDeleator::~FITKDataFactoryDeleator()
    {
        if (FITKDataFactory::_instance)
            delete FITKDataFactory::_instance;
        FITKDataFactory::_instance = nullptr;
    }
    static FITKDataFactoryDeleator __DELEATOR__;
}

