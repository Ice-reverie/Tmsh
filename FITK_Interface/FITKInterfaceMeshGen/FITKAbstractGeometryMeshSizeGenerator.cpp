#include "FITKAbstractGeometryMeshSizeGenerator.h"

namespace Interface
{
    FITKAbstractGeometryMeshSizeGenerator::~FITKAbstractGeometryMeshSizeGenerator()
    {
        //纯虚函数
    }

    FITKGeometryMeshSize* FITKAbstractGeometryMeshSizeGenerator::createGeometryMeshSize()
    {
        //默认值
        return new FITKGeometryMeshSize;
    }

}


