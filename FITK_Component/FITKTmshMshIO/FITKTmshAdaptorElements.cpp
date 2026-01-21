#include "FITKTmshAdaptorElements.h"
#include "FITKTmshMshIOReader.h"
#include "FITKTmshMshIOReaderTool.h"
#include "FITK_Interface/FITKInterfaceIO/FITKAbstractTextReader.h"
#include "FITK_Interface/FITKInterfaceIO/FITKAbstractTextWriter.h"
#include "FITK_Interface/FITKInterfaceMesh/FITKUnstructuredMeshVTK.h"
#include "FITK_Interface/FITKInterfaceModel/FITKElementFactory.h"
#include "FITK_Interface/FITKInterfaceModel/FITKAbstractElement.h"
#include "FITK_Interface/FITKInterfaceModel/FITKModelSet.h"
#include "FITK_Interface/FITKInterfaceModel/FITKComponentManager.h"

namespace Tmsh
{
    QString FITKTmshAdaptorElements::getAdaptorClass()
    {
        return "FITKTmshAdaptorElements";
    }

    void FITKTmshAdaptorElements::setComponentManager(Interface::FITKComponentManager* setSurfManager)
    {
        m_setSurfManager = setSurfManager;
    }

	bool FITKTmshAdaptorElements::adaptR()
	{
		//获取网格数据对象
		Interface::FITKUnstructuredMesh* meshObj = dynamic_cast<Interface::FITKUnstructuredMesh*>(_dataObj);
		if (!meshObj || !_reader) return false;

		QString line = _reader->nextLine().trimmed();
		count = 1;
		//开始读取节点
		while (!_reader->atEnd())
		{
			QString line = _reader->readLine().trimmed();
			if (line.contains("Edges"))
			{
				break;
			}
			//维度 维度一致的第几个 单元类型 单元数
			QStringList info = line.split(' ', QString::SkipEmptyParts);
			if (info.size() != 4)continue;
			bool readOK = this->readElement(info);
			count++;
			if (!readOK)
				return false;
		}

		return true;
	}

    bool FITKTmshAdaptorElements::adaptW()
    {
        return true;
    }

    bool FITKTmshAdaptorElements::readElementsInfo()
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

    bool FITKTmshAdaptorElements::readElement(QStringList info)
    {
		Interface::FITKUnstructuredMesh* meshObj = dynamic_cast<Interface::FITKUnstructuredMesh*>(_dataObj);
		FITKTmshMshIOReader* reader = dynamic_cast<FITKTmshMshIOReader*>(_reader);
		if (!meshObj || !reader || info.size() != 4) return false;
		FITKTmshMshIOReaderTool* tool = reader->getReaderTool();
		if (!tool) return false;
		bool ok = false;
		//获取单元编号
		int elemID = count;
		//获取单元类型
		int nodeNum = info.size() - 1;
		QList<int> nodeIndexs;
		int type = this->getElementType(2, nodeNum, nodeIndexs);
		//获取单元节点id
		QList<int> nodeIDs;
		for (int index : nodeIndexs)
		{
			QString nodeIDStr = info.at(index);
			int nodeID = nodeIDStr.toInt(&ok);
			if (!ok) return false;
			nodeIDs.append(nodeID);
		}
		//添加单元
		tool->addElement(elemID, type, nodeIDs);
		return true;
    }

    bool FITKTmshAdaptorElements::readSetData(QList<int> elemIdList)
    {
        if (!m_setSurfManager) return false;
        QString name = m_setSurfManager->checkName("TmshSet-1");
        Interface::FITKModelSet* setData = new Interface::FITKModelSet(Interface::FITKModelEnum::FITKModelSetType::FMSElem);
        setData->setDataObjectName(name);
        setData->setAbsoluteMember(elemIdList);
        m_setSurfManager->appendDataObj(setData);
        return true;
    }

    int FITKTmshAdaptorElements::getElementType(int type, int & nodeNum, QList<int>& nodeIndexs)
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


