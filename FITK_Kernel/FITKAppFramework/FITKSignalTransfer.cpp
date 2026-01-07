#include "FITKSignalTransfer.h"
#include <QMetaMethod>

AppFrame::FITKSignalTransfer::~FITKSignalTransfer()
{
    //释放数据处理器
    for (auto obj : _signalProcesser)
        delete obj;
    _signalProcesser.clear();
}

void AppFrame::FITKSignalTransfer::addSignalProcesser(QObject* obj)
{
    const QMetaObject* thisMeta = this->metaObject();
    const QMetaObject* slotMeta = obj->metaObject();

    //根据信号的名字自动匹配槽函数
    auto getSlotIndex = [&](const QString& name)
    {
        //遍历全部函数
        const int mc = slotMeta->methodCount();
        for (int i = 0; i < mc; ++i)
        {
            QMetaMethod me = slotMeta->method(i);
            //只对槽函数进行判断
            if (me.methodType() != QMetaMethod::Slot) continue;
            const QString slotName = me.name();
            //槽函数名称符合on_<信号名称> 则认为找到
            if (slotName == "on_" + name) return i;
        }
        return -1;
    };

    //遍历全部函数
    const int mc = thisMeta->methodCount();
    for ( int i = 0; i<mc; ++i)
    {
        QMetaMethod me = thisMeta->method(i);
        //只判断信号
        if (me.methodType() != QMetaMethod::Signal) continue;
        const QString signalName = me.name();
        //根据信号名称获取槽函数名称
        const int index = getSlotIndex(signalName);
        if (index < 0) continue;
        //关联信号与槽函数
        QMetaObject::connect(this, i, obj, index );
    }
    _signalProcesser.append(obj);
}

