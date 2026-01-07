#include "FITKElementPolygon.h"

namespace Interface
{
    FITKModelEnum::FITKEleType FITKElementPolygon::getEleType()
    {
        return FITKModelEnum::FITKEleType::Polygon;
    }

    bool FITKElementPolygon::getQuadraticFlag()
    {
        return false;
    }

    void FITKElementPolygon::setNodeCount(const int c)
    {
        FITKAbstractElement::setNodeCount(c);
    }


    int FITKElementPolygon::getNodeCount()
    {
        return _elemenTopo.size();
    }

    int FITKElementPolygon::getFaceCount()
    {
        return 1;
    }

    int FITKElementPolygon::getElementDim()
    {
        return 2;
    }

    int FITKElementPolygon::getEdgeCount()
    {
        return _elemenTopo.size();
    }

    QList<int> FITKElementPolygon::getFace(const int index, const bool & lowerOrder)
    {
        Q_UNUSED(index);
        Q_UNUSED(lowerOrder);
        // 返回单元的所有节点
        return _elemenTopo;
    }

    QList<int> FITKElementPolygon::getEdge(const int index)
    {
        const int np = _elemenTopo.size();
        QList<int> edge;
        edge << index % np << (index + 1) % np;
        return edge;
    }

}


