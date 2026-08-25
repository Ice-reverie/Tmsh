#include "GraphObjectMesh.h"
#include "MeshQualityColorMapper.h"

#include "ModelData/MeshKernel.h"

#include "FITK_Kernel/FITKAppFramework/FITKMessage.h"
#include "FITK_Kernel/FITKCore/FITKDataRepo.h"
#include "FITK_Kernel/FITKCore/FITKVec3D.h"
#include "FITK_Interface/FITKInterfaceModel/FITKAbstractMesh.h"
#include "FITK_Interface/FITKInterfaceModel/FITKAbstractModelComponent.h"
#include "FITK_Interface/FITKInterfaceModel/FITKStructuredMesh.h"
#include "FITK_Interface/FITKInterfaceModel/FITKUnstructuredMesh.h"
#include "FITK_Interface/FITKInterfaceModel/FITKAbstractElement.h"
#include "FITK_Interface/FITKInterfaceModel/FITKComponentManager.h"
#include "FITK_Interface/FITKInterfaceModel/FITKModelSet.h"
#include "FITK_Interface/FITKInterfaceMesh/FITKUnstructuredMeshVTK.h"
#include "FITK_Interface/FITKVTKAlgorithm/FITKGraphActor.h"

#include <vtkSmartPointer.h>
#include <vtkUnstructuredGrid.h>
#include <vtkStructuredGrid.h>
#include <vtkCellData.h>
#include <vtkUnsignedCharArray.h>
#include <vtkPointData.h>
#include <vtkIntArray.h>
#include <vtkProperty.h>
#include <vtkMapper.h>
#include <vtkMultiBlockDataSet.h>
#include <vtkExtractBlock.h>
#include <vtkCompositeDataGeometryFilter.h>

namespace Graph
{
    GraphObjectMesh::GraphObjectMesh(ModelData::MeshKernel * meshKernel):
        GraphObjectBase(meshKernel)
    {
        _renderLayer = 1;
        init();

        initActor();
    }

    GraphObjectMesh::~GraphObjectMesh()
    {
        deleteVTKObj(_meshData);
        deleteVTKObj(_comData);
    }

    void GraphObjectMesh::update(bool forceUpdate)
    {
        updateVisibility();

        if (forceUpdate == false) {
            return;
        }

        updateGraphData();
    }

    void GraphObjectMesh::updateVisibility()
    {
        ModelData::MeshKernel* meshKernel = dynamic_cast<ModelData::MeshKernel*>(_dataObj);
        if (meshKernel == nullptr) {
            return;
        }

        bool meshKernelVis = getVisibility();
        bool meshVis = false;
        if (meshKernel->getMesh()) {
            meshVis = meshKernel->getMesh()->isEnable();
        }

        _meshActor->SetVisibility(meshKernelVis && meshVis);
        _comActor->SetVisibility(meshKernelVis);
    }

    void GraphObjectMesh::updateSubColor(QColor color, QVariant otherPar)
    {
        if (_comData == nullptr) {
            return;
        }

        int subId = otherPar.toInt();
        Interface::FITKAbstractModelComponent* com = FITKDATAREPO->getTDataByID<Interface::FITKAbstractModelComponent>(subId);
        if (com == nullptr) {
            return;
        }

        if (!color.isValid()) {
            color = com->getColor();
        }

        //分组网格颜色数组获取
        vtkUnsignedCharArray* meshColorArray = vtkUnsignedCharArray::SafeDownCast(
            _comData->GetCellData()->GetArray(GraphObjectCommons::_dataColorArrayName.toUtf8().constData()));

        //获取要修改的vtk单元id
        QList<int> changeColorCellIds = getVTKCellIDsByComID(subId);
        for (int id : changeColorCellIds) {
            meshColorArray->SetTuple3(id, color.red(), color.green(), color.blue());
        }
    }

