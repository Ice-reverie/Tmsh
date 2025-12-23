#include "FITKGmshAdaptorElements.h"
#include "FITKGmshMshIOReader.h"
#include "FITKGmshMshIOReaderTool.h"
#include "FITK_Interface/FITKInterfaceIO/FITKAbstractTextReader.h"
#include "FITK_Interface/FITKInterfaceIO/FITKAbstractTextWriter.h"
#include "FITK_Interface/FITKInterfaceMesh/FITKUnstructuredMeshVTK.h"
#include "FITK_Interface/FITKInterfaceModel/FITKElementFactory.h"
#include "FITK_Interface/FITKInterfaceModel/FITKAbstractElement.h"
#include "FITK_Interface/FITKInterfaceModel/FITKModelSet.h"
#include "FITK_Interface/FITKInterfaceModel/FITKComponentManager.h"

namespace Gmsh
{
    QString FITKGmshAdaptorElements::getAdaptorClass()
    {
        return "FITKGmshAdaptorElements";
    }

    void FITKGmshAdaptorElements::setComponentManager(Interface::FITKComponentManager* setSurfManager)
    {
        m_setSurfManager = setSurfManager;
    }

    bool FITKGmshAdaptorElements::adaptR()
    {
        //获取网格数据对象
        Interface::FITKUnstructuredMesh* meshObj = dynamic_cast<Interface::FITKUnstructuredMesh*>(_dataObj);
        if (!meshObj || !_reader) return false;
        //上一行
        QString line = _reader->previousLine();
        //获取关键字
        QString keywords = line.trimmed().toLower();
        keywords.remove("$");
        if (keywords != "elements") return false;
        this->readElementsInfo();

        //开始读取节点
        while (!_reader->atEnd())
        {
            QString line = _reader->readLine().trimmed();
            if (line.contains("$"))break;
            //维度 维度一致的第几个 单元类型 单元数
            QStringList info = line.split(' ', QString::SkipEmptyParts);
            if (info.size() != 4)continue;
            bool readOK = this->readElement(info);
            if (!readOK)
                return false;
        }

        return true;
    }

    bool FITKGmshAdaptorElements::adaptW()
    {
        return true;
    }

    bool FITKGmshAdaptorElements::readElementsInfo()
    {
        QString line = _reader->nextLine().trimmed();
        if (line.split(' ', QString::SkipEmptyParts).size() != 4) return false;
        //实体块 单元数 单元最小标号 单元最大标号
        QString elementsInfo = _reader->readLine().trimmed();
        QStringList info = elementsInfo.split(' ', QString::SkipEmptyParts);
        if (info.size() != 4) return false;
        bool ok = false;
        //实体块
        int entityNum = info.at(0).toInt(&ok);
        if (!ok) return false;
        //节点数
        int elementsNum = info.at(1).toInt(&ok);
        if (!ok) return false;
        //节点最小标号
        int elementMinID = info.at(2).toInt(&ok);
        if (!ok) return false;
        //节点最大标号
        int elementMaxID = info.at(3).toInt(&ok);
        if (!ok) return false;
        return true;
    }

