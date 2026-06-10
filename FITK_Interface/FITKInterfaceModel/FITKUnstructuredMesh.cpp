#include "FITKUnstructuredMesh.h"
#include "FITKAbstractElement.h"
#include "FITKModelEnum.h"
#include "FITKUnstructuredMeshTopo.h"
#include "FITKGeoToMeshMapper.h"

#include "FITK_Kernel/FITKCore/FITKVec3DAlg.h"

namespace Interface
{
    FITKUnstructuredMesh::FITKUnstructuredMesh()
    {
        _geoToMeshMapper = new FITKGeoToMeshMapper();
    }
    FITKUnstructuredMesh::~FITKUnstructuredMesh()
    {
        if (_meshTopo) delete _meshTopo;

        if (_geoToMeshMapper) {
            delete _geoToMeshMapper;
            _geoToMeshMapper = nullptr;
        }
    }
    Interface::FITKModelEnum::AbsModelType FITKUnstructuredMesh::getAbsModelType()
    {
        //抽象类型
        return FITKModelEnum::AbsModelType::AMTunstructuredMesh;
    }

    bool FITKUnstructuredMesh::isComponentValid(FITKModelEnum::FITKModelSetType compType, const QList<int> & members)
    {
        switch (compType)
        {
        case Interface::FITKModelEnum::FMSNode:
        {
            for (const int & member : members)
            {
                if (!this->getNodeAt(member))
                {
                    return false;
                }
            }

            break;
        }       
        case Interface::FITKModelEnum::FMSElem:
        {
            for (const int & member : members)
            {
                if (!this->getElementAt(member))
                {
                    return false;
                }
            }

            break;
        }
        default:
            return false;
        }

        return true;
    }

    void FITKUnstructuredMesh::update()
    {
        //更新
    }

    void FITKUnstructuredMesh::clearMesh()
    {
        this->removeAllNode();
        this->removeAllElement();
    }

    FITKModelEnum::FITKMeshDim FITKUnstructuredMesh::getMeshDim()
    {
        //错误判断
        if (_elementList.isEmpty()) return FITKModelEnum::FITKMeshDim::FMDimNone;
        FITKAbstractElement* ele = _elementList.at(0);
        if (ele == nullptr) return FITKModelEnum::FITKMeshDim::FMDimNone;
        //第一个单元类型
        FITKModelEnum::FITKEleType type = ele->getEleType();
        FITKModelEnum::FITKMeshDim dim = FITKModelEnum::GetElementDim(type);

        //遍历单元
        const int n = this->getElementCount();
        for (int i =1; i<n; ++i)
        {
            FITKAbstractElement* elei = _elementList.at(i);
            if(elei == nullptr) continue;
            //第i个单元与第一个单元维度是否一致
            FITKModelEnum::FITKEleType typei = elei->getEleType();
            FITKModelEnum::FITKMeshDim dimi = FITKModelEnum::GetElementDim(typei);
            if (dimi != dim) return FITKModelEnum::FITKMeshDim::FMDimMix;
        }
        return dim;

    }

    FITKElemntQuality FITKUnstructuredMesh::checkElementQuality(const int eleIndex)
    {
        FITKElemntQuality q;
        //获取单元
        FITKAbstractElement* ele = this->getElementAt(eleIndex);
        if (ele == nullptr) return q;
        //遍历全部边
        const int n = ele->getEdgeCount();
        QList <double> edgeLength;
        for (int i =0;i< n; ++i)
        {
            QList<int> edge = ele->getEdge(i);
            //边的长度
            double l = this->calMeshEdgeLength(edge);
            if(l >0)
                edgeLength.append(l);
        }
        if (edgeLength.isEmpty()) return q;
        qSort(edgeLength);
        //记录最长最短边
        q._minEdgeLength = edgeLength.first();
        q._maxEdgeLength = edgeLength.last();
        //计算长宽比
        q._aspectRatio = q._maxEdgeLength / q._minEdgeLength;
        
        //计算扩展质量指标
        q._jacobian = calculateJacobian(eleIndex);
        q._skewness = calculateSkewness(eleIndex);
        q._warpage = calculateWarpage(eleIndex);
        q._scaledJacobian = calculateScaledJacobian(eleIndex);
        q._shape = calculateShape(eleIndex);
        q._condition = calculateCondition(eleIndex);
        
        return q;
    }
    