    void GraphObjectMesh::updateModelColor(QColor color)
    {
        if (_meshData == nullptr) {
            return;
        }

        if (!color.isValid()) {
            color = GraphObjectCommons::_meshFaceDefaultColor;
        }

        //网格颜色修改
        vtkUnsignedCharArray* meshColorArray = vtkUnsignedCharArray::SafeDownCast(
            _meshData->GetCellData()->GetArray(GraphObjectCommons::_dataColorArrayName.toUtf8().constData()));
        meshColorArray->SetNumberOfTuples(0);
        for (int cellId = 0; cellId < _meshData->GetNumberOfCells(); cellId++) {
            Q_UNUSED(cellId);
            meshColorArray->InsertNextTuple3(color.red(), color.green(), color.blue());
        }
    }

    QList<int> GraphObjectMesh::getVTKCellIDsByComID(int comID)
    {
        QList<int> vtkCellIdList = {};
        Interface::FITKAbstractModelComponent* com = FITKDATAREPO->getTDataByID<Interface::FITKAbstractModelComponent>(comID);
        if (com == nullptr || com->getAbstractModel() == nullptr) {
            return vtkCellIdList;
        }

        //分组网格id映射关系获取
        vtkIntArray* meshCellIdArray = vtkIntArray::SafeDownCast(
            _comData->GetCellData()->GetArray(GraphObjectCommons::_dataCellIDArrayName.toUtf8().constData()));

        switch (com->getAbstractModel()->getAbsModelType()) {
        case Interface::FITKModelEnum::AMTunstructuredMeshvtk:
        case Interface::FITKModelEnum::AMTunstructuredMesh: {
            Interface::FITKUnstructuredMesh* unStrMesh = dynamic_cast<Interface::FITKUnstructuredMesh*>(com->getAbstractModel());
            if (unStrMesh == nullptr) {
                return vtkCellIdList;
            }

            //获取每个FITK单元对应的VTK单元
            for (int i = 0; i < unStrMesh->getElementCount(); i++) {
                int eleId = unStrMesh->getEleIDByIndex(i);
                for (int j = 0; j < meshCellIdArray->GetNumberOfTuples(); j++) {
                    if (meshCellIdArray->GetValue(j) != eleId) {
                        continue;
                    }
                    vtkCellIdList.append(j);
                    break;
                }
            }
            break;
        }
        case Interface::FITKModelEnum::AMTStructuredMesh: {

        }
        }
        return vtkCellIdList;
    }

    void GraphObjectMesh::init()
    {
        ModelData::MeshKernel* meshKernel = dynamic_cast<ModelData::MeshKernel*>(_dataObj);
        if (meshKernel == nullptr || meshKernel->getMesh() == nullptr) {
            return;
        }

        switch (meshKernel->getMesh()->getAbsModelType()){
        case Interface::FITKModelEnum::AMTStructuredMesh:_meshData = vtkStructuredGrid::New(); break;
        default:_meshData = vtkUnstructuredGrid::New(); break;
        }

        //初始化分区数据
        _comData = vtkUnstructuredGrid::New();
    }

