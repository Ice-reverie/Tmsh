#include "MeshKernel.h"
#include "FITK_Interface/FITKInterfaceModel/FITKComponentManager.h"
#include "FITK_Interface/FITKInterfaceModel/FITKAbstractMesh.h"

namespace ModelData
{

    MeshKernel::MeshKernel()
    {
        _componentManager = new Interface::FITKComponentManager();
        _componentManager->setParentDataID(this->getDataObjectID());
    }

    MeshKernel::~MeshKernel()
    {
        clear();
        if (_componentManager != nullptr)
        {
            delete _componentManager;
            _componentManager = nullptr;
        }
    }

    Interface::FITKModelEnum::AbsModelType MeshKernel::getAbsModelType()
    {
        return Interface::FITKModelEnum::AbsModelType::AMTMesh;
    }

    void MeshKernel::setMesh(Interface::FITKAbstractMesh* mesh)
    {
        if (_mesh != mesh)
        {
            clear();
            _mesh = mesh;
        }

    }


    Interface::FITKAbstractMesh* MeshKernel::getMesh() const
    {
        return _mesh;
    }

    MeshKernel::MeshType MeshKernel::getMeshType() const
    {
        return _meshType;
    }

    void MeshKernel::setMeshType(MeshKernel::MeshType type)
    {
        _meshType = type;
    }

    Interface::FITKComponentManager *MeshKernel::getComponentManager()
    {
        return _componentManager;
    }

    void MeshKernel::clear()
    {
        if (_mesh != nullptr)
        {
            delete _mesh;
            _mesh = nullptr;
        }
        _componentManager->clear();
    }
}
