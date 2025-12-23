#include "FITKAbstractViewAdaptor.h"


namespace Adaptor
{
    FITKAbstractViewAdaptor::~FITKAbstractViewAdaptor()
    {
    }


    QString FITKAbstractViewAdaptor::getAdaptorClass()
    {
        //返回错误值
        return QString();
    }

    Adaptor::FITKAdaptorType FITKAbstractViewAdaptor::getAdaptorType()
    {
        //返回视图类
        return FITKAdaptorType::AdaTView;
    }

}