    bool GraphObjectMesh::updateGraphData()
    {
        //AppFrame::FITKMessageNormal(QString("Graph 3D"));

        ModelData::MeshKernel* meshKernel = dynamic_cast<ModelData::MeshKernel*>(_dataObj);
        if (_meshData == nullptr || _comData == nullptr || meshKernel == nullptr) {
            return false;
        }

        //清空数据
        _meshData->Initialize();
        resetVtkObj(_comData);

        //生成网格数据
        getMesh(meshKernel->getMesh(), _meshData);

        //AppFrame::FITKMessageNormal(QString("Graph 3D over"));

        //设置网格单元颜色
        vtkSmartPointer<vtkUnsignedCharArray> colorArray = vtkUnsignedCharArray::SafeDownCast(
            _meshData->GetCellData()->GetArray(GraphObjectCommons::_dataColorArrayName.toUtf8().constData()));
        if (colorArray == nullptr) {
            colorArray = vtkSmartPointer<vtkUnsignedCharArray>::New();
            colorArray->SetName(GraphObjectCommons::_dataColorArrayName.toUtf8().constData());
            colorArray->SetNumberOfComponents(3);
            _meshData->GetCellData()->SetScalars(colorArray);
        }
        colorArray->SetNumberOfTuples(0);

        //单元设置颜色
        QColor meshColor = GraphObjectCommons::_meshFaceDefaultColor;
        for (int cellId = 0; cellId < _meshData->GetNumberOfCells(); cellId++) {
            Q_UNUSED(cellId);
            colorArray->InsertNextTuple3(meshColor.red(), meshColor.green(), meshColor.blue());
        }

        //生成分组数据
        vtkSmartPointer<vtkMultiBlockDataSet> blockData = vtkSmartPointer<vtkMultiBlockDataSet>::New();
        vtkSmartPointer<vtkExtractBlock> extractBlock = vtkSmartPointer<vtkExtractBlock>::New();
        extractBlock->SetInputData(blockData);
        vtkSmartPointer<vtkCompositeDataGeometryFilter> compFilter = vtkSmartPointer<vtkCompositeDataGeometryFilter>::New();
        compFilter->SetInputConnection(extractBlock->GetOutputPort());

        for (int comIndex = 0; comIndex < meshKernel->getComponentManager()->getDataCount(); comIndex++) {
            Interface::FITKAbstractModelComponent* modelCom = meshKernel->getComponentManager()->getDataByIndex(comIndex);
            if (modelCom == nullptr) {
                continue;
            }
            Interface::FITKAbstractModel* model = modelCom->getAbstractModel();
            if (model == nullptr) {
                continue;
            }

            //判断分组类型
            vtkSmartPointer<vtkDataSet> dataSet = nullptr;
            switch (model->getAbsModelType()) {
            case Interface::FITKModelEnum::AMTStructuredMesh: {
                vtkSmartPointer<vtkStructuredGrid> grid = vtkSmartPointer<vtkStructuredGrid>::New();
                grid->SetPoints(vtkSmartPointer<vtkPoints>::New());
                getStrComponent(modelCom, grid);
                dataSet = grid;
                break;
            }
            default: {
                vtkSmartPointer<vtkUnstructuredGrid> grid = vtkSmartPointer<vtkUnstructuredGrid>::New();
                grid->SetPoints(vtkSmartPointer<vtkPoints>::New());
                getUnstrComponent(modelCom, grid);
                dataSet = grid;
                break;
            }
            }

            blockData->SetBlock(comIndex, dataSet);
            extractBlock->AddIndex(comIndex);
        }
        compFilter->Update();

        vtkDataSet* comTempData = compFilter->GetOutput();
        if (comTempData == nullptr) {
            return false;
        }
        _comData->DeepCopy(comTempData);

        //AppFrame::FITKMessageNormal(QString("Graph over"));

        return true;
    }

    void GraphObjectMesh::initActor()
    {
        double meshGraphOffSet = 0.0;
        ModelData::MeshKernel* meshKernel = dynamic_cast<ModelData::MeshKernel*>(_dataObj);
        if (meshKernel == nullptr) {
            return;
        }

        switch (meshKernel->getMeshType()){
        case ModelData::MeshKernel::D2: meshGraphOffSet = GraphObjectCommons::_mesh2DGraphOffset; break;
        case ModelData::MeshKernel::D3: meshGraphOffSet = GraphObjectCommons::_mesh3DGraphOffset; break;
        case ModelData::MeshKernel::Mixed: meshGraphOffSet = GraphObjectCommons::_mesh3DGraphOffset; break;
        }

        _meshActor = FITKGraphActor::New();
        _meshActor->SetPickable(false);
        _meshActor->setGraphObject(this);
        _meshActor->GetProperty()->SetRepresentation(VTK_SURFACE);
        _meshActor->GetProperty()->SetEdgeVisibility(true);
        _meshActor->setInputDataObject(_meshData);
        _meshActor->GetMapper()->SetScalarModeToUseCellFieldData();
        _meshActor->GetMapper()->SelectColorArray(GraphObjectCommons::_dataColorArrayName.toUtf8().constData());
        _meshActor->setRelativeCoincidentTopologyPolygonOffsetParameters(meshGraphOffSet);
        addActor(_meshActor);

        _comActor = FITKGraphActor::New();
        _comActor->SetPickable(false);
        _comActor->setGraphObject(this);
        _comActor->GetProperty()->SetRepresentation(VTK_SURFACE);
        _comActor->GetProperty()->SetEdgeVisibility(true);
        _comActor->setInputDataObject(_comData);
        _comActor->GetMapper()->SetScalarModeToUseCellFieldData();
        _comActor->GetMapper()->SelectColorArray(GraphObjectCommons::_dataColorArrayName.toUtf8().constData());
        _comActor->setRelativeCoincidentTopologyPolygonOffsetParameters(meshGraphOffSet);
        addActor(_comActor);
    }