    double FITKUnstructuredMesh::calculateJacobian(const int eleIndex)
    {
        FITKAbstractElement* ele = this->getElementAt(eleIndex);
        if (ele == nullptr) return 0.0;
        
        QList<int> nodes = ele->getAllNodes();
        if (nodes.size() < 3) return 0.0;
        
        Core::FITKNode* n0 = this->getNodeByID(nodes[0]);
        Core::FITKNode* n1 = this->getNodeByID(nodes[1]);
        Core::FITKNode* n2 = this->getNodeByID(nodes[2]);
        
        if (!n0 || !n1 || !n2) return 0.0;
        
        double v1[3], v2[3];
        for (int i = 0; i < 3; ++i) {
            v1[i] = (*n1)[i] - (*n0)[i];
            v2[i] = (*n2)[i] - (*n0)[i];
        }
        
        double jacobian = v1[0] * v2[1] - v1[1] * v2[0];
        
        if (nodes.size() >= 4) {
            Core::FITKNode* n3 = this->getNodeByID(nodes[3]);
            if (n3) {
                double v3[3];
                for (int i = 0; i < 3; ++i) {
                    v3[i] = (*n3)[i] - (*n0)[i];
                }
                jacobian = v1[0] * (v2[1] * v3[2] - v2[2] * v3[1])
                         - v1[1] * (v2[0] * v3[2] - v2[2] * v3[0])
                         + v1[2] * (v2[0] * v3[1] - v2[1] * v3[0]);
            }
        }
        
        return std::abs(jacobian);
    }
    
    double FITKUnstructuredMesh::calculateSkewness(const int eleIndex)
    {
        FITKAbstractElement* ele = this->getElementAt(eleIndex);
        if (ele == nullptr) return 0.0;
        
        FITKModelEnum::FITKEleType eleType = ele->getEleType();
        QList<int> nodes = ele->getAllNodes();
        
        double skewness = 0.0;
        
        switch (eleType) {
        case FITKModelEnum::Tri3:
        case FITKModelEnum::Tri6: {
            if (nodes.size() < 3) return 0.0;
            Core::FITKNode* n0 = this->getNodeByID(nodes[0]);
            Core::FITKNode* n1 = this->getNodeByID(nodes[1]);
            Core::FITKNode* n2 = this->getNodeByID(nodes[2]);
            if (!n0 || !n1 || !n2) return 0.0;
            
            double a = Core::Distance(*n0, *n1);
            double b = Core::Distance(*n1, *n2);
            double c = Core::Distance(*n2, *n0);
            
            double maxEdge = qMax(a, qMax(b, c));
            double s = (a + b + c) / 2.0;
            double area = sqrt(s * (s-a) * (s-b) * (s-c));
            
            double optimalArea = (sqrt(3.0) / 4.0) * maxEdge * maxEdge;
            skewness = 1.0 - qMin(area, optimalArea) / qMax(area, optimalArea);
            break;
        }
        case FITKModelEnum::Quad4:
        case FITKModelEnum::Quad8: {
            if (nodes.size() < 4) return 0.0;
            Core::FITKNode* n0 = this->getNodeByID(nodes[0]);
            Core::FITKNode* n1 = this->getNodeByID(nodes[1]);
            Core::FITKNode* n2 = this->getNodeByID(nodes[2]);
            Core::FITKNode* n3 = this->getNodeByID(nodes[3]);
            if (!n0 || !n1 || !n2 || !n3) return 0.0;
            
            double d1 = Core::Distance(*n0, *n2);
            double d2 = Core::Distance(*n1, *n3);
            
            double maxEdge = 0.0;
            maxEdge = qMax(maxEdge, Core::Distance(*n0, *n1));
            maxEdge = qMax(maxEdge, Core::Distance(*n1, *n2));
            maxEdge = qMax(maxEdge, Core::Distance(*n2, *n3));
            maxEdge = qMax(maxEdge, Core::Distance(*n3, *n0));
            
            double optimalDiag = maxEdge * sqrt(2.0);
            skewness = qAbs(d1 - optimalDiag) / optimalDiag;
            skewness = qMax(skewness, qAbs(d2 - optimalDiag) / optimalDiag);
            break;
        }
        default:
            break;
        }
        
        return skewness;
    }
    
