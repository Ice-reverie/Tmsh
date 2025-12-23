#include "FITKGmshMshIOReaderTool.h"

#include "FITK_Interface/FITKInterfaceMesh/FITKUnstructuredMeshVTK.h"
#include "FITK_Interface/FITKInterfaceModel/FITKElementFactory.h"
#include "FITK_Interface/FITKInterfaceModel/FITKAbstractElement.h"
#include "FITK_Interface/FITKInterfaceModel/FITKModelSet.h"
#include "FITK_Interface/FITKInterfaceModel/FITKComponentManager.h"

#include "vtkUnstructuredGrid.h"
#include "vtkPoints.h"
#include "vtkIdList.h"
#include "vtkDataSet.h"

#include <QtAlgorithms>
#include <QDebug>

namespace Gmsh
{
    //其他接口
    /**
     * @brief    获取vtk的单元类型
     * @param[i] type 
     * @return   VTKCellType
     * @author   liuzhonghua (liuzhonghuaszch@163.com)
     * @date     2025-09-04
     */
    VTKCellType GetVTKElementType(Interface::FITKModelEnum::FITKEleType type)
    {
        //获取单元类型
        VTKCellType tp = VTK_EMPTY_CELL;
        switch (type)
        {
        case Interface::FITKModelEnum::EleNone: tp = VTK_EMPTY_CELL;
            break;
        case Interface::FITKModelEnum::Line2: tp = VTK_LINE;
            break;
        case Interface::FITKModelEnum::Line3: tp = VTK_QUADRATIC_EDGE;
            break;
        case Interface::FITKModelEnum::Tri3: tp = VTK_TRIANGLE;
            break;
        case Interface::FITKModelEnum::Tri6: tp = VTK_QUADRATIC_TRIANGLE;
            break;
        case Interface::FITKModelEnum::Quad4: tp = VTK_QUAD;
            break;
        case Interface::FITKModelEnum::Quad8: tp = VTK_QUADRATIC_QUAD;
            break;
        case Interface::FITKModelEnum::Tet4: tp = VTK_TETRA;
            break;
        case Interface::FITKModelEnum::Tet10: tp = VTK_QUADRATIC_TETRA;
            break;
        case Interface::FITKModelEnum::Wedge6: tp = VTK_PENTAGONAL_PRISM;
            break;
        case Interface::FITKModelEnum::Hex8: tp = VTK_HEXAHEDRON;
            break;
        case Interface::FITKModelEnum::Hex20: tp = VTK_QUADRATIC_HEXAHEDRON;
            break;
        case Interface::FITKModelEnum::Polygon: tp = VTK_POLYGON;
            break;
        default:
            break;
        }
        return tp;
    }
    /**
     * @brief    判断列表b是否为列表a的子集
     * @param[i] a 
     * @param[i] b 
     * @return   bool
     * @author   liuzhonghua (liuzhonghuaszch@163.com)
     * @date     2025-09-04
     */
    bool IsIdListSubset(vtkIdList* a, vtkIdList* b)
    {
        // 边界条件：若b为空列表，默认是任何列表的子集
        if (!b || b->GetNumberOfIds() == 0)
            return true;

        // 若a为空列表且b非空，则b不可能是a的子集
        if (!a || a->GetNumberOfIds() == 0)
            return false;

        // 遍历b中的每个元素，检查是否都在a中存在
        for (vtkIdType i = 0; i < b->GetNumberOfIds(); i++)
        {
            vtkIdType id = b->GetId(i);
            // vtkIdList::IsId()返回元素在列表中的索引，若不存在则返回-1
            if (a->IsId(id) == -1)
            {
                // 找到b中不在a中的元素，返回false
                return false;
            }
        }

        // 所有元素都存在，返回true
        return true;
    }