    void GraphObjectMesh::getMesh(Interface::FITKAbstractModel* absModel, vtkDataSet* dataSet)
    {
        if (dataSet == nullptr || absModel == nullptr) {
            return;
        }

        switch (absModel->getAbsModelType()) {
        case Interface::FITKModelEnum::AMTunstructuredMeshvtk: createUnStrGrid(absModel, dataSet); break;
        case Interface::FITKModelEnum::AMTunstructuredMesh: createUnStrGrid(absModel, dataSet); break;
        case Interface::FITKModelEnum::AMTStructuredMesh: createStrGrid(absModel, dataSet); break;
        }
    }

    void GraphObjectMesh::getUnstrComponent(Interface::FITKAbstractModelComponent * compoment, vtkUnstructuredGrid * dataSet)
    {
        Interface::FITKUnstructuredMesh* unStrMesh = dynamic_cast<Interface::FITKUnstructuredMesh*>(compoment->getAbstractModel());
        if (unStrMesh == nullptr) {
            return;
        }

        if (dataSet == nullptr || compoment == nullptr) {
            return;
        }

        vtkPoints* points = dataSet->GetPoints();
        if (points == nullptr) {
            return;
        }

        //转为为集合
        Interface::FITKModelSet* modelSet = dynamic_cast<Interface::FITKModelSet*>(compoment);
        if (modelSet == nullptr) {
            return;
        }

        //获取分组所有网格单元
        QList<int> cellIDs = modelSet->getAbsoluteMember();
        for (int cellID : cellIDs) {
            Interface::FITKAbstractElement* ele = unStrMesh->getElementByID(cellID);
            if (ele == nullptr)continue;

            //创建单元
            vtkSmartPointer<vtkIdList> cell = vtkSmartPointer<vtkIdList>::New();

            for (int pointID : ele->getAllNodes()) {
                double pos3[3]{ 0., 0., 0. };
                Core::FITKNode* node = unStrMesh->getNodeByID(pointID);
                if (node == nullptr) {
                    continue;
                }
                node->getCoor(pos3);

                int iPt = points->InsertNextPoint(pos3);
                cell->InsertNextId(iPt);
            }

            //单元类型判断
            int cellID = -1;
            switch (ele->getEleType()) {
            case Interface::FITKModelEnum::EleNone:break;
            case Interface::FITKModelEnum::Line2:cellID = dataSet->InsertNextCell(VTKCellType::VTK_LINE, cell); break;
            case Interface::FITKModelEnum::Line3:cellID = dataSet->InsertNextCell(VTKCellType::VTK_POLY_LINE, cell); break;
            case Interface::FITKModelEnum::Tri3:cellID = dataSet->InsertNextCell(VTKCellType::VTK_TRIANGLE, cell); break;
            case Interface::FITKModelEnum::Tri6:cellID = dataSet->InsertNextCell(VTKCellType::VTK_QUADRATIC_TRIANGLE, cell); break;
            case Interface::FITKModelEnum::Quad4:cellID = dataSet->InsertNextCell(VTKCellType::VTK_QUAD, cell); break;
            case Interface::FITKModelEnum::Quad8:cellID = dataSet->InsertNextCell(VTKCellType::VTK_QUADRATIC_QUAD, cell); break;
            case Interface::FITKModelEnum::Tet4:cellID = dataSet->InsertNextCell(VTKCellType::VTK_TETRA, cell); break;
            case Interface::FITKModelEnum::Tet10:cellID = dataSet->InsertNextCell(VTKCellType::VTK_QUADRATIC_TETRA, cell); break;
            case Interface::FITKModelEnum::Wedge6:cellID = dataSet->InsertNextCell(VTKCellType::VTK_WEDGE, cell); break;
            case Interface::FITKModelEnum::Hex8:cellID = dataSet->InsertNextCell(VTKCellType::VTK_HEXAHEDRON, cell); break;
            case Interface::FITKModelEnum::Hex20:cellID = dataSet->InsertNextCell(VTKCellType::VTK_QUADRATIC_HEXAHEDRON, cell); break;
            case Interface::FITKModelEnum::Polygon:cellID = dataSet->InsertNextCell(VTKCellType::VTK_POLYGON, cell); break;
            }
        }

        vtkCellData* cellData = dataSet->GetCellData();
        if (cellData == nullptr) {
            return;
        }

        //设置单元颜色
        vtkSmartPointer<vtkUnsignedCharArray> colorArray =vtkUnsignedCharArray::SafeDownCast(
            cellData->GetArray(GraphObjectCommons::_dataColorArrayName.toUtf8().constData()));
        if (colorArray == nullptr) {
            colorArray = vtkSmartPointer<vtkUnsignedCharArray>::New();
            colorArray->SetName(GraphObjectCommons::_dataColorArrayName.toUtf8().constData());
            colorArray->SetNumberOfComponents(3);
            cellData->SetScalars(colorArray);
        }
        colorArray->SetNumberOfTuples(0);

        //单元设置颜色
        QColor comColor = compoment->getColor();
        for (int cellId = 0; cellId < dataSet->GetNumberOfCells(); cellId++) {
            Q_UNUSED(cellId);
            colorArray->InsertNextTuple3(comColor.red(), comColor.green(), comColor.blue());
        }
    }