    double FITKUnstructuredMesh::calculateWarpage(const int eleIndex)
    {
        FITKAbstractElement* ele = this->getElementAt(eleIndex);
        if (ele == nullptr) return 0.0;
        
        FITKModelEnum::FITKEleType eleType = ele->getEleType();
        QList<int> nodes = ele->getAllNodes();
        
        if (eleType != FITKModelEnum::Quad4 && eleType != FITKModelEnum::Quad8 &&
            eleType != FITKModelEnum::Hex8 && eleType != FITKModelEnum::Hex20) {
            return 0.0;
        }
        
        if (nodes.size() < 4) return 0.0;
        
        Core::FITKNode* n0 = this->getNodeByID(nodes[0]);
        Core::FITKNode* n1 = this->getNodeByID(nodes[1]);
        Core::FITKNode* n2 = this->getNodeByID(nodes[2]);
        Core::FITKNode* n3 = this->getNodeByID(nodes[3]);
        
        if (!n0 || !n1 || !n2 || !n3) return 0.0;
        
        Core::FITKVec3D v1(*n1, *n0);
        Core::FITKVec3D v2(*n2, *n0);
        Core::FITKVec3D normal1 = Core::CrossProduct(v1, v2);
        
        Core::FITKVec3D v3(*n3, *n2);
        Core::FITKVec3D v4(*n0, *n2);
        Core::FITKVec3D normal2 = Core::CrossProduct(v3, v4);
        
        double len1 = sqrt(normal1[0]*normal1[0] + normal1[1]*normal1[1] + normal1[2]*normal1[2]);
        double len2 = sqrt(normal2[0]*normal2[0] + normal2[1]*normal2[1] + normal2[2]*normal2[2]);
        
        if (len1 < 1e-10 || len2 < 1e-10) return 1.0;
        
        double dot = normal1[0]*normal2[0] + normal1[1]*normal2[1] + normal1[2]*normal2[2];
        double cosAngle = dot / (len1 * len2);
        
        return 1.0 - qAbs(cosAngle);
    }
    
    double FITKUnstructuredMesh::calculateScaledJacobian(const int eleIndex)
    {
        double jac = calculateJacobian(eleIndex);
        if (jac < 1e-10) return 0.0;
        
        FITKAbstractElement* ele = this->getElementAt(eleIndex);
        if (ele == nullptr) return 0.0;
        
        const int n = ele->getEdgeCount();
        if (n == 0) return 0.0;
        
        double maxEdgeLen = 0.0;
        for (int i = 0; i < n; ++i) {
            QList<int> edge = ele->getEdge(i);
            double len = this->calMeshEdgeLength(edge);
            maxEdgeLen = qMax(maxEdgeLen, len);
        }
        
        if (maxEdgeLen < 1e-10) return 0.0;
        
        int dim = ele->getElementDim();
        double scaleFactor = pow(maxEdgeLen, dim);
        
        return jac / scaleFactor;
    }
    
    double FITKUnstructuredMesh::calculateShape(const int eleIndex)
    {
        FITKAbstractElement* ele = this->getElementAt(eleIndex);
        if (ele == nullptr) return 0.0;
        
        double volume = calculateElementVolume(eleIndex);
        if (volume < 1e-10) return 0.0;
        
        const int n = ele->getEdgeCount();
        double sumEdgeLenSq = 0.0;
        for (int i = 0; i < n; ++i) {
            QList<int> edge = ele->getEdge(i);
            double len = this->calMeshEdgeLength(edge);
            sumEdgeLenSq += len * len;
        }
        
        if (sumEdgeLenSq < 1e-10) return 0.0;
        
        int dim = ele->getElementDim();
        double factor = (dim == 2) ? 4.0 * sqrt(3.0) : 6.0 * pow(2.0, 2.0/3.0);
        
        return factor * volume / sumEdgeLenSq;
    }
    
