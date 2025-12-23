#include "MeshData2INPConverter.h"
#include "MeshManager.h"
#include "MeshData.h"
#include "MeshKernel.h"
#include "FITK_Component/FITKAbaqusData/FITKDataCase.h"
#include "FITK_Component/FITKAbaqusData/FITKAbaqusPart.h"
#include "FITK_Interface/FITKInterfaceModel/FITKUnstructuredMesh.h"
#include "FITK_Interface/FITKInterfaceModel/FITKStructuredMesh.h"
#include "FITK_Interface/FITKInterfaceModel/FITKAbstractElement.h"
#include "FITK_Interface/FITKInterfaceStructural/FITKAbaMeshPartVTK.h"
#include "FITK_Kernel/FITKCore/FITKVec3D.h"
#include "FITK_Kernel/FITKCore/FITKEnumTransformer.hpp"
#include "FITK_Kernel/FITKAppFramework/FITKMessage.h"
#include <QHash>

namespace ModelData
{
    /**
     * @brief 建立单元类型映射
     * @author LiBaojun (libaojunqd@foxmail.com)
     * @date 2024-03-30
     */
    static const QHash<Interface::FITKModelEnum::FITKEleType, Interface::FITKAbaMeshPartVTK::AbaEleType>  AbaElementTypeReverseHash =
    {
        { Interface::FITKModelEnum::FITKEleType::Line2, Interface::FITKAbaMeshPartVTK::AbaEleType::B31 },
        { Interface::FITKModelEnum::FITKEleType::Line3, Interface::FITKAbaMeshPartVTK::AbaEleType::B32 },
        { Interface::FITKModelEnum::FITKEleType::Tri3, Interface::FITKAbaMeshPartVTK::AbaEleType::S3 },
        { Interface::FITKModelEnum::FITKEleType::Quad4, Interface::FITKAbaMeshPartVTK::AbaEleType::S4 },
        { Interface::FITKModelEnum::FITKEleType::Quad8, Interface::FITKAbaMeshPartVTK::AbaEleType::S8R },
        { Interface::FITKModelEnum::FITKEleType::Tet4, Interface::FITKAbaMeshPartVTK::AbaEleType::C3D4 },
        { Interface::FITKModelEnum::FITKEleType::Tet10, Interface::FITKAbaMeshPartVTK::AbaEleType::C3D10 },
        { Interface::FITKModelEnum::FITKEleType::Wedge6, Interface::FITKAbaMeshPartVTK::AbaEleType::C3D6 },
        { Interface::FITKModelEnum::FITKEleType::Hex8, Interface::FITKAbaMeshPartVTK::AbaEleType::C3D8 },
        { Interface::FITKModelEnum::FITKEleType::Hex20, Interface::FITKAbaMeshPartVTK::AbaEleType::C3D20 },
    };
 
    MeshData2INPConverter::MeshData2INPConverter(MeshManager* mgr, AbaqusData::FITKDataCase* caseData)
        : _meshManager(mgr), _caseData(caseData)
    { 
    }