    void GraphObjectMesh::getStrComponent(Interface::FITKAbstractModelComponent * compoment, vtkStructuredGrid * dataSet)
    {
        if (dataSet == nullptr) {
            return;
        }
        Interface::FITKStructuredMesh* strMesh = dynamic_cast<Interface::FITKStructuredMesh*>(dataSet);
        if (strMesh == nullptr) {
            return;
        }

        vtkSmartPointer<vtkStructuredGrid> grid = vtkSmartPointer<vtkStructuredGrid>::New();
        vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
        grid->SetPoints(points);

        //点id存储数值
        vtkSmartPointer<vtkIntArray> pointIDArray = vtkSmartPointer<vtkIntArray>::New();
        pointIDArray->SetName(GraphObjectCommons::_dataPointIDArrayName.toUtf8().constData());
        pointIDArray->SetNumberOfComponents(1);
        grid->GetPointData()->AddArray(pointIDArray);
        //填充点
        for (int i = 0; i < strMesh->getNodeCount(); i++) {
            double pos3[3]{ 0., 0., 0. };
            Core::FITKNode* node = strMesh->getNodeAt(i);
            if (node == nullptr) {
                continue;
            }
            node->getCoor(pos3);

            int iPt = points->InsertNextPoint(pos3);
            //存储点id
            pointIDArray->InsertNextValue(node->getNodeID());
        }
        //设置维度
        int ijk[3] = { 0,0,0 };
        strMesh->getIJKDim(ijk);
        grid->SetDimensions(ijk);

        if (grid) {
            dataSet->DeepCopy(grid);
        }
    }

    void GraphObjectMesh::createUnStrGridVKT(Interface::FITKAbstractModel* absModel, vtkDataSet* dataSet)
    {
        if (dataSet == nullptr) {
            return;
        }

        Interface::FITKUnstructuredMeshVTK* unStrMeshVTK = dynamic_cast<Interface::FITKUnstructuredMeshVTK*>(absModel);
        if (unStrMeshVTK == nullptr || unStrMeshVTK->getGrid() == nullptr) {
            return;
        }

        dataSet->DeepCopy(unStrMeshVTK->getGrid());
    }

