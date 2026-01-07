#include "GraphObjectGeoModel.h"
#include "GraphObjectCommons.h"

#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoCommand.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKGeoCommandProp.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKGeometryMeshEntity.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKGeometryMeshVS.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsVirtualTopoMapper.h"
#include "FITK_Interface/FITKVTKAlgorithm/FITKGraphActor.h"

#include <QSet>
#include <vtkPolyData.h>
#include <vtkUnstructuredGrid.h>
#include <vtkProperty.h>
#include <vtkUnsignedCharArray.h>
#include <vtkCellData.h>
#include <vtkMapper.h>
#include <vtkPolyDataNormals.h>

namespace Graph
    {
        namespace
        {
            // 判断拓扑是否存在禁用的指定类型祖先（通过虚拓扑映射查询）
            bool hasDisabledAncestor(Interface::FITKAbsVirtualTopo* topo, Interface::FITKGeoEnum::VTopoShapeType type)
        {
            if (!topo) return false;
            Interface::FITKVirtualTopoMapper mapper;
            mapper.mapTopo(topo, type, false);
            for (int i = 0; i < mapper.length(); ++i)
            {
                Interface::FITKAbsVirtualTopo* anc = mapper.virtualTopo(i);
                if (!anc) continue;
                if (!anc->isEnable())
                {
                    return true;
                }
            }
            return false;
        }

        inline bool hasDisabledSolidAncestor(Interface::FITKAbsVirtualTopo* topo)
        {
            return hasDisabledAncestor(topo, Interface::FITKGeoEnum::VTopoShapeType::VSSolid);
        }

        inline bool hasDisabledShellAncestor(Interface::FITKAbsVirtualTopo* topo)
        {
            return hasDisabledAncestor(topo, Interface::FITKGeoEnum::VTopoShapeType::VSShell);
        }

        // 是否存在启用的指定类型祖先
        bool hasEnabledAncestor(Interface::FITKAbsVirtualTopo* topo, Interface::FITKGeoEnum::VTopoShapeType type)
        {
            if (!topo) return false;
            Interface::FITKVirtualTopoMapper mapper;
            mapper.mapTopo(topo, type, false);
            for (int i = 0; i < mapper.length(); ++i)
            {
                Interface::FITKAbsVirtualTopo* anc = mapper.virtualTopo(i);
                if (anc && anc->isEnable())
                {
                    return true;
                }
            }
            return false;
        }
    }

    GraphObjectGeoModel::GraphObjectGeoModel(Interface::FITKAbsGeoCommand * geoCommand) :
        GraphObjectBase(geoCommand)
    {
        _baseColorCache = getGeoModelColor();
        if (!_baseColorCache.isValid()) {
            _baseColorCache = GraphObjectCommons::_geoFaceDefaultColor;
        }
        _vTopoColorOverrides.clear();
        init();

        if (updateGraphData() == false) {
            return;
        }

        createActor();
    }

    GraphObjectGeoModel::~GraphObjectGeoModel()
    {
        deleteVTKObj(_vertexGrid);
        deleteVTKObj(_curveGrid);
        deleteVTKObj(_faceGrid);
        deleteVTKObj(_solidGrid);
        deleteVTKObj(_vertexColorArray);
        deleteVTKObj(_curveColorArray);
        deleteVTKObj(_faceColorArray);
        deleteVTKObj(_solidColorArray);
        _vertexInfoHash.clear();
        _curveInfoHash.clear();
        _faceInfoHash.clear();
        _solidInfoHash.clear();
        _vertexVirTopIdVirIndex.clear();
        _curveVirTopIdVirIndex.clear();
        _faceVirTopIdVirIndex.clear();
        _solidVirTopIdVirIndex.clear();
    }

    void GraphObjectGeoModel::update(bool forceUpdate)
    {
        updateVisibility();

        if (forceUpdate == false) {
            return;
        }

        updateGraphData();
        // 强制重建后恢复已存在的自定义子拓扑颜色
        applyStoredTopoColors();
    }

    bool GraphObjectGeoModel::getVisibility()
    {
        //获取数据可见性
        bool isEnable = this->Superclass::getVisibility();

        //判断引用关系可见性
        bool beReferenced = false;
        Interface::FITKAbsGeoCommand* cmd = dynamic_cast<Interface::FITKAbsGeoCommand*>(_dataObj);
        if (cmd == nullptr) {
            return false;
        }
        //是否开启引用计数状态（为开启则是进入缓存区域）
        beReferenced = cmd->getEnableReferenceList();

        //判断是否是被删除获取被编辑状态
        beReferenced &= cmd->getCommandStatus() != Interface::FITKGeoEnum::FITKGeoStatus::FGSDeleted &&
            cmd->getCommandStatus() != Interface::FITKGeoEnum::FITKGeoStatus::FGSModified;

        //获取当前命令是被引用数量
        beReferenced &= (cmd->getReferencedCmdCount() == 0);

        return beReferenced &= isEnable;
    }

    void GraphObjectGeoModel::updateVisibility()
    {
        bool isVisibility = getVisibility();

        _vertexActor->SetVisibility(isVisibility);
        _curveActor->SetVisibility(isVisibility);
        _faceActor->SetVisibility(isVisibility);
        _solidActor->SetVisibility(isVisibility);
    }

    void GraphObjectGeoModel::updateSubColor(QColor color, QVariant otherPar)
    {
        if (!color.isValid()) {
            return;
        }
        //获取虚拟拓扑id
        int virTopoId = otherPar.toInt();
        if (virTopoId < 1) {
            return;
        }

        //获取当前虚拓扑类型
        ShapeAbsEnum curType = ShapeAbsEnum::None;
        if (_vertexVirTopIdVirIndex.keys().contains(virTopoId)) {
            curType = ShapeAbsEnum::STA_VERTEX;
        }
        else if (_curveVirTopIdVirIndex.keys().contains(virTopoId)) {
            curType = ShapeAbsEnum::STA_EDGE;
        }
        else if (_faceVirTopIdVirIndex.keys().contains(virTopoId)) {
            curType = ShapeAbsEnum::STA_FACE;
        }
        else if (_solidVirTopIdVirIndex.keys().contains(virTopoId)) {
            curType = ShapeAbsEnum::STA_SOLID;
        }
        else {
            // 可能是壳体：递归为壳体下的面/边着色
            Interface::FITKVirtualTopoManager* vtMgr = getVirTopoManager();
            if (vtMgr)
            {
                Interface::FITKAbsVirtualTopo* shellTopo = vtMgr->findTopo(virTopoId);
                if (shellTopo && shellTopo->getShapeType() == Interface::FITKGeoEnum::VTopoShapeType::VSShell)
                {
                    Interface::FITKVirtualTopoMapper mapper;
                    // 先处理面
                    mapper.mapTopo(shellTopo, Interface::FITKGeoEnum::VTopoShapeType::VSFace, false);
                    for (int i = 0; i < mapper.length(); ++i)
                    {
                        if (auto faceTopo = mapper.virtualTopo(i))
                        {
                            QVector<int> faceCells = getVTKCellIdsByShapeId(faceTopo->getDataObjectID(), Graph::ShapeAbsEnum::STA_FACE);
                            // 兼容索引标签作为shapeId的情况（避免映射不到导致不上色）
                            if (faceCells.isEmpty()) {
                                faceCells = getVTKCellIdsByShapeId(faceTopo->getIndexLabel(), Graph::ShapeAbsEnum::STA_FACE);
                            }
                            for (int cellId : faceCells) {
                                _faceColorArray->SetTuple3(cellId, color.red(), color.green(), color.blue());
                            }
                        }
                    }
                    // 再处理边（显示线框一致）
                    mapper.mapTopo(shellTopo, Interface::FITKGeoEnum::VTopoShapeType::VSEdge, false);
                    for (int i = 0; i < mapper.length(); ++i)
                    {
                        if (auto edgeTopo = mapper.virtualTopo(i))
                        {
                            QVector<int> edgeCells = getVTKCellIdsByShapeId(edgeTopo->getDataObjectID(), Graph::ShapeAbsEnum::STA_EDGE);
                            if (edgeCells.isEmpty()) {
                                edgeCells = getVTKCellIdsByShapeId(edgeTopo->getIndexLabel(), Graph::ShapeAbsEnum::STA_EDGE);
                            }
                            for (int cellId : edgeCells) {
                                _curveColorArray->SetTuple3(cellId, color.red(), color.green(), color.blue());
                            }
                        }
                    }
                    _faceColorArray->Modified();
                    _curveColorArray->Modified();
                    // 壳体颜色需要缓存，供重建后恢复
                    _vTopoColorOverrides.insert(virTopoId, color);
                }
            }
            return;
        }

        //获取当前颜色数组
        vtkUnsignedCharArray* curColorArray = nullptr;
        switch (curType) {
        case Graph::ShapeAbsEnum::STA_VERTEX:curColorArray = _vertexColorArray; break;
        case Graph::ShapeAbsEnum::STA_EDGE:curColorArray = _curveColorArray; break;
        case Graph::ShapeAbsEnum::STA_FACE:curColorArray = _faceColorArray; break;
        case Graph::ShapeAbsEnum::STA_SOLID:curColorArray = _solidColorArray; break;
        default:return;
        }
        if (curColorArray == nullptr) {
            return;
        }

        //获取当前虚拓扑id对应的Cell列表
        QVector<int> vtkCellIds = getVTKCellIdsByShapeId(virTopoId, curType);

        // 遍历所有相关单元并更新颜色
        for (int vtkCellId : vtkCellIds) {
            // 设置颜色值到颜色数组
            curColorArray->SetTuple3(vtkCellId, color.red(), color.green(), color.blue());
        }
        curColorArray->Modified();

        // 记录自定义颜色，便于重建后恢复
        _vTopoColorOverrides.insert(virTopoId, color);
    }

    void GraphObjectGeoModel::updateModelColor(QColor color)
    {
        // 新的基础色（根节点色），无效则沿用当前属性色
        QColor geoColor = getGeoModelColor();
        QColor targetBase = color.isValid() ? color : geoColor;

        // 旧的基础色，用于识别哪些单元目前仍在使用根色（而非子节点自定义色）
        QColor faceOldBase = _baseColorCache.isValid() ? _baseColorCache : GraphObjectCommons::_geoFaceDefaultColor;
        QColor solidOldBase = _baseColorCache.isValid() ? _baseColorCache : GraphObjectCommons::_geoSolidDefaultColor;

        QColor faceTarget = targetBase.isValid() ? targetBase : GraphObjectCommons::_geoFaceDefaultColor;
        QColor solidTarget = targetBase.isValid() ? targetBase : GraphObjectCommons::_geoSolidDefaultColor;

        auto updateArrayBaseColor = [](vtkUnsignedCharArray* array,
            const QHash<int, int>& infoHash,
            const QColor& oldBase,
            const QColor& newBase)
        {
            if (!array) return;
            double tuple[3]{ 0.0,0.0,0.0 };
            for (auto it = infoHash.constBegin(); it != infoHash.constEnd(); ++it)
            {
                int cellId = it.key();
                array->GetTuple(cellId, tuple);
                QColor current(static_cast<int>(tuple[0] + 0.5),
                    static_cast<int>(tuple[1] + 0.5),
                    static_cast<int>(tuple[2] + 0.5));
                // 仅更新仍使用基础色的单元，保留子节点自定义颜色
                if (current == oldBase)
                {
                    array->SetTuple3(cellId, newBase.red(), newBase.green(), newBase.blue());
                }
            }
            array->Modified();
        };

        updateArrayBaseColor(_faceColorArray, _faceInfoHash, faceOldBase, faceTarget);
        updateArrayBaseColor(_solidColorArray, _solidInfoHash, solidOldBase, solidTarget);

        // 缓存新的基础色，供后续覆盖判断
        _baseColorCache = targetBase.isValid() ? targetBase : faceTarget;

        // 基础色变更后，保持子拓扑自定义色：先保存的 overrides 已覆盖子节点，自定义色保持不变
        // 这里追加一次 applyStoredTopoColors 以确保刚才的基础色刷新后重新施加自定义覆盖色
        applyStoredTopoColors();
    }

    void GraphObjectGeoModel::applyStoredTopoColors()
    {
        if (_vTopoColorOverrides.isEmpty()) return;
        for (auto it = _vTopoColorOverrides.constBegin(); it != _vTopoColorOverrides.constEnd(); ++it)
        {
            int topoId = it.key();
            const QColor& color = it.value();
            if (!color.isValid()) continue;
            updateSubColor(color, topoId);
        }
    }

    vtkDataSet * GraphObjectGeoModel::getMesh(ShapeType type)
    {
        switch (type) {
        case Graph::ShapeType::ModelVertex:
            return _vertexGrid;
            break;
        case Graph::ShapeType::ModelEdge:
            return _curveGrid;
            break;
        case Graph::ShapeType::ModelFace:
            return _faceGrid;
            break;
        case Graph::ShapeType::ModelSolid:
            return _solidGrid;
            break;
        }
        return nullptr;
    }

    int GraphObjectGeoModel::getShapeIdByVTKCellId(int vtkCellId, ShapeAbsEnum topAbsShapeType)
    {
        int shapeId = -1;
        switch (topAbsShapeType) {
        case Graph::ShapeAbsEnum::STA_VERTEX:
            shapeId = _vertexInfoHash.value(vtkCellId);
            break;
        case Graph::ShapeAbsEnum::STA_EDGE:
            shapeId = _curveInfoHash.value(vtkCellId);
            break;
        case Graph::ShapeAbsEnum::STA_FACE:
            shapeId = _faceInfoHash.value(vtkCellId);
            break;
        case Graph::ShapeAbsEnum::STA_SOLID:
            shapeId = _solidInfoHash.value(vtkCellId);
            break;
        }
        return shapeId;
    }

    int GraphObjectGeoModel::getVirTopoIndexByShapeId(int vTopoId, ShapeAbsEnum topAbsShapeType)
    {
        int index = -1;
        switch (topAbsShapeType){
        case Graph::ShapeAbsEnum::STA_VERTEX:
            index = _vertexVirTopIdVirIndex.value(vTopoId);
            break;
        case Graph::ShapeAbsEnum::STA_EDGE:
            index = _curveVirTopIdVirIndex.value(vTopoId);
            break;
        case Graph::ShapeAbsEnum::STA_FACE:
            index = _faceVirTopIdVirIndex.value(vTopoId);
            break;
        case Graph::ShapeAbsEnum::STA_SOLID:
            index = _solidVirTopIdVirIndex.value(vTopoId);
            break;
        }
        return index;
    }

    const QVector<int> GraphObjectGeoModel::getVTKCellIdsByShapeId(int shapeId, ShapeAbsEnum topAbsShapeType)
    {
        QVector<int> vtkCellIds = {};
        auto getKeys = [](QHash<int, int>& hash, QVector<int>& result, int value) {
            QHash<int, int>::const_iterator i = hash.constBegin();
            while (i != hash.constEnd()) {
                // 如果当前键值对的值等于指定的值
                if (i.value() == value) {
                    // 将该键添加到结果列表中
                    result.append(i.key());
                }
                ++i;
            }
        };

        switch (topAbsShapeType) {
        case Graph::ShapeAbsEnum::STA_VERTEX:
            getKeys(_vertexInfoHash, vtkCellIds, shapeId);
            break;
        case Graph::ShapeAbsEnum::STA_EDGE:
            getKeys(_curveInfoHash, vtkCellIds, shapeId);
            break;
        case Graph::ShapeAbsEnum::STA_FACE:
            getKeys(_faceInfoHash, vtkCellIds, shapeId);
            break;
        case Graph::ShapeAbsEnum::STA_SOLID:
            getKeys(_solidInfoHash, vtkCellIds, shapeId);
            break;
        }
        return vtkCellIds;
    }

    void GraphObjectGeoModel::setPickModel(bool isPick, ShapeType type)
    {
        switch (type) {
        case Graph::ShapeType::ModelVertex:
            _vertexActor->SetPickable(isPick);
            break;
        case Graph::ShapeType::ModelEdge:
            _curveActor->SetPickable(isPick);
            break;
        case Graph::ShapeType::ModelFace:
            _faceActor->SetPickable(isPick);
            break;
        case Graph::ShapeType::ModelSolid:
            _solidActor->SetPickable(isPick);
            break;
        }
    }

    void GraphObjectGeoModel::init()
    {
        //点网格创建
        _vertexGrid = vtkUnstructuredGrid::New();
        _vertexGrid->SetPoints(vtkSmartPointer<vtkPoints>::New());

        //线网格创建
        _curveGrid = vtkUnstructuredGrid::New();
        _curveGrid->SetPoints(vtkSmartPointer<vtkPoints>::New());

        //面网格创建
        _faceGrid = vtkPolyData::New();
        _faceGrid->SetPoints(vtkSmartPointer<vtkPoints>::New());
        _faceGrid->SetPolys(vtkSmartPointer<vtkCellArray>::New());

        //体网格创建
        _solidGrid = vtkPolyData::New();
        _solidGrid->SetPoints(vtkSmartPointer<vtkPoints>::New());
        _solidGrid->SetPolys(vtkSmartPointer<vtkCellArray>::New());

        //点网格颜色数值指定
        _vertexColorArray = vtkUnsignedCharArray::New();
        _vertexColorArray->SetName(GraphObjectCommons::_dataColorArrayName.toUtf8().constData());
        _vertexColorArray->SetNumberOfComponents(3);
        _vertexGrid->GetCellData()->AddArray(_vertexColorArray);

        //线网格颜色数值指定
        _curveColorArray = vtkUnsignedCharArray::New();
        _curveColorArray->SetName(GraphObjectCommons::_dataColorArrayName.toUtf8().constData());
        _curveColorArray->SetNumberOfComponents(3);
        _curveGrid->GetCellData()->AddArray(_curveColorArray);

        //面网格颜色数值指定
        _faceColorArray = vtkUnsignedCharArray::New();
        _faceColorArray->SetName(GraphObjectCommons::_dataColorArrayName.toUtf8().constData());
        _faceColorArray->SetNumberOfComponents(3);
        _faceGrid->GetCellData()->AddArray(_faceColorArray);

        //体网格颜色数值指定
        _solidColorArray = vtkUnsignedCharArray::New();
        _solidColorArray->SetName(GraphObjectCommons::_dataColorArrayName.toUtf8().constData());
        _solidColorArray->SetNumberOfComponents(3);
        _solidGrid->GetCellData()->AddArray(_solidColorArray);
    }

    bool GraphObjectGeoModel::updateGraphData()
    {
        updateVertexGrid();
        updateCurveGrid();
        updateFaceGrid();
        updateSolidGrid();
        return true;
    }

    void GraphObjectGeoModel::createActor()
    {
        _vertexActor = FITKGraphActor::New();
        _vertexActor->SetPickable(false);
        _vertexActor->setGraphObject(this);
        _vertexActor->GetProperty()->SetRepresentation(VTK_POINTS);
        _vertexActor->setInputDataObject(_vertexGrid);
        _vertexActor->GetMapper()->SetScalarModeToUseCellFieldData();
        _vertexActor->GetMapper()->SelectColorArray(GraphObjectCommons::_dataColorArrayName.toUtf8().constData());
        _vertexActor->GetProperty()->SetPointSize(GraphObjectCommons::_geoPointDefaultSize);
        _vertexActor->setRelativeCoincidentTopologyPolygonOffsetParameters(GraphObjectCommons::_geoGraphOffset);
        addActor(_vertexActor);

        _curveActor = FITKGraphActor::New();
        _curveActor->SetPickable(false);
        _curveActor->setGraphObject(this);
        _curveActor->GetProperty()->SetRepresentation(VTK_WIREFRAME);
        _curveActor->setInputDataObject(_curveGrid);
        _curveActor->GetMapper()->SetScalarModeToUseCellFieldData();
        _curveActor->GetMapper()->SelectColorArray(GraphObjectCommons::_dataColorArrayName.toUtf8().constData());
        _curveActor->GetProperty()->SetLineWidth(GraphObjectCommons::_geoLineDefaultSize);
        _curveActor->setRelativeCoincidentTopologyPolygonOffsetParameters(GraphObjectCommons::_geoGraphOffset);
        addActor(_curveActor);

        _faceActor = FITKGraphActor::New();
        _faceActor->SetPickable(false);
        _faceActor->setGraphObject(this);
        _faceActor->GetProperty()->SetRepresentation(VTK_SURFACE);
        _faceActor->setInputDataObject(_faceGrid);
        _faceActor->GetMapper()->SetScalarModeToUseCellFieldData();
        _faceActor->GetMapper()->SelectColorArray(GraphObjectCommons::_dataColorArrayName.toUtf8().constData());
        _faceActor->setRelativeCoincidentTopologyPolygonOffsetParameters(GraphObjectCommons::_geoGraphOffset);
        addActor(_faceActor);

        _solidActor = FITKGraphActor::New();
        _solidActor->SetPickable(false);
        _solidActor->setGraphObject(this);
        _solidActor->GetProperty()->SetRepresentation(VTK_SURFACE);
        _solidActor->setInputDataObject(_solidGrid);
        _solidActor->GetMapper()->SetScalarModeToUseCellFieldData();
        _solidActor->GetMapper()->SelectColorArray(GraphObjectCommons::_dataColorArrayName.toUtf8().constData());
        _solidActor->setRelativeCoincidentTopologyPolygonOffsetParameters(GraphObjectCommons::_geoGraphOffset);
        addActor(_solidActor);
    }

    void GraphObjectGeoModel::updateVertexGrid()
    {
        if (_vertexGrid == nullptr || _vertexGrid->GetPoints() == nullptr) {
            return;
        }

        //数据清空
        _vertexGrid->Reset();
        _vertexInfoHash.clear();
        _vertexVirTopIdVirIndex.clear();
        _vertexColorArray->SetNumberOfTuples(0);
        // 彻底清空坐标，避免残留点位
        if (_vertexGrid->GetPoints()) {
            _vertexGrid->GetPoints()->Reset();
        }
        if (_vertexActor) {
            _vertexActor->SetVisibility(false);
        }

        vtkPoints* points = _vertexGrid->GetPoints();

        Interface::FITKAbsGeoCommand* modelData = dynamic_cast<Interface::FITKAbsGeoCommand*>(_dataObj);
        if (modelData == nullptr || modelData->getShapeAgent() == nullptr) {
            return;
        }

        //获取几何模型渲染网格对象与虚拓扑管理器
        Interface::FITKGeometryMeshVS* triData = modelData->getShapeAgent()->getMeshVS();
        Interface::FITKVirtualTopoManager* virTopoManager = modelData->getShapeAgent()->getVirtualTopoManager();
        if (triData == nullptr || virTopoManager == nullptr) {
            return;
        }

        // 收集启用的 Solid/Shell 对应的面，再由面收集点，保证只渲染可见部分
        QSet<int> allowedFaces;
        QSet<int> allowedPoints;
        Interface::FITKVirtualTopoMapper mapper;
        auto collectFaces = [&](Interface::FITKAbsVirtualTopo* topo) {
            if (!topo) return;
            mapper.mapTopo(topo, Interface::FITKGeoEnum::VTopoShapeType::VSFace, false);
            for (int i = 0; i < mapper.length(); ++i) {
                if (auto f = mapper.virtualTopo(i)) {
                    allowedFaces.insert(f->getDataObjectID());
                }
            }
        };
        if (auto solidMgr = virTopoManager->getShapeVirtualTopoManager(Interface::FITKGeoEnum::VTopoShapeType::VSSolid)) {
            for (int i = 0; i < solidMgr->getDataCount(); ++i) {
                Interface::FITKAbsVirtualTopo* s = solidMgr->getDataByIndex(i);
                if (s && s->isEnable()) collectFaces(s);
            }
        }
        if (auto shellMgr = virTopoManager->getShapeVirtualTopoManager(Interface::FITKGeoEnum::VTopoShapeType::VSShell)) {
            for (int i = 0; i < shellMgr->getDataCount(); ++i) {
                Interface::FITKAbsVirtualTopo* sh = shellMgr->getDataByIndex(i);
                if (sh && sh->isEnable()) collectFaces(sh);
            }
        }
        // 由面收集点（只要面启用才收集）
        for (int faceId : allowedFaces) {
            Interface::FITKAbsVirtualTopo* faceTopo = virTopoManager->findTopo(faceId);
            if (!faceTopo || !faceTopo->isEnable()) continue;
            mapper.mapTopo(faceTopo, Interface::FITKGeoEnum::VTopoShapeType::VSPoint, false);
            for (int i = 0; i < mapper.length(); ++i) {
                Interface::FITKAbsVirtualTopo* p = mapper.virtualTopo(i);
                if (p && p->isEnable()) {
                    allowedPoints.insert(p->getDataObjectID());
                }
            }
        }

        //获取点拓扑管理器
        Interface::FITKShapeVirtualTopoManager* vsMgrPoint = virTopoManager->getShapeVirtualTopoManager(Interface::FITKGeoEnum::VTopoShapeType::VSPoint);
        if (vsMgrPoint == nullptr) {
            return;
        }

        const QHash<int, Interface::FITKGeoMeshVSPt*> & pointsHash = triData->getPointItems();
        QHashIterator<int, Interface::FITKGeoMeshVSPt*> iter(pointsHash);

        int ptIndex = -1;
        int nVerts = vsMgrPoint->getDataCount();
        for (int i = 0; i < nVerts; i++) {
            Interface::FITKAbsVirtualTopo* vTopoV = vsMgrPoint->getDataByIndex(i);
            if (vTopoV == nullptr) {
                continue;
            }

            //判断数据的可见性
            if (vTopoV->isEnable() == false || hasDisabledSolidAncestor(vTopoV) || hasDisabledShellAncestor(vTopoV)) {
                continue;
            }

            // 必须属于启用的 Solid 或 Shell，否则跳过
            if (!allowedPoints.contains(vTopoV->getDataObjectID())) {
                continue;
            }

            int pointId = vTopoV->getDataObjectID();

            Interface::FITKGeoMeshVSPt* pt = pointsHash[pointId];
            if (pt == nullptr) {
                continue;
            }
            ptIndex = points->InsertNextPoint(pt->x(), pt->y(), pt->z());

            vtkSmartPointer<vtkIdList> cell = vtkSmartPointer<vtkIdList>::New();
            cell->InsertNextId(ptIndex);
            //添加单元
            _vertexGrid->InsertNextCell(VTKCellType::VTK_VERTEX, cell);
            //保存VTK单元与虚拓扑映射
            _vertexInfoHash.insert(ptIndex, pointId);
            //保存虚拓扑id与虚拓扑索引映射
            _vertexVirTopIdVirIndex.insert(pointId, i);
            // 设置默认颜色
            QColor defQColor = GraphObjectCommons::_geoPointDefaultColor;
            double defColor[3] = { defQColor.red(), defQColor.green(), defQColor.blue() };
            _vertexColorArray->InsertNextTuple(defColor);
        }

        if (_vertexGrid->GetNumberOfCells() > 0)
        {
            _vertexGrid->GetCellData()->SetScalars(_vertexColorArray);
            _vertexGrid->Modified();
            _vertexColorArray->Modified();
            if (_vertexActor) {
                _vertexActor->SetVisibility(true);
            }
        }
        else
        {
            if (_vertexActor) {
                _vertexActor->SetVisibility(false);
            }
        }
    }

    void GraphObjectGeoModel::updateCurveGrid()
    {
        if (_curveGrid == nullptr || _curveGrid->GetPoints() == nullptr) {
            return;
        }

        //数据清空
        _curveGrid->Reset();
        _curveInfoHash.clear();
        _curveVirTopIdVirIndex.clear();
        _curveColorArray->SetNumberOfTuples(0);
        if (_curveGrid->GetPoints()) {
            _curveGrid->GetPoints()->Reset();
        }
        vtkPoints* points = _curveGrid->GetPoints();
        if (_curveActor) {
            _curveActor->SetVisibility(false);
        }

        Interface::FITKAbsGeoCommand* modelData = dynamic_cast<Interface::FITKAbsGeoCommand*>(_dataObj);
        if (modelData == nullptr || modelData->getShapeAgent() == nullptr) {
            return;
        }

        //获取几何模型渲染网格对象与虚拓扑管理器
        Interface::FITKGeometryMeshVS* triData = modelData->getShapeAgent()->getMeshVS();
        Interface::FITKVirtualTopoManager* virTopoManager = modelData->getShapeAgent()->getVirtualTopoManager();
        if (triData == nullptr || virTopoManager == nullptr) {
            return;
        }

        // 允许的边/面集合（由启用的 Solid 或 Shell 收集）
        QSet<int> allowedFaces;
        QSet<int> allowedEdges;
        Interface::FITKVirtualTopoMapper mapper;
        auto collectFaces = [&](Interface::FITKAbsVirtualTopo* topo) {
            if (!topo) return;
            mapper.mapTopo(topo, Interface::FITKGeoEnum::VTopoShapeType::VSFace, false);
            for (int i = 0; i < mapper.length(); ++i) {
                if (auto f = mapper.virtualTopo(i)) {
                    allowedFaces.insert(f->getDataObjectID());
                }
            }
        };
        auto collectEdgesFromFaces = [&]() {
            for (int faceId : allowedFaces) {
                Interface::FITKAbsVirtualTopo* faceTopo = virTopoManager->findTopo(faceId);
                if (!faceTopo) continue;
                mapper.mapTopo(faceTopo, Interface::FITKGeoEnum::VTopoShapeType::VSEdge, false);
                for (int i = 0; i < mapper.length(); ++i) {
                    if (auto e = mapper.virtualTopo(i)) {
                        allowedEdges.insert(e->getDataObjectID());
                    }
                }
            }
        };

        // 收集启用的 Solid
        if (auto solidMgr = virTopoManager->getShapeVirtualTopoManager(Interface::FITKGeoEnum::VTopoShapeType::VSSolid)) {
            for (int i = 0; i < solidMgr->getDataCount(); ++i) {
                Interface::FITKAbsVirtualTopo* s = solidMgr->getDataByIndex(i);
                if (s && s->isEnable()) {
                    collectFaces(s);
                }
            }
        }
        // 收集启用的 Shell
        if (auto shellMgr = virTopoManager->getShapeVirtualTopoManager(Interface::FITKGeoEnum::VTopoShapeType::VSShell)) {
            for (int i = 0; i < shellMgr->getDataCount(); ++i) {
                Interface::FITKAbsVirtualTopo* sh = shellMgr->getDataByIndex(i);
                if (sh && sh->isEnable()) {
                    collectFaces(sh);
                }
            }
        }
        collectEdgesFromFaces();

        // Get the edge and wire shape topo manager.
        Interface::FITKShapeVirtualTopoManager* vsMgrEdge = virTopoManager->getShapeVirtualTopoManager(Interface::FITKGeoEnum::VTopoShapeType::VSEdge);
        Interface::FITKShapeVirtualTopoManager* vsMgrWire = virTopoManager->getShapeVirtualTopoManager(Interface::FITKGeoEnum::VTopoShapeType::VSWire);
        if (!vsMgrEdge || !vsMgrWire) {
            return;
        }

        // Get all edges.
        const QHash<int, Interface::FITKGeoMeshVSEdgeEntity*> & edgesHash = triData->getEdgeItems();
        QHashIterator<int, Interface::FITKGeoMeshVSEdgeEntity*> iter(edgesHash);

        int ptIndex = -1, cellIndex = -1;

        int nEdges = vsMgrEdge->getDataCount();
        for (int i = 0; i < nEdges; i++) {
            Interface::FITKAbsVirtualTopo* vTopoE = vsMgrEdge->getDataByIndex(i);
            if (!vTopoE) {
                continue;
            }

            //判断数据的可见性
            if (vTopoE->isEnable() == false || hasDisabledSolidAncestor(vTopoE) || hasDisabledShellAncestor(vTopoE)) {
                continue;
            }

            int edgeId = vTopoE->getDataObjectID();
            // 必须属于启用的 Solid 或启用的 Shell，否则跳过
            if (!allowedEdges.contains(edgeId)) {
                continue;
            }

            // Get the edge mesh.
            Interface::FITKGeoMeshVSEdgeEntity* edge = edgesHash[edgeId];
            if (!edge) {
                continue;
            }

            // Get edge points.
            const QList<Interface::FITKGeoMeshVSPt*> & pts = edge->getPoints();

            // Get the node data and create poly line.
            vtkSmartPointer<vtkIdList> polyLineCell = vtkSmartPointer<vtkIdList>::New();

            for (Interface::FITKGeoMeshVSPt* pt : pts) {
                ptIndex = points->InsertNextPoint(pt->x(), pt->y(), pt->z());
                polyLineCell->InsertNextId(ptIndex);
            }

            //添加单元
            cellIndex = _curveGrid->InsertNextCell(VTKCellType::VTK_POLY_LINE, polyLineCell);
            //保存VTK单元与虚拓扑映射
            _curveInfoHash.insert(cellIndex, edgeId);
            //保存虚拓扑id与虚拓扑索引映射
            _curveVirTopIdVirIndex.insert(edgeId, i);
            // 设置默认颜色
            QColor defQColor = GraphObjectCommons::_geoCurveDefaultColor;
            double defColor[3] = { defQColor.red(), defQColor.green(), defQColor.blue() };
            _curveColorArray->InsertNextTuple(defColor);
        }

        // 刷新标记并控制可见性
        if (_curveGrid->GetNumberOfCells() > 0)
        {
            _curveGrid->GetCellData()->SetScalars(_curveColorArray);
            _curveGrid->Modified();
            _curveColorArray->Modified();
            if (_curveActor) {
                _curveActor->SetVisibility(true);
            }
        }
        else
        {
            if (_curveActor) {
                _curveActor->SetVisibility(false);
            }
        }
    }

    void GraphObjectGeoModel::updateFaceGrid()
    {
        if (_faceGrid == nullptr || _faceGrid->GetPoints() == nullptr) {
            return;
        }

        //数据清空
        _faceGrid->Reset();
        _faceInfoHash.clear();
        _faceVirTopIdVirIndex.clear();
        _faceColorArray->SetNumberOfTuples(0);
        if (_faceGrid->GetPoints()) {
            _faceGrid->GetPoints()->Reset();
        }
        if (_faceGrid->GetPolys()) {
            _faceGrid->GetPolys()->Reset();
        }
        if (_faceActor) {
            _faceActor->SetVisibility(false);
        }

        vtkPoints* points = _faceGrid->GetPoints();

        Interface::FITKAbsGeoCommand* modelData = dynamic_cast<Interface::FITKAbsGeoCommand*>(_dataObj);
        if (modelData == nullptr || modelData->getShapeAgent() == nullptr) {
            return;
        }

        //获取几何模型渲染网格对象与虚拓扑管理器
        Interface::FITKGeometryMeshVS* triData = modelData->getShapeAgent()->getMeshVS();
        Interface::FITKVirtualTopoManager* virTopoManager = modelData->getShapeAgent()->getVirtualTopoManager();
        if (triData == nullptr || virTopoManager == nullptr) {
            return;
        }

        const QHash<int, Interface::FITKGeoMeshVSFaceEntity*>& facesHash = triData->getFaceItems();

        // 允许的面集合（来自启用的 Solid/Shell）
        QSet<int> allowedFaces;
        Interface::FITKVirtualTopoMapper mapper;
        auto collectFaces = [&](Interface::FITKAbsVirtualTopo* topo) {
            if (!topo) return;
            mapper.mapTopo(topo, Interface::FITKGeoEnum::VTopoShapeType::VSFace, false);
            for (int i = 0; i < mapper.length(); ++i) {
                if (auto f = mapper.virtualTopo(i)) {
                    allowedFaces.insert(f->getDataObjectID());
                }
            }
        };
        if (auto solidMgr = virTopoManager->getShapeVirtualTopoManager(Interface::FITKGeoEnum::VTopoShapeType::VSSolid)) {
            for (int i = 0; i < solidMgr->getDataCount(); ++i) {
                Interface::FITKAbsVirtualTopo* s = solidMgr->getDataByIndex(i);
                if (s && s->isEnable()) {
                    collectFaces(s);
                }
            }
        }
        if (auto shellMgr = virTopoManager->getShapeVirtualTopoManager(Interface::FITKGeoEnum::VTopoShapeType::VSShell)) {
            for (int i = 0; i < shellMgr->getDataCount(); ++i) {
                Interface::FITKAbsVirtualTopo* sh = shellMgr->getDataByIndex(i);
                if (sh && sh->isEnable()) {
                    collectFaces(sh);
                }
            }
        }

        // Get the face and shell shape topo manager.
        Interface::FITKShapeVirtualTopoManager* vsMgrFace = virTopoManager->getShapeVirtualTopoManager(Interface::FITKGeoEnum::VTopoShapeType::VSFace);
        Interface::FITKShapeVirtualTopoManager* vsMgrShell = virTopoManager->getShapeVirtualTopoManager(Interface::FITKGeoEnum::VTopoShapeType::VSShell);
        if (!vsMgrFace || !vsMgrShell) {
            return;
        }

        int ptIndexStart = 0;
        int cellIndex = -1;

        int nFaces = vsMgrFace->getDataCount();
        for (int i = 0; i < nFaces; i++)
        {
            Interface::FITKAbsVirtualTopo* vTopoF = vsMgrFace->getDataByIndex(i);
            if (!vTopoF) {
                continue;
            }
        //判断数据的可见性
        if (vTopoF->isEnable() == false || hasDisabledSolidAncestor(vTopoF) || hasDisabledShellAncestor(vTopoF)) {
            continue;
        }

        // 必须属于启用的 Solid 或启用的 Shell，否则跳过
        if (!allowedFaces.contains(vTopoF->getDataObjectID())) {
            continue;
        }

            int faceId = vTopoF->getDataObjectID();
            //添加面虚拓扑id与虚拓扑索引映射
            _faceVirTopIdVirIndex.insert(faceId, i);

            // Get the face mesh.
            Interface::FITKGeoMeshVSFaceEntity* face = facesHash[faceId];
            if (!face)
            {
                continue;
            }

            // Get all points of the face mesh.
            const QList<Interface::FITKGeoMeshVSPt*> & facePoints = face->getPoints();
            for (Interface::FITKGeoMeshVSPt* facePoint : facePoints)
            {
                points->InsertNextPoint(facePoint->x(), facePoint->y(), facePoint->z());
            }

            // Get face mesh triangles.
            const QList<Interface::FITKGeoMeshVSTri*> & tris = face->getTriangles();
            for (Interface::FITKGeoMeshVSTri* tri : tris)
            {
                // Get points indice pf the triangle.
                const int* nodes = tri->getNode();

                // Get points index in VTK grid of vertice
                int ptIndex1 = ptIndexStart + nodes[0];
                int ptIndex2 = ptIndexStart + nodes[1];
                int ptIndex3 = ptIndexStart + nodes[2];

                // Add cell.
                vtkSmartPointer<vtkIdList> triCell = vtkSmartPointer<vtkIdList>::New();
                triCell->InsertNextId(ptIndex1);
                triCell->InsertNextId(ptIndex2);
                triCell->InsertNextId(ptIndex3);
                cellIndex = _faceGrid->InsertNextCell(VTKCellType::VTK_TRIANGLE, triCell);
                QColor defQColor = getGeoModelColor();
                if (!defQColor.isValid()) {
                    defQColor = GraphObjectCommons::_geoFaceDefaultColor;
                }
                double defColor[3] = { defQColor.red(),defQColor.green(),defQColor.blue() };
                _faceColorArray->InsertNextTuple(defColor);

                _faceInfoHash.insert(cellIndex, faceId);
            }

            ptIndexStart += facePoints.count();
        }

        //曲面平滑处理
        vtkSmartPointer<vtkPolyDataNormals> normals = vtkSmartPointer<vtkPolyDataNormals>::New();
        normals->SetInputDataObject(_faceGrid);
        normals->SetComputePointNormals(true);
        normals->Update();
        _faceGrid->DeepCopy(normals->GetOutput());
        _faceGrid->GetCellData()->SetScalars(_faceColorArray);
        _faceGrid->Modified();
        _faceColorArray->Modified();
        if (_faceGrid->GetNumberOfCells() > 0 && _faceActor) {
            _faceActor->SetVisibility(true);
        } else {
            if (_faceActor) {
                _faceActor->SetVisibility(false);
            }
        }
    }

    void GraphObjectGeoModel::updateSolidGrid()
    {
        if (_solidGrid == nullptr || _solidGrid->GetPoints() == nullptr) {
            return;
        }

        //数据清空
        _solidGrid->Reset();
        _solidInfoHash.clear();
        _solidVirTopIdVirIndex.clear();
        if (_solidActor != nullptr) {
            _solidActor->SetVisibility(false);
        }
        if (_solidGrid->GetPoints()) {
            _solidGrid->GetPoints()->Reset();
        }
        if (_solidGrid->GetPolys()) {
            _solidGrid->GetPolys()->Reset();
        }

        vtkPoints* points = _solidGrid->GetPoints();

        Interface::FITKAbsGeoCommand* modelData = dynamic_cast<Interface::FITKAbsGeoCommand*>(_dataObj);
        if (modelData == nullptr || modelData->getShapeAgent() == nullptr) {
            return;
        }

        //获取几何模型渲染网格对象与虚拓扑管理器
        Interface::FITKGeometryMeshVS* triData = modelData->getShapeAgent()->getMeshVS();
        Interface::FITKVirtualTopoManager* virTopoManager = modelData->getShapeAgent()->getVirtualTopoManager();
        if (triData == nullptr || virTopoManager == nullptr) {
            return;
        }

        const QHash<int, Interface::FITKGeoMeshVSFaceEntity*> & facesHash = triData->getFaceItems();

        Interface::FITKShapeVirtualTopoManager* vsMgrSolid = virTopoManager->getShapeVirtualTopoManager(Interface::FITKGeoEnum::VTopoShapeType::VSSolid);
        if (vsMgrSolid == nullptr) {
            return;
        }

        int ptIndexStart = 0;
        int cellIndex = -1;

        // Get the (root) solid topo.
        int nSolids = vsMgrSolid->getDataCount();
        for (int i = 0; i < nSolids; i++)
        {
            Interface::FITKAbsVirtualTopo* vTopoS = vsMgrSolid->getDataByIndex(i);
            if (!vTopoS)
            {
                continue;
            }
            //判断数据的可见性
            if (vTopoS->isEnable() == false)
            {
                continue;
            }
            int solidId = vTopoS->getDataObjectID();
            //添加面虚拓扑id与虚拓扑索引映射
            _solidVirTopIdVirIndex.insert(solidId, i);

            Interface::FITKVirtualTopoMapper vMapper;
            vMapper.mapTopo(vTopoS, Interface::FITKGeoEnum::VTopoShapeType::VSFace);

            // Get all faces.
            int nFaces = vMapper.length();
            for (int j = 0; j < nFaces; j++)
            {
                Interface::FITKAbsVirtualTopo* vTopoF = vMapper.virtualTopo(j);
                if (!vTopoF)
                {
                    continue;
                }

                //判断数据的可见性
                if (vTopoF->isEnable() == false || hasDisabledSolidAncestor(vTopoF) || hasDisabledShellAncestor(vTopoF))
                {
                    continue;
                }

                // 必须属于启用的 Solid 或启用的 Shell，否则跳过
                bool enableBySolid = hasEnabledAncestor(vTopoF, Interface::FITKGeoEnum::VTopoShapeType::VSSolid);
                bool enableByShell = hasEnabledAncestor(vTopoF, Interface::FITKGeoEnum::VTopoShapeType::VSShell);
                if (!enableBySolid && !enableByShell) {
                    continue;
                }
                int faceId = vTopoF->getDataObjectID();

                // Get the face mesh.
                Interface::FITKGeoMeshVSFaceEntity* face = facesHash[faceId];
                if (!face)
                {
                    continue;
                }

                // Get all points of the face mesh.
                const QList<Interface::FITKGeoMeshVSPt*> & facePoints = face->getPoints();
                for (Interface::FITKGeoMeshVSPt* facePoint : facePoints)
                {
                    points->InsertNextPoint(facePoint->x(), facePoint->y(), facePoint->z());
                }

                // Get face mesh triangles.
                const QList<Interface::FITKGeoMeshVSTri*> & tris = face->getTriangles();

                for (Interface::FITKGeoMeshVSTri* tri : tris)
                {
                    // Get points indice pf the triangle.
                    const int* nodes = tri->getNode();

                    // Get points index in VTK grid of vertice
                    int ptIndex1 = ptIndexStart + nodes[0];
                    int ptIndex2 = ptIndexStart + nodes[1];
                    int ptIndex3 = ptIndexStart + nodes[2];

                    // Add cell.
                    vtkSmartPointer<vtkIdList> triCell = vtkSmartPointer<vtkIdList>::New();
                    triCell->InsertNextId(ptIndex1);
                    triCell->InsertNextId(ptIndex2);
                    triCell->InsertNextId(ptIndex3);
                    cellIndex = _solidGrid->InsertNextCell(VTKCellType::VTK_TRIANGLE, triCell);
                    QColor defQColor = getGeoModelColor();
                    if (!defQColor.isValid()) {
                        defQColor = GraphObjectCommons::_geoSolidDefaultColor;
                    }
                    double defColor[3] = { defQColor.red(), defQColor.green(), defQColor.blue() };
                    _solidColorArray->InsertNextTuple(defColor);
                    _solidInfoHash.insert(cellIndex, solidId);
                }

                ptIndexStart += facePoints.count();
            }
        }

        //曲面平滑处理
        vtkSmartPointer<vtkPolyDataNormals> normals = vtkSmartPointer<vtkPolyDataNormals>::New();
        if (_solidGrid->GetNumberOfCells() == 0)
        {
            _solidColorArray->SetNumberOfTuples(0);
            _solidGrid->Modified();
            _solidColorArray->Modified();
            if (_solidActor != nullptr) {
                _solidActor->SetVisibility(false);
            }
            return;
        }

        normals->SetInputDataObject(_solidGrid);
        normals->SetComputePointNormals(true);
        normals->Update();
        _solidGrid->DeepCopy(normals->GetOutput());
        _solidGrid->GetCellData()->SetScalars(_solidColorArray);
        _solidGrid->Modified();
        _solidColorArray->Modified();
        if (_solidActor != nullptr) {
            _solidActor->SetVisibility(true);
        }
    }

    Interface::FITKVirtualTopoManager* GraphObjectGeoModel::getVirTopoManager()
    {
        Interface::FITKAbsGeoCommand* modelData = dynamic_cast<Interface::FITKAbsGeoCommand*>(_dataObj);
        if (modelData == nullptr) {
            return nullptr;
        }

        Interface::FITKAbsGeoShapeAgent* shapeAgent = modelData->getShapeAgent();
        if (shapeAgent == nullptr) {
            return nullptr;
        }

        Interface::FITKGeometryMeshVS* triData = shapeAgent->getMeshVS();
        if (triData == nullptr) {
            return nullptr;
        }
        return shapeAgent->getVirtualTopoManager();
    }

    QColor GraphObjectGeoModel::getGeoModelColor()
    {
        QColor color;
        Interface::FITKAbsGeoCommand* geoCom = dynamic_cast<Interface::FITKAbsGeoCommand*>(_dataObj);
        if (geoCom == nullptr) {
            return color;
        }

        Interface::FITKGeoCommandProp* geoComProp = geoCom->getCommandProp();
        if (geoComProp == nullptr) {
            return color;
        }

        color = geoComProp->getColor();

        return color;
    }
}
