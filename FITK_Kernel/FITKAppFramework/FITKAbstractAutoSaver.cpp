#include "FITKAbstractAutoSaver.h"
#include <QTimer>

namespace AppFrame
{
    FITKAbstractAutoSaver::FITKAbstractAutoSaver(const int & msc)
    {
        //创建并初始化计时器
        _timer = new QTimer;
        //默认2分钟执行一次
        this->setTimeInterval(msc);
        this->stop();
        connect(_timer, SIGNAL(timeout()), this,SLOT(timeOutSlot()) );
    }
    FITKAbstractAutoSaver::~FITKAbstractAutoSaver()
    {
        //首先停止计时
        this->stop();
        if (_timer) delete _timer;
    }

    void FITKAbstractAutoSaver::start()
    {
        if (_timer) _timer->start();
    }

    void FITKAbstractAutoSaver::stop()
    {
        if (_timer) _timer->stop();
    }

    void FITKAbstractAutoSaver::setTimeInterval(const int & msc)
    {
        //设置超时时间间隔
        if (_timer) _timer->setInterval(msc);
    }

    void FITKAbstractAutoSaver::autoSave()
    {
        //虚函数
    }

    void FITKAbstractAutoSaver::finalize()
    {

    }

    void FITKAbstractAutoSaver::timeOutSlot()
    {
        //计时器超时触发
        this->autoSave();
    }

}