    void GraphObjectMesh::createUnStrGrid(Interface::FITKAbstractModel* absModel, vtkDataSet* dataSet)
    {
        if (dataSet == nullptr) {
            return;
        }

        Interface::FITKUnstructuredMesh* unStrMesh = dynamic_cast<Interface::FITKUnstructuredMesh*>(absModel);
        if (unStrMesh == nullptr) {
            return;
        }

        //初始化网格
        vtkSmartPointer<vtkUnstructuredGrid> grid = vtkSmartPointer<vtkUnstructuredGrid>::New();
        vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
        grid->SetPoints(points);
        //点id存储数值
        vtkSmartPointer<vtkIntArray> pointIDArray = vtkSmartPointer<vtkIntArray>::New();
        pointIDArray->SetName(GraphObjectCommons::_dataPointIDArrayName.toUtf8().constData());
        pointIDArray->SetNumberOfComponents(1);
        grid->GetPointData()->AddArray(pointIDArray);

        //单元id存储数据
        vtkSmartPointer<vtkIntArray> cellIDArray = vtkSmartPointer<vtkIntArray>::New();
        cellIDArray->SetName(GraphObjectCommons::_dataCellIDArrayName.toUtf8().constData());
        cellIDArray->SetNumberOfComponents(1);
        grid->GetCellData()->AddArray(cellIDArray);

        //单元填充
        for (int index = 0; index < unStrMesh->getElementCount(); index++) {
            Interface::FITKAbstractElement* ele = unStrMesh->getElementAt(index);
            if (ele == nullptr)continue;

            //创建单元
            vtkSmartPointer<vtkIdList> cell = vtkSmartPointer<vtkIdList>::New();

            for (int pointID : ele->getAllNodes()) {
                double pos3[3]{ 0., 0., 0. };
                Core::FITKNode* node = unStrMesh->getNodeByID(pointID);
                if (node == nullptr) {
                    continue;
                }
                node->getCoor(pos3);

                int iPt = points->InsertNextPoint(pos3);
                //存储点id
                pointIDArray->InsertNextValue(node->getNodeID());
                cell->InsertNextId(iPt);
            }

            //单元类型判断
            int cellID = -1;
            switch (ele->getEleType()) {
            case Interface::FITKModelEnum::EleNone:break;
            case Interface::FITKModelEnum::Line2:cellID = grid->InsertNextCell(VTKCellType::VTK_LINE, cell); break;
            case Interface::FITKModelEnum::Line3:cellID = grid->InsertNextCell(VTKCellType::VTK_POLY_LINE, cell); break;
            case Interface::FITKModelEnum::Tri3:cellID = grid->InsertNextCell(VTKCellType::VTK_TRIANGLE, cell); break;
            case Interface::FITKModelEnum::Tri6:cellID = grid->InsertNextCell(VTKCellType::VTK_QUADRATIC_TRIANGLE, cell); break;
            case Interface::FITKModelEnum::Quad4:cellID = grid->InsertNextCell(VTKCellType::VTK_QUAD, cell); break;
            case Interface::FITKModelEnum::Quad8:cellID = grid->InsertNextCell(VTKCellType::VTK_QUADRATIC_QUAD, cell); break;
            case Interface::FITKModelEnum::Tet4:cellID = grid->InsertNextCell(VTKCellType::VTK_TETRA, cell); break;
            case Interface::FITKModelEnum::Tet10:cellID = grid->InsertNextCell(VTKCellType::VTK_QUADRATIC_TETRA, cell); break;
            case Interface::FITKModelEnum::Wedge6:cellID = grid->InsertNextCell(VTKCellType::VTK_WEDGE, cell); break;
            case Interface::FITKModelEnum::Hex8:cellID = grid->InsertNextCell(VTKCellType::VTK_HEXAHEDRON, cell); break;
            case Interface::FITKModelEnum::Hex20:cellID = grid->InsertNextCell(VTKCellType::VTK_QUADRATIC_HEXAHEDRON, cell); break;
            case Interface::FITKModelEnum::Polygon:cellID = grid->InsertNextCell(VTKCellType::VTK_POLYGON, cell); break;
            }

            //存储单元id
            if (cellID != -1) {
                cellIDArray->InsertNextValue(ele->getEleID());
            }
        }

        if (grid) {
            dataSet->DeepCopy(grid);
        }
    }

