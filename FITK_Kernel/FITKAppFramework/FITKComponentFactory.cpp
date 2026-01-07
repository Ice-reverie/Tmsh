#include "FITKComponentFactory.h"
#include "FITKAppFramework.h"
#include "FITKComponents.h"
#include "FITKComponentInterface.h"

namespace AppFrame
{
    void FITKComponentFactory::create()
    {
        //错误判断
        auto comps = FITKAPP->getComponents();
        if (comps == nullptr) return;
       //虚函数执行
        QList<FITKComponentInterface*> compList = this->createComponents();
       //压入管理类
        for (FITKComponentInterface* ci : compList)
        {
            ci->init();
            comps->addComponent(ci);
        }
            
    }

    QList<FITKComponentInterface*> FITKComponentFactory::createComponents()
    {
        //抽象类返回错误值
        return QList<FITKComponentInterface*>();
    }

}