    bool MeshData2INPConverter::execDataConversion()
    {
        if (_meshManager == nullptr || _caseData == nullptr) return false;

        // Perform data conversion logic here
        int kernelCount = 0;
        const int dataCount = _meshManager->getDataCount(); 
        for (int i = 0; i < dataCount; ++i)
        {
            MeshData* meshData = _meshManager->getDataByIndex(i);
            if (meshData == nullptr) continue;

            const int meshKernelCount = meshData->getDataCount();
            for (int j = 0; j < meshKernelCount; ++j)
            {
                MeshKernel* meshKernel = meshData->getDataByIndex(j);
                if (meshKernel == nullptr) continue;
                kernelCount++;
                AbaqusData::FITKAbaqusPart* abaqusPart = new AbaqusData::FITKAbaqusPart();
                // Convert MeshKernel to FITKAbaqusPart
                bool conversionSuccess = convertMeshKernelToAbaqusPart(meshKernel, abaqusPart,kernelCount);

                if (conversionSuccess)
                    _caseData->getPartManager()->appendDataObj(abaqusPart);
                else
                    delete abaqusPart;
                
            }
        }
        // Implement the data conversion logic here
        return kernelCount > 0; // Return true if conversion is successful
    }
    bool MeshData2INPConverter::convertMeshKernelToAbaqusPart(MeshKernel *kernel, 
                                    AbaqusData::FITKAbaqusPart *abaqusPart, int index)
    {
        if (kernel == nullptr || abaqusPart == nullptr) return false;
        // Set the name for the Abaqus part
        QString partName = kernel->getDataObjectName();
        if (partName.isEmpty())
            partName = QString("Part_%1").arg(index);
        abaqusPart->setDataObjectName(partName);
        
         // 获取模型网格数据
        Interface::FITKAbstractMesh* dataModelMesh = kernel->getMesh();
        if (dataModelMesh == nullptr) return false;
        Interface::FITKModelEnum::AbsModelType modelType = dataModelMesh->getAbsModelType();

        // 根据模型类型设置Abaqus部件的属性
        switch (modelType)
        {
        case Interface::FITKModelEnum::AbsModelType::AMTunstructuredMesh:
        case Interface::FITKModelEnum::AbsModelType::AMTunstructuredMeshvtk:
            // 处理非结构化网格
           this->convertUnstructuredMeshToAbaqusPart(dataModelMesh, abaqusPart);
            break;
        case Interface::FITKModelEnum::AbsModelType::AMTStructuredMesh:
            // 处理结构化网格
            this->convertStructuredMeshToAbaqusPart(dataModelMesh, abaqusPart);
            break;
        }

        return true;
    }
    void MeshData2INPConverter::convertUnstructuredMeshToAbaqusPart(Interface::FITKAbstractMesh *mesh, AbaqusData::FITKAbaqusPart *abaqusPart)
    {
        if (mesh == nullptr || abaqusPart == nullptr) return;

        // 获取网格数据
        Interface::FITKUnstructuredMesh* unstructuredMesh = dynamic_cast<Interface::FITKUnstructuredMesh*>(mesh);
        if (unstructuredMesh == nullptr) return;
        // 转换网格数据到Abaqus部件
        Interface::FITKAbaMeshPartVTK* abaqusMesh = abaqusPart->getMesh();
        if (abaqusMesh == nullptr) return;

        // 进行具体的转换逻辑 
        // 转换节点数据
        const int nodeCount = unstructuredMesh->getNodeCount();
        for (int i = 0; i < nodeCount; i++)
        {
            Core::FITKNode* node = unstructuredMesh->getNodeAt(i);
            if (node == nullptr) continue;
            double coor[3] = { 0.0, 0.0, 0.0 };
            node->getCoor(coor);
            abaqusMesh->addNode(node->getNodeID(), coor[0], coor[1], coor[2]);
        }
         // 转换单元数据
        const int elementCount = unstructuredMesh->getElementCount();
        for (int i = 0; i < elementCount; i++)
        {
            Interface::FITKAbstractElement* element = unstructuredMesh->getElementAt(i);
            if (element == nullptr ) continue;
            Interface::FITKAbaMeshPartVTK::AbaEleType abaEleType = AbaElementTypeReverseHash.value(
                element->getEleType(), Interface::FITKAbaMeshPartVTK::AbaEleType::AbaEleNone);
            // 如果单元类型不在映射中，则跳过
            if (abaEleType == Interface::FITKAbaMeshPartVTK::AbaEleType::AbaEleNone)
            {
                bool OK= false;
                Core::FITKEnumTransfer<Interface::FITKModelEnum::FITKEleType> eleTypeTransfer;
                QString eleTypeString = eleTypeTransfer.toString(element->getEleType(), OK);
                // 这里输出警告信息
                AppFrame::FITKMessageWarning(QString("Element type %1 is not supported Skipping element ID: %2")
                                     .arg(eleTypeString).arg(element->getEleID()));
                continue;
            }

            abaqusMesh->appendElement(abaEleType, element->getEleID(),  element->getAllNodes());
        }

    }
    void MeshData2INPConverter::convertStructuredMeshToAbaqusPart(Interface::FITKAbstractMesh *mesh, AbaqusData::FITKAbaqusPart *abaqusPart)
    {
        //todo
    }
} // namespace ModelData