    double FITKUnstructuredMesh::calculateCondition(const int eleIndex)
    {
        FITKAbstractElement* ele = this->getElementAt(eleIndex);
        if (ele == nullptr) return 0.0;
        
        QList<int> nodes = ele->getAllNodes();
        if (nodes.size() < 3) return 0.0;
        
        Core::FITKNode* n0 = this->getNodeByID(nodes[0]);
        Core::FITKNode* n1 = this->getNodeByID(nodes[1]);
        Core::FITKNode* n2 = this->getNodeByID(nodes[2]);
        
        if (!n0 || !n1 || !n2) return 0.0;
        
        double J[2][2];
        J[0][0] = (*n1)[0] - (*n0)[0];
        J[0][1] = (*n2)[0] - (*n0)[0];
        J[1][0] = (*n1)[1] - (*n0)[1];
        J[1][1] = (*n2)[1] - (*n0)[1];
        
        double det = J[0][0] * J[1][1] - J[0][1] * J[1][0];
        if (std::abs(det) < 1e-10) return 1e10;
        
        double JTJ[2][2];
        JTJ[0][0] = J[0][0] * J[0][0] + J[1][0] * J[1][0];
        JTJ[0][1] = J[0][0] * J[0][1] + J[1][0] * J[1][1];
        JTJ[1][0] = JTJ[0][1];
        JTJ[1][1] = J[0][1] * J[0][1] + J[1][1] * J[1][1];
        
        double trace = JTJ[0][0] + JTJ[1][1];
        double detJTJ = JTJ[0][0] * JTJ[1][1] - JTJ[0][1] * JTJ[1][0];
        
        double sqrtDiscrim = sqrt(trace * trace - 4.0 * detJTJ);
        double lambdaMax = (trace + sqrtDiscrim) / 2.0;
        double lambdaMin = (trace - sqrtDiscrim) / 2.0;
        
        if (lambdaMin < 1e-10) return 1e10;
        
        return sqrt(lambdaMax / lambdaMin);
    }
    
    double FITKUnstructuredMesh::calculateElementVolume(const int eleIndex)
    {
        FITKAbstractElement* ele = this->getElementAt(eleIndex);
        if (ele == nullptr) return 0.0;
        
        QList<int> nodes = ele->getAllNodes();
        FITKModelEnum::FITKEleType eleType = ele->getEleType();
        
        switch (eleType) {
        case FITKModelEnum::Tri3:
        case FITKModelEnum::Tri6: {
            if (nodes.size() < 3) return 0.0;
            Core::FITKNode* n0 = this->getNodeByID(nodes[0]);
            Core::FITKNode* n1 = this->getNodeByID(nodes[1]);
            Core::FITKNode* n2 = this->getNodeByID(nodes[2]);
            if (!n0 || !n1 || !n2) return 0.0;
            
            double a = Core::Distance(*n0, *n1);
            double b = Core::Distance(*n1, *n2);
            double c = Core::Distance(*n2, *n0);
            double s = (a + b + c) / 2.0;
            return sqrt(s * (s-a) * (s-b) * (s-c));
        }
        case FITKModelEnum::Quad4:
        case FITKModelEnum::Quad8: {
            if (nodes.size() < 4) return 0.0;
            Core::FITKNode* n0 = this->getNodeByID(nodes[0]);
            Core::FITKNode* n1 = this->getNodeByID(nodes[1]);
            Core::FITKNode* n2 = this->getNodeByID(nodes[2]);
            Core::FITKNode* n3 = this->getNodeByID(nodes[3]);
            if (!n0 || !n1 || !n2 || !n3) return 0.0;
            
            Core::FITKVec3D v1(*n1, *n0);
            Core::FITKVec3D v2(*n3, *n0);
            Core::FITKVec3D v3(*n2, *n1);
            Core::FITKVec3D v4(*n2, *n3);
            
            Core::FITKPoint cross1 = Core::CrossProduct(v1, v2);
            Core::FITKPoint cross2 = Core::CrossProduct(v3, v4);
            
            double area1 = sqrt(cross1[0]*cross1[0] + cross1[1]*cross1[1] + cross1[2]*cross1[2]) / 2.0;
            double area2 = sqrt(cross2[0]*cross2[0] + cross2[1]*cross2[1] + cross2[2]*cross2[2]) / 2.0;
            return area1 + area2;
        }
        case FITKModelEnum::Tet4:
        case FITKModelEnum::Tet10: {
            if (nodes.size() < 4) return 0.0;
            Core::FITKNode* n0 = this->getNodeByID(nodes[0]);
            Core::FITKNode* n1 = this->getNodeByID(nodes[1]);
            Core::FITKNode* n2 = this->getNodeByID(nodes[2]);
            Core::FITKNode* n3 = this->getNodeByID(nodes[3]);
            if (!n0 || !n1 || !n2 || !n3) return 0.0;
            
            Core::FITKVec3D v1(*n1, *n0);
            Core::FITKVec3D v2(*n2, *n0);
            Core::FITKVec3D v3(*n3, *n0);
            
            return std::abs(Core::DotProduct(v1, Core::CrossProduct(v2, v3))) / 6.0;
        }
        case FITKModelEnum::Hex8:
        case FITKModelEnum::Hex20: {
            if (nodes.size() < 8) return 0.0;
            double volume = 0.0;
            
            int tets[6][4] = {
                {0, 1, 3, 4}, {1, 2, 3, 6},
                {1, 3, 4, 6}, {3, 4, 6, 7},
                {1, 4, 5, 6}, {4, 6, 7, 5}
            };
            
            for (int i = 0; i < 6; ++i) {
                Core::FITKNode* n0 = this->getNodeByID(nodes[tets[i][0]]);
                Core::FITKNode* n1 = this->getNodeByID(nodes[tets[i][1]]);
                Core::FITKNode* n2 = this->getNodeByID(nodes[tets[i][2]]);
                Core::FITKNode* n3 = this->getNodeByID(nodes[tets[i][3]]);
                
                if (n0 && n1 && n2 && n3) {
                    Core::FITKVec3D v1(*n1, *n0);
                    Core::FITKVec3D v2(*n2, *n0);
                    Core::FITKVec3D v3(*n3, *n0);
                    volume += std::abs(Core::DotProduct(v1, Core::CrossProduct(v2, v3))) / 6.0;
                }
            }
            return volume;
        }
        default:
            return 0.0;
        }
    }

