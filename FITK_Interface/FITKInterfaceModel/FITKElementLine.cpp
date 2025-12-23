#include "FITKElementLine.h"

namespace Interface
{

    FITKElementLine2::FITKElementLine2()
    {
        //填充2个节点
        this->setNodeCount(this->getNodeCount());
    }

    FITKModelEnum::FITKEleType FITKElementLine2::getEleType()
    {
        return FITKModelEnum::FITKEleType::Line2;
    }

    bool FITKElementLine2::getQuadraticFlag()
    {
        return false;
    }

    int FITKElementLine2::getNodeCount()
    {
        //3个节点
        return 2;
    }

    int FITKElementLine2::getFaceCount()
    {
        return 0;
    }

    int FITKElementLine2::getEdgeCount()
    {
        return 1;
    }

    QList<int> FITKElementLine2::getEdge(const int index)
    {
        return _elemenTopo;
    }

    int FITKElementLine2::getElementDim()
    {
        return 1;
    }

    FITKElementLine3::FITKElementLine3()
    {
        //填充6个节点
        this->setNodeCount(this->getNodeCount());
    }

    Interface::FITKModelEnum::FITKEleType FITKElementLine3::getEleType()
    {
        return FITKModelEnum::FITKEleType::Line3;
    }

    bool FITKElementLine3::getQuadraticFlag()
    {
        return true;
    }

    int FITKElementLine3::getNodeCount()
    {
        //3节点
        return 3;
    }

    int FITKElementLine3::getFaceCount()
    {
        return 0;
    }

    int FITKElementLine3::getEdgeCount()
    {
        return 1;
    }

    QList<int> FITKElementLine3::getEdge(const int index)
    {
        return _elemenTopo;
    }

    int FITKElementLine3::getElementDim()
    {
        return 1;
    }

}


