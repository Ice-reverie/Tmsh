#ifndef _MESH_DATA_GMSHEXEC_H_
#define _MESH_DATA_GMSHEXEC_H_

#include "ModelData/MeshData.h"
#include "PluginGmshExecAPI.h"

namespace ModelData
{
    /**
     * @brief Gmsh网格数据类
     * @details 继承自MeshData，用于存储和管理Gmsh网格数据
     */
    class PluginGmshExecAPI MeshDataGmshExec : public MeshData
    {
    public:
        
        explicit MeshDataGmshExec();
         
        virtual ~MeshDataGmshExec();

    };
}

#endif 
