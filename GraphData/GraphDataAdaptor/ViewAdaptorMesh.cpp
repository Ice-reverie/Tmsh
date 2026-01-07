#include "ViewAdaptorMesh.h"
#include "GraphObjectMesh.h"

#include "ModelData/MeshKernel.h"

namespace Graph
{
    bool ViewAdaptorMesh::adapt()
    {
        ModelData::MeshKernel* meshKernel = dynamic_cast<ModelData::MeshKernel*>(_dataObj);
        if (meshKernel == nullptr) {
            return false;
        }

        if(_outputData){
            delete _outputData;
            _outputData = nullptr;
        }

        GraphObjectMesh* graphObj = new GraphObjectMesh(meshKernel);

        if (graphObj->getActorCount() == 0) {
            delete graphObj;
            graphObj = nullptr;
        }

        _outputData = graphObj;
        return true;
    }
}