    void GraphObjectMesh::createStrGrid(Interface::FITKAbstractModel* absModel, vtkDataSet* dataSet)
    {
        if (dataSet == nullptr) {
            return;
        }

        Interface::FITKStructuredMesh* strMesh = dynamic_cast<Interface::FITKStructuredMesh*>(absModel);
        if (strMesh == nullptr) {
            return;
        }
        //结构化网格初始化
        vtkSmartPointer<vtkStructuredGrid> grid = vtkSmartPointer<vtkStructuredGrid>::New();
        vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
        grid->SetPoints(points);
        //点id存储数值
        vtkSmartPointer<vtkIntArray> pointIDArray = vtkSmartPointer<vtkIntArray>::New();
        pointIDArray->SetName(GraphObjectCommons::_dataPointIDArrayName.toUtf8().constData());
        pointIDArray->SetNumberOfComponents(1);
        grid->GetPointData()->AddArray(pointIDArray);
        //填充点
        for (int i = 0; i < strMesh->getNodeCount(); i++) {
            double pos3[3]{ 0., 0., 0. };
            Core::FITKNode* node = strMesh->getNodeAt(i);
            if (node == nullptr) {
                continue;
            }
            node->getCoor(pos3);

            int iPt = points->InsertNextPoint(pos3);
            //存储点id
            pointIDArray->InsertNextValue(node->getNodeID());
        }
        //设置维度
        int ijk[3] = { 0,0,0 };
        strMesh->getIJKDim(ijk);
        grid->SetDimensions(ijk);

        if (grid) {
            dataSet->DeepCopy(grid);
        }
    }
    
    bool GraphObjectMesh::computeQualityRange(Interface::QualityMetric qualityMetric,
                                              double& minValue,
                                              double& maxValue) const
    {
        if (_meshData == nullptr) {
            AppFrame::FITKMessageWarning(QStringLiteral("computeQualityRange: _meshData为空"));
            return false;
        }

        ModelData::MeshKernel* meshKernel = dynamic_cast<ModelData::MeshKernel*>(_dataObj);
        if (meshKernel == nullptr) {
            AppFrame::FITKMessageWarning(QStringLiteral("computeQualityRange: meshKernel为空"));
            return false;
        }

        Interface::FITKAbstractMesh* absMesh = meshKernel->getMesh();
        if (absMesh == nullptr) {
            AppFrame::FITKMessageWarning(QStringLiteral("computeQualityRange: absMesh为空"));
            return false;
        }

        Interface::FITKUnstructuredMesh* unstrMesh = nullptr;
        if (absMesh->getAbsModelType() == Interface::FITKModelEnum::AMTunstructuredMesh ||
            absMesh->getAbsModelType() == Interface::FITKModelEnum::AMTunstructuredMeshvtk) {
            unstrMesh = dynamic_cast<Interface::FITKUnstructuredMesh*>(absMesh);
        }

        if (unstrMesh == nullptr) {
            AppFrame::FITKMessageWarning(QStringLiteral("computeQualityRange: 仅支持非结构化网格"));
            return false;
        }

        const int eleCount = unstrMesh->getElementCount();
        if (eleCount == 0) {
            AppFrame::FITKMessageWarning(QStringLiteral("computeQualityRange: 网格单元数为0"));
            return false;
        }

        QList<Interface::FITKElemntQuality> qualities;
        qualities.reserve(eleCount);
        for (int i = 0; i < eleCount; ++i) {
            qualities.append(unstrMesh->checkElementQuality(i));
        }

        MeshQualityColorMapper mapper;
        mapper.setQualityMetric(qualityMetric);
        mapper.computeAutoRange(qualities);

        minValue = mapper.getMinValue();
        maxValue = mapper.getMaxValue();

        AppFrame::FITKMessageNormal(QString(QStringLiteral("computeQualityRange: 质量范围 %1 ~ %2")).arg(minValue).arg(maxValue));
        return true;
    }

