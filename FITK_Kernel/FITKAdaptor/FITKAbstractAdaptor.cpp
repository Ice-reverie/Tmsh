#include "FITKAbstractAdaptor.h"
#include "FITK_Kernel/FITKCore/FITKLibInfo.h"

namespace Adaptor
{
   FITKLIBINFOREGISTER(FITKAdaptor, FITKAdaptorVersion);

    /**
     * @brief Destroy the FITKAbstractAdaptor::FITKAbstractAdaptor object
     * @author LiBaojun (libaojunqd@foxmail.com)
     * @date 2024-03-03
     */
    FITKAbstractAdaptor::~FITKAbstractAdaptor()
    {

    }

    QString FITKAbstractAdaptor::getAdaptorClass()
    {
        return QString();
    }

    FITKAdaptorType FITKAbstractAdaptor::getAdaptorType()
    {
        //返回错误值
        return FITKAdaptorType::AdaTNone;
    }

    void FITKAbstractAdaptor::setDataObject(Core::FITKAbstractDataObject* data)
    {
        _dataObj = data;
    }

    bool FITKAbstractAdaptor::adapt()
    {
        return false;
    }

}



