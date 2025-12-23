#include "FITKParticalMesh.h"


namespace Interface
{

    /**
     * @brief 获取粒子网格的绝对模型类型
     *
     * 该函数用于返回粒子网格对象的绝对模型类型。在FITK框架中，不同的模型类型通过枚举值来区分，
     * 这里返回的是AMTPartical，表示该对象是一个粒子网格模型。
     *
     * @return FITKModelEnum::AbsModelType 返回粒子网格的绝对模型类型枚举值
     */
    FITKModelEnum::AbsModelType FITKParticalMesh::getAbsModelType()
    {
        return FITKModelEnum::AMTPartical;
    }

    void FITKParticalMesh::update()
    {

    }

    FITKModelEnum::FITKMeshDim FITKParticalMesh::getMeshDim()
    {
        //默认是三维模型
        return FITKModelEnum::FITKMeshDim::FMDimD3;
    }

}