    void GraphObjectMesh::applyQualityColoring(Interface::QualityMetric qualityMetric,
                                                Graph::ColorScheme colorScheme,
                                                double minVal,
                                                double maxVal,
                                                bool autoRange)
    {
        if (_meshData == nullptr) {
            AppFrame::FITKMessageWarning(QStringLiteral("质量着色失败: _meshData为空"));
            return;
        }
        
        ModelData::MeshKernel* meshKernel = dynamic_cast<ModelData::MeshKernel*>(_dataObj);
        if (meshKernel == nullptr) {
            AppFrame::FITKMessageWarning(QStringLiteral("质量着色失败: meshKernel为空"));
            return;
        }
        
        Interface::FITKAbstractMesh* absMesh = meshKernel->getMesh();
        if (absMesh == nullptr) {
            AppFrame::FITKMessageWarning(QStringLiteral("质量着色失败: absMesh为空"));
            return;
        }
        
        Interface::FITKUnstructuredMesh* unstrMesh = nullptr;
        if (absMesh->getAbsModelType() == Interface::FITKModelEnum::AMTunstructuredMesh ||
            absMesh->getAbsModelType() == Interface::FITKModelEnum::AMTunstructuredMeshvtk) {
            unstrMesh = dynamic_cast<Interface::FITKUnstructuredMesh*>(absMesh);
        }
        
        if (unstrMesh == nullptr) {
            AppFrame::FITKMessageWarning(QStringLiteral("质量着色仅支持非结构化网格"));
            return;
        }
        
        const int eleCount = unstrMesh->getElementCount();
        const int vtkCellCount = _meshData->GetNumberOfCells();
        
        AppFrame::FITKMessageNormal(QString(QStringLiteral("质量着色: 单元数=%1, VTK单元数=%2")).arg(eleCount).arg(vtkCellCount));
        
        if (eleCount == 0) {
            AppFrame::FITKMessageWarning(QStringLiteral("质量着色失败: 网格单元数为0"));
            return;
        }
        
        QList<Interface::FITKElemntQuality> qualities;
        qualities.reserve(eleCount);
        
        for (int i = 0; i < eleCount; ++i) {
            qualities.append(unstrMesh->checkElementQuality(i));
        }
        
        MeshQualityColorMapper mapper;
        mapper.setColorScheme(colorScheme);
        mapper.setQualityMetric(qualityMetric);
        mapper.setAutoRange(autoRange);
        
        if (autoRange) {
            double autoMin = 0.0;
            double autoMax = 0.0;
            if (computeQualityRange(qualityMetric, autoMin, autoMax)) {
                mapper.setRange(autoMin, autoMax);
            }
        } else {
            mapper.setRange(minVal, maxVal);
        }
        
        AppFrame::FITKMessageNormal(QString(QStringLiteral("质量范围: %1 ~ %2")).arg(mapper.getMinValue()).arg(mapper.getMaxValue()));
        
        vtkUnsignedCharArray* colorArray = vtkUnsignedCharArray::SafeDownCast(
            _meshData->GetCellData()->GetArray(GraphObjectCommons::_dataColorArrayName.toUtf8().constData()));
        
        if (colorArray == nullptr) {
            AppFrame::FITKMessageWarning(QStringLiteral("质量着色失败: 颜色数组为空"));
            return;
        }
        
        colorArray->SetNumberOfTuples(0);
        
        for (int i = 0; i < vtkCellCount; ++i) {
            int eleIndex = (i < eleCount) ? i : (eleCount - 1);
            QColor color = mapper.mapToColor(qualities[eleIndex]);
            colorArray->InsertNextTuple3(color.red(), color.green(), color.blue());
        }
        
        _meshData->Modified();
        
        if (_meshActor) {
            _meshActor->Modified();
        }
        
        AppFrame::FITKMessageNormal(QStringLiteral("质量着色完成"));
    }
    
    void GraphObjectMesh::clearQualityColoring()
    {
        updateModelColor();
    }
}