    bool FITKGmshAdaptorElements::readElement(QStringList info)
    {
        Interface::FITKUnstructuredMesh* meshObj = dynamic_cast<Interface::FITKUnstructuredMesh*>(_dataObj);
        FITKGmshMshIOReader* reader = dynamic_cast<FITKGmshMshIOReader*>(_reader);
        if (!meshObj || !reader || info.size() != 4) return false;
        FITKGmshMshIOReaderTool* tool = reader->getReaderTool();
        if (!tool) return false;
        QList<int> filterDim = reader->getReadFilterDim();
        //获取节点数
        bool ok = false;
        int dim = info.at(0).toInt(&ok);
        if (!ok) return false;
        if (dim >= 4) return false;
        int elemType = info.at(2).toInt(&ok);
        if (!ok) return false;
        int elemNum = info.at(3).toInt(&ok);
        if (!ok) return false;
        //获取单元数据
        QList<int> elementsType;
        QList<int> elementsID;
        QList<QList<int>> elemsNodeID;
        for (int i = 0; i < elemNum; ++i)
        {
            if (_reader->atEnd())
                break;
            QString line = _reader->readLine().trimmed();
            if (line.contains("$"))break;
            //单元编号 节点1 ...
            QStringList info = line.split(' ', QString::SkipEmptyParts);
            if (info.isEmpty())continue;
            //获取单元编号
            int elemID = info.at(0).toInt(&ok);
            if (!ok) return false;
            //获取单元类型
            int nodeNum = info.size() - 1;
            QList<int> nodeIndexs;
            int type = this->getElementType(elemType, nodeNum, nodeIndexs);
            //获取单元节点id
            QList<int> nodeIDs;
            for (int index : nodeIndexs)
            {
                QString nodeIDStr = info.at(index + 1);
                int nodeID = nodeIDStr.toInt(&ok);
                if (!ok) return false;
                nodeIDs.append(nodeID);
            }
            elementsID.append(elemID);
            elementsType.append(type);
            elemsNodeID.append(nodeIDs);
        }
        if (elementsType.size() != elemNum || elemsNodeID.size() != elemNum || elementsID.size() != elemNum)
            return false;
        if (dim == 0 || filterDim.contains(dim)) return true;
        //添加单元
        for (int i = 0; i < elemNum; ++i)
        {
            int elemID = elementsID.at(i);
            Interface::FITKModelEnum::FITKEleType type = Interface::FITKModelEnum::FITKEleType(elementsType.at(i));
            QList<int> nodeIDs = elemsNodeID.at(i);
            tool->addElement(elemID, type, nodeIDs);

            //Interface::FITKAbstractElement* element = Interface::FITKElementFactory::createElement(type);
            //if (!element) continue;
            //element->setEleID(elemID);
            //element->setNodeID(nodeIDs);
            //meshObj->appendElement(element);
        }
        //创建集合
        if (elemNum > 0 && elementsType.at(0) != Interface::FITKModelEnum::FITKEleType::EleNone)
        {
            //this->readSetData(elementsID);
            tool->addElementSet(dim, elementsID);
        }
        return true;
    }

    bool FITKGmshAdaptorElements::readSetData(QList<int> elemIdList)
    {
        if (!m_setSurfManager) return false;
        QString name = m_setSurfManager->checkName("GmshSet-1");
        Interface::FITKModelSet* setData = new Interface::FITKModelSet(Interface::FITKModelEnum::FITKModelSetType::FMSElem);
        setData->setDataObjectName(name);
        setData->setAbsoluteMember(elemIdList);
        m_setSurfManager->appendDataObj(setData);
        return true;
    }

    int FITKGmshAdaptorElements::getElementType(int type, int & nodeNum, QList<int>& nodeIndexs)
    {
        if (type == 1 && nodeNum == 2)
        {
            nodeIndexs << 0 << 1;
            return Interface::FITKModelEnum::FITKEleType::Line2;
        }
        else if (type == 2 && nodeNum == 3)
        {
            nodeIndexs << 0 << 1 << 2;
            return Interface::FITKModelEnum::FITKEleType::Tri3;
        }
        else if (type == 9 && nodeNum == 6)
        {
            nodeIndexs << 0 << 1 << 2 << 3 << 4 << 5;
            return Interface::FITKModelEnum::FITKEleType::Tri6;
        }
        else if (type == 3 && nodeNum == 4)
        {
            nodeIndexs << 0 << 1 << 2 << 3;
            return Interface::FITKModelEnum::FITKEleType::Quad4;
        }
        else if (type == 10 && nodeNum == 9)
        {
            nodeNum = 8;
            nodeIndexs << 0 << 1 << 2 << 3 << 4 << 5 << 6 << 7;
            return Interface::FITKModelEnum::FITKEleType::Quad8;
        }
        else if (type == 4 && nodeNum == 4)
        {
            nodeIndexs << 0 << 1 << 2 << 3;
            return Interface::FITKModelEnum::FITKEleType::Tet4;
        }
        else if (type == 11 && nodeNum == 10)
        {
            nodeIndexs << 0 << 1 << 2 << 3 << 4 << 5 << 6 << 7 << 9 << 8;
            return Interface::FITKModelEnum::FITKEleType::Tet10;
        }
        else if (type == 5 && nodeNum == 8)
        {
            nodeIndexs << 0 << 1 << 2 << 3 << 4 << 5 << 6 << 7;
            return Interface::FITKModelEnum::FITKEleType::Hex8;
        }
        else if (type == 12 && nodeNum == 27)
        {
            nodeIndexs << 0 << 1 << 2 << 3 << 4 << 5 << 6 << 7 << 8 << 11 
                << 13 << 9 << 16 << 18 << 19 << 17 << 10 << 12 << 14 << 15;
            return Interface::FITKModelEnum::FITKEleType::Hex20;
        }
        return Interface::FITKModelEnum::FITKEleType::EleNone;
    }
}


