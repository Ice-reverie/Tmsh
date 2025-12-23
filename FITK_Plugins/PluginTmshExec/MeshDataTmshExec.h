#ifndef _MESH_DATA_TMSHEXEC_H_
#define _MESH_DATA_TMSHEXEC_H_

#include "ModelData/MeshData.h"
#include "PluginTmshExecAPI.h"

namespace ModelData
{
    class PluginTmshExecAPI MeshDataTmshExec : public MeshData
    {
    public:
        explicit MeshDataTmshExec();
        virtual ~MeshDataTmshExec();
    };
}

#endif

