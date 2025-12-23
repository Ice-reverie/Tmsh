#include "GlobalDataFactory.h"
#include "ModelData/GeometryManager.h"
#include "ModelData/MeshManager.h"

Core::FITKAbstractDataObject *GlobalDataFactory::createMeshData()
{
    return new ModelData::MeshManager;
}

Core::FITKAbstractDataObject* GlobalDataFactory::createGeoData()
{
    // 创建几何数据
    return new ModelData::GeometryManager;
}

Core::FITKAbstractDataObject *GlobalDataFactory::createPhysicsData()
{
    // 创建算例求解的物理场数据
    return nullptr;
}

Core::FITKAbstractDataObject *GlobalDataFactory::createPostData()
{
    // 创建后处理数据
    return nullptr;
}

QHash<int, Core::FITKAbstractDataObject *> GlobalDataFactory::createOtherData()
{
    // 不创建其他数据
    return QHash<int, Core::FITKAbstractDataObject *>();
}