    bool FITKUnstructuredMesh::getElementDirection(double* dir, const int id)
    {
        FITKAbstractElement* ele = this->getElementByID(id);
        if (ele == nullptr) return false;
        int dim = ele->getElementDim();
        switch (dim)
        {
        //线单元方向
        case 1: return this->getLineEleDirection(dir, ele);
        //壳单元方向
        case 2: return this->getShellEleDirection(dir, ele);
        default: break;
        }
        return false;
    }

    bool FITKUnstructuredMesh::hasOrphanMesh()
    {
        // 检查网格节点。
        for (Core::FITKNode* node : _nodeList)
        {
            if (!node)
            {
                continue;
            }

            if (!node->getNativeFlag())
            {
                return true;
            }
        }

        // 检查网格单元。
        for (FITKAbstractElement* ele : _elementList)
        {
            if (!ele)
            {
                continue;
            }

            if (!ele->getNativeFlag())
            {
                return true;
            }
        }

        return false;
    }

    bool FITKUnstructuredMesh::hasNativeMesh()
    {
        // 检查网格节点。
        for (Core::FITKNode* node : _nodeList)
        {
            if (!node)
            {
                continue;
            }

            if (node->getNativeFlag())
            {
                return true;
            }
        }

        // 检查网格单元。
        for (FITKAbstractElement* ele : _elementList)
        {
            if (!ele)
            {
                continue;
            }

            if (ele->getNativeFlag())
            {
                return true;
            }
        }

        return false;
    }

    void FITKUnstructuredMesh::clearNativeMesh()
    {
        // 移除抽象层网格数据。
        int nPoints = this->getNodeCount();
        int nEles = this->getElementCount();

        for (int i = nEles - 1; i >= 0; i--)
        {
            FITKAbstractElement* element = this->getElementAt(i);
            if (!element)
            {
                continue;
            }

            if (!element->getNativeFlag())
            {
                continue;
            }

            this->fastRemoveElementAt(i);
        }

        // 移除节点。
        for (int i = nPoints - 1; i >= 0; i--)
        {
            Core::FITKNode* node = this->getNodeAt(i);
            if (!node)
            {
                continue;
            }

            if (!node->getNativeFlag())
            {
                continue;
            }

            // 循环索引为节点索引。
            this->fastRemoveNodeAt(i);
        }

        // 重新构建ID索引映射。
        this->updateNodeIDIndexMap();
        this->updateElementIDIndexMap();

        // 重建VTK数据。
        update();

        //清除几何与网格映射关系
        if (_geoToMeshMapper) {
            _geoToMeshMapper->clear();
        }
    }

    void FITKUnstructuredMesh::clearOrphanMesh()
    {
        // 移除抽象层网格数据。
        int nPoints = this->getNodeCount();
        int nEles = this->getElementCount();

        for (int i = nEles - 1; i >= 0; i--)
        {
            FITKAbstractElement* element = this->getElementAt(i);            
            if (!element)
            {
                continue;
            }

            if (element->getNativeFlag())
            {
                continue;
            }

            this->fastRemoveElementAt(i);
        }

        // 移除节点。
        for (int i = nPoints - 1; i >= 0; i--)
        {
            Core::FITKNode* node = this->getNodeAt(i);
            if (!node)
            {
                continue;
            }

            if (node->getNativeFlag())
            {
                continue;
            }

            // 循环索引为节点索引。
            this->fastRemoveNodeAt(i);
        }

        // 重新构建ID索引映射。
        this->updateNodeIDIndexMap();
        this->updateElementIDIndexMap();

        // 重建VTK数据。
        update();
    }

