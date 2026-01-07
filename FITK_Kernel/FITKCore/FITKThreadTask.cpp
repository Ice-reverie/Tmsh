#include "FITKThreadTask.h"
#include "FITKThreadPool.h"
#include "FITKThreadPoolPrivate.h"
#include "FITKThreadTaskGroup.h"

namespace Core
{
    FITKThreadTask::FITKThreadTask(QObject* parent /*= nullptr*/) : QObject(parent)
    {
        //设置自动删除，线程结束自动delete
        this->setAutoDelete(true);
    }

    FITKThreadTask::~FITKThreadTask()
    {
        auto ag = FITKThreadPool::getInstance()->getAgent();
        if (ag == nullptr) return;
        //从列表删除
        ag->removeTask(this);
        //发生信号
        emit taskFinishedSig(this);
        //从任务组移除
        if (_taskGroup)
            _taskGroup->removeThreadTask(this);
    }

    void FITKThreadTask::setResultMark(bool *success)
    {
        _resultMark = success;
    }

    void FITKThreadTask::setTaskGroup(FITKThreadTaskGroup* group)
    {
        _taskGroup = group;
    }

}


