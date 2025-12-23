#include "FITKAbstractFactory.h"

namespace Core
{
    /**
     * @brief Destroy the FITKAbstractFactory::FITKAbstractFactory object
     * @author LiBaojun (libaojunqd@foxmail.com)
     * @date 2024-03-02
     */
    FITKAbstractFactory::~FITKAbstractFactory()
    {

    }

    Core::FITKAbstractObject* FITKAbstractFactory::create(const QString& key)
    {
        return nullptr;
    }

    Core::FITKAbstractObject* FITKAbstractFactory::create(const int key)
    {
        return nullptr;
    }

}