    bool FITKUnstructuredMesh::buildUnstructuredMeshTopo()
    {
        if(_meshTopo == nullptr)
            _meshTopo = new FITKUnstructuredMeshTopo(this);
        return _meshTopo->buildUnstructuredMeshTopo();
    }

    double FITKUnstructuredMesh::calMeshEdgeLength(const QList<int> & edge)
    {
        double length = 0;
        //错误判断
        const int n = edge.size();
        if (n < 2) return -1;
        //计算全部的点的长度
        for (int i =0; i< n-1; ++i)
        {
            //相邻两个点的距离
            const Core::FITKNode* node1 = this->getNodeByID(edge[i]);
            const Core::FITKNode* node2 = this->getNodeByID(edge[i+1]);
            if(node1 && node2)
                length += Core::Distance(*node2, *node1);
        }
        return length;
    }

    bool FITKUnstructuredMesh::getLineEleDirection(double* dir, FITKAbstractElement* ele)
    {
        if (!ele) return false;
        //获取第一个边
        int ne = ele->getEdgeCount();
        if (ne <= 0) return false;
        QList<int> ed = ele->getEdge(0);
        if (ed.size() < 2) return false;
        //边上的两个点
        Core::FITKNode* n1 = this->getNodeByID(ed.first());
        Core::FITKNode* n2 = this->getNodeByID(ed.last());
        if (n1 == nullptr || n2 == nullptr) return false;
        //第一点到第二点的矢量
        Core::FITKVec3D v(*n2,*n1);
        for (int i =0; i<3; ++i)
            dir[i] = v[i];
        return true;
    }

    bool FITKUnstructuredMesh::getShellEleDirection(double* dir, FITKAbstractElement* ele)
    {
        if (!ele) return false;
        //至少三条边组成一个面
        int ne = ele->getEdgeCount();
        if (ne <= 2) return false;
        //获取第一条边
        QList<int> ed1 = ele->getEdge(0);
        if (ed1.size() < 2) return false;
        Core::FITKNode* n1 = this->getNodeByID(ed1.first());
        Core::FITKNode* n2 = this->getNodeByID(ed1.last());
        //获取第二条边
        QList<int> ed2 = ele->getEdge(1);
        if (ed2.size() < 2) return false;
        Core::FITKNode* n3 = this->getNodeByID(ed2.last());
        if (n1 == nullptr || n2 == nullptr || n3 == nullptr) return false;
        //两个矢量的叉乘为最终结果
        Core::FITKVec3D v1(*n2, *n1);
        Core::FITKVec3D v2(*n3, *n1);
        Core::FITKPoint p = Core::CrossProduct(v1, v2);
        //赋值输出
        for (int i = 0; i < 3; ++i)
            dir[i] = p[i];
        return true;
    }

    FITKUnstructuredMeshTopo *FITKUnstructuredMesh::getUnstructuredMeshTopo()
    {
        return _meshTopo;
    }

    bool FITKUnstructuredMesh::getPointCoor(int pointID, double* coor, int modelIndex)
    {
        Core::FITKNode* node = this->getNodeByID(pointID);
        if (!node)
        {
            return false;
        }

        node->getCoor(coor);

        return true;
    }

    FITKGeoToMeshMapper * FITKUnstructuredMesh::getGeoToMeshMapper()
    {
        return _geoToMeshMapper;
    }

    unsigned int FITKUnstructuredMesh::getMeshDimBit()
    {
        unsigned int dimBit = 0;
        if (_elementList.isEmpty()) return dimBit; 

        //遍历单元
        const int n = this->getElementCount();
        for (int i =0; i<n; ++i)
        {
            FITKAbstractElement* elei = _elementList.at(i);
            if(elei == nullptr) continue;
         
            FITKModelEnum::FITKEleType typei = elei->getEleType();
            FITKModelEnum::FITKMeshDim dimi = FITKModelEnum::GetElementDim(typei);
            dimBit |= static_cast<unsigned int>(dimi);
        }
        return dimBit;
    }
        
}
