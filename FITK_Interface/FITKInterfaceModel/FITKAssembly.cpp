#include "FITKAssembly.h"

namespace Interface
{
    /**
     * @brief Construct a new FITKAssembly::FITKAssembly object
     * @author LiBaojun (libaojunqd@foxmail.com)
     * @date 2024-03-29
     */
    FITKAssembly::FITKAssembly()
    {
    }
    /**
     * @brief Destroy the FITKAssembly::FITKAssembly object
     * @author LiBaojun (libaojunqd@foxmail.com)
     * @date 2024-03-29
     */
    FITKAssembly::~FITKAssembly()
    {
    }

    FITKModelEnum::AbsModelType FITKAssembly::getAbsModelType()
    {
        //装配体类型
        return FITKModelEnum::AbsModelType::Assembly;
    }

    void FITKAssembly::removeInstanceByModel(const int modelID)
    {
        //遍历查找使用该模型的装配实例
        QList<FITKAbstractAssInstance*> instanceList;
        for (int i = 0; i < this->getDataCount(); ++i)
        {
            FITKAbstractAssInstance* ins = this->getDataByIndex(i);
            if(ins == nullptr) continue;
            if (ins->getModelID() == modelID)
                instanceList.append(ins);
        }
        //移除对象
        for (FITKAbstractAssInstance* ins : instanceList)
            this->removeDataObj(ins);
    }

}