    //类的接口实现
    FITKGmshMshIOReaderTool::FITKGmshMshIOReaderTool(Interface::FITKUnstructuredMesh * mesh, Interface::FITKComponentManager * setSurfManager)
    {
        _mesh = mesh;
        _setSurfManager = setSurfManager;
    }
    void FITKGmshMshIOReaderTool::addNode(int id, double x, double y, double z)
    {
        _nodes.append(Gmsh::GmshNode(id, x, y, z));
    }
    void FITKGmshMshIOReaderTool::addElement(int id, int eleType, QList<int> nodeIDs)
    {
        int index = _elements.size();
        _elements.append(Gmsh::GmshElement(id, eleType, nodeIDs));
        _elemMapping.insert(id, index);
    }
    void FITKGmshMshIOReaderTool::addElementSet(int dim, QList<int> elementIDs)
    {
        _elementSets.append(Gmsh::GmshSet(dim, elementIDs));
        if (dim > _maxDim)
            _maxDim = dim;
    }
    void FITKGmshMshIOReaderTool::update(bool isFilterElement)
    {
        if (!_mesh) return;
        if (isFilterElement)
        {
            this->startFilterElement();
        }
        for (int i = 0; i < _nodes.size(); ++i)
        {
            int id = _nodes[i].id;
            double x = _nodes[i].x;
            double y = _nodes[i].y;
            double z = _nodes[i].z;
            _mesh->addNode(id, x, y, z);
        }
        for (int i = 0; i < _elements.size(); ++i)
        {
            int elemID = _elements[i].id;
            Interface::FITKModelEnum::FITKEleType type = Interface::FITKModelEnum::FITKEleType(_elements[i].eleType);
            QList<int> nodeIDs = _elements[i].nodeIDs;
            Interface::FITKAbstractElement* element = Interface::FITKElementFactory::createElement(type);
            if (!element) continue;
            element->setEleID(elemID);
            element->setNodeID(nodeIDs);
            _mesh->appendElement(element);
        }
        for (int i = 0; i < _elementSets.size(); ++i)
        {
            this->createSetData(_elementSets[i].elementIDs);
        }
    }
    void FITKGmshMshIOReaderTool::createSetData(QList<int> elemIdList)
    {
        if (elemIdList.isEmpty() || !_setSurfManager) return;
        QString name = _setSurfManager->checkName("GmshSet-1");
        Interface::FITKModelSet* setData = new Interface::FITKModelSet(Interface::FITKModelEnum::FITKModelSetType::FMSElem);
        setData->setDataObjectName(name);
        setData->setAbsoluteMember(elemIdList);
        _setSurfManager->appendDataObj(setData);
    }
    void FITKGmshMshIOReaderTool::startFilterElement()
    {
        QHash<int, int> nodeMapping;//节点数据
        QHash<int, int> elementMapping;//单元数据
        vtkSmartPointer<vtkUnstructuredGrid> mesh = vtkUnstructuredGrid::New();
        //添加最高维度单元shuju
        this->vtkInitMaxDimElement(mesh, &nodeMapping, &elementMapping);
        //过滤低维数据
        this->vtkFilterLowDimElement(_maxDim - 1, mesh, &nodeMapping, &elementMapping);

        //移除包含的数据
        qSort(_containsLowDimElemenet.begin(), _containsLowDimElemenet.end(), qGreater<int>());
        for (int index : _containsLowDimElemenet)
            _elements.remove(index);
    }
    void FITKGmshMshIOReaderTool::vtkInitMaxDimElement(vtkUnstructuredGrid* mesh, QHash<int, int>* nodeMapping, QHash<int, int>* elementMapping)
    {
        //获取所有的最高单元
        QList<int> maxElementIDs;
        for (int i = 0; i < _elementSets.size(); ++i)
        {
            if (_maxDim != _elementSets[i].dim)continue;
            maxElementIDs.append(_elementSets[i].elementIDs);
        }
        //插入节点
        vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
        for (int i = 0; i < _nodes.size(); ++i)
        {
            int nodeID = points->InsertNextPoint(_nodes[i].x, _nodes[i].y, _nodes[i].z);
            nodeMapping->insert(_nodes[i].id, nodeID);
        }
        mesh->SetPoints(points);
        //插入高维单元
        for (int i = 0; i < maxElementIDs.size(); ++i)
        {
            int eIndex = _elemMapping[maxElementIDs[i]];
            if (_elements[eIndex].nodeIDs.isEmpty())continue;
            Interface::FITKModelEnum::FITKEleType type = Interface::FITKModelEnum::FITKEleType(_elements[eIndex].eleType);
            VTKCellType tp = GetVTKElementType(type);
            if (tp == VTK_EMPTY_CELL) continue;
            vtkSmartPointer<vtkIdList> indexList = vtkSmartPointer<vtkIdList>::New();
            for (int j = 0; j < _elements[eIndex].nodeIDs.size(); ++j)
            {
                int nID = (*nodeMapping)[_elements[eIndex].nodeIDs[j]];
                indexList->InsertNextId(nID);
            }
            int elementID = mesh->InsertNextCell(tp, indexList);
            elementMapping->insert(elementID, _elements[eIndex].id);
        }
    }
    void FITKGmshMshIOReaderTool::vtkFilterLowDimElement(int dim, vtkUnstructuredGrid * mesh, QHash<int, int>* nodeMapping, QHash<int, int>* elementMapping)
    {
        if (dim == 0 || !mesh || !nodeMapping || !elementMapping) return;
        //获取所有该维度单元
        for (int iSet = 0; iSet < _elementSets.size(); ++iSet)
        {
            if (dim != _elementSets[iSet].dim)continue;
            QList<int> maxElementIDs = _elementSets[iSet].elementIDs;
            //遍历该维度单元
            for (int i = maxElementIDs.size() - 1; i >= 0; --i)
            {
                int eIndex = _elemMapping[maxElementIDs[i]];
                if (_elements[eIndex].nodeIDs.isEmpty())continue;
                /**
                 * @brief    过滤低维数据
                 *           获取低维单元上一点，通过该点获取该点周围所有的单元，依次判断周围的单元上的所有点是否包含低维单元上的所有点
                 *           包含则证明该低维单元被高维单元所包含，该低维单元就会被过滤
                 *           不包含，就把该单元存放到mesh里面
                 * @author   liuzhonghua (liuzhonghuaszch@163.com)
                 * @date     2025-09-04
                 */
                vtkSmartPointer<vtkIdList> indexList = vtkSmartPointer<vtkIdList>::New();
                for (int j = 0; j < _elements[eIndex].nodeIDs.size(); ++j)
                {
                    int nID = (*nodeMapping)[_elements[eIndex].nodeIDs[j]];
                    indexList->InsertNextId(nID);
                }
                //通过该点获取该点周围所有的单元
                vtkIdType targetPointId = indexList->GetId(0);
                vtkSmartPointer<vtkIdList> cellIds = vtkSmartPointer<vtkIdList>::New();
                mesh->GetPointCells(targetPointId, cellIds);
                //依次判断周围的单元上的所有点是否包含低维单元上的所有点
                bool isContains = false;
                for (int cindex = 0; cindex < cellIds->GetNumberOfIds(); ++cindex)
                {
                    vtkIdType targetCellId = cellIds->GetId(cindex);
                    vtkSmartPointer<vtkIdList> pointIds = vtkSmartPointer<vtkIdList>::New();
                    mesh->GetCellPoints(targetCellId, pointIds);//获取单元的所有点
                    if (IsIdListSubset(pointIds, indexList))
                    {
                        _containsLowDimElemenet.append(eIndex);
                        _elementSets[iSet].elementIDs.removeAt(i);
                        isContains = true;
                        break;
                    }
                }
                if (isContains) continue;
                //不包含，就把该单元存放到mesh里面
                Interface::FITKModelEnum::FITKEleType type = Interface::FITKModelEnum::FITKEleType(_elements[eIndex].eleType);
                VTKCellType tp = GetVTKElementType(type);
                if (tp == VTK_EMPTY_CELL) continue;

                int elementID = mesh->InsertNextCell(tp, indexList);
                elementMapping->insert(elementID, _elements[eIndex].id);
            }
        }

        mesh->BuildLinks();
        //继续过滤数据
        this->vtkFilterLowDimElement(dim - 1, mesh, nodeMapping, elementMapping);
    }
}


