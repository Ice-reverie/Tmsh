#include "FITKAbstractMesh.h"

namespace Interface
{

    FITKAbstractMesh::~FITKAbstractMesh()
    {

    }

    FITKModelEnum::AbsModelType FITKAbstractMesh::getAbsModelType()
    {
         // 返回错误值，子类重写
        return FITKModelEnum::AbsModelType::AMTMesh;
    }

    void FITKAbstractMesh::update()
    {
        //todo
    }

    FITKModelEnum::FITKMeshDim FITKAbstractMesh::getMeshDim()
    {
        //错误值
        return FITKModelEnum::FITKMeshDim::FMDimNone;
    }

    unsigned int FITKAbstractMesh::getMeshDimBit() 
    {
       return 0;
    }

}