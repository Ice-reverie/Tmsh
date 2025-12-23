#include "FITKAbstractIOAdaptor.h"

namespace Adaptor
{
    /**
     * @brief Destroy the FITKAbstractIOAdaptor::FITKAbstractIOAdaptor object
     * @author LiBaojun (libaojunqd@foxmail.com)
     * @date 2024-03-03
     */
    FITKAbstractIOAdaptor::~FITKAbstractIOAdaptor()
    {
    }

    QString FITKAbstractIOAdaptor::getAdaptorClass()
    {
        return QString();
    }

    FITKAdaptorType FITKAbstractIOAdaptor::getAdaptorType()
    {
        //返回错误值
        return FITKAdaptorType::AdaTIO;
    }

    bool FITKAbstractIOAdaptor::adaptR()
    {
        return false;
    }

    bool FITKAbstractIOAdaptor::adaptW()
    {
        return false;
    }

}