#include "FITKTmshAdaptorNodes.h"
#include "FITKTmshMshIOReaderTool.h"
#include "FITKTmshMshIOReader.h"
#include "FITK_Interface/FITKInterfaceIO/FITKAbstractTextReader.h"
#include "FITK_Interface/FITKInterfaceIO/FITKAbstractTextWriter.h"
#include "FITK_Interface/FITKInterfaceModel/FITKUnstructuredMesh.h"

namespace Tmsh
{
    QString FITKTmshAdaptorNodes::getAdaptorClass()
    {
        return "FITKTmshAdaptorNodes";
    }

	bool FITKTmshAdaptorNodes::adaptR()
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
			if (line.contains("Triangles"))
			{
				break;
			}
			//维度 维度一致的第几个 是否有参数坐标 节点数
			QStringList info = line.split(' ', QString::SkipEmptyParts);
			//if (info.size() != 3/*4*/)continue;
			if (info.size() < 3)continue;
			bool readOK = this->readNode(info);
			if (!readOK)
				return false;
			count++;
		}

		return true;
	}

	bool FITKTmshAdaptorNodes::adaptRT()
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
			if (line.contains("Edges"))break;
			//维度 维度一致的第几个 是否有参数坐标 节点数
			QStringList info = line.split(' ', QString::SkipEmptyParts);
			if (info.size() < 3)continue;
			bool readOK = this->readNode(info);
			if (!readOK)
				return false;
			count++;
		}

		return true;
	}

    bool FITKTmshAdaptorNodes::adaptW()
    {
        return true;
    }

    bool FITKTmshAdaptorNodes::readNodesInfo()
    {
        QString line = _reader->nextLine().trimmed();
        if (line.split(' ', QString::SkipEmptyParts).size() != 4) return false;
        //实体块 节点数 节点最小标号 节点最大标号
        QString nodesInfo = _reader->readLine().trimmed();
        QStringList info = nodesInfo.split(' ', QString::SkipEmptyParts);
        if (info.size() != 4) return false;
        bool ok = false;
        //实体块
        int entityNum = info.at(0).toInt(&ok);
        if (!ok) return false;
        //节点数
        int nodesNum = info.at(1).toInt(&ok);
        if (!ok) return false;
        //节点最小标号
        int nodeMinID = info.at(2).toInt(&ok);
        if (!ok) return false;
        //节点最大标号
        int nodeMaxID = info.at(3).toInt(&ok);
        if (!ok) return false;
        return true;
    }

    bool FITKTmshAdaptorNodes::readNode(QStringList info)
    {
		Interface::FITKUnstructuredMesh* meshObj = dynamic_cast<Interface::FITKUnstructuredMesh*>(_dataObj);
		FITKTmshMshIOReader* reader = dynamic_cast<FITKTmshMshIOReader*>(_reader);
		//if (!meshObj || !reader || info.size() != 3/*4*/) return false;
		if (!meshObj || !reader || info.size() < 3) return false;
		FITKTmshMshIOReaderTool* tool = reader->getReaderTool();
		if (!tool) return false;
		bool ok = false;
		//读取节点数据
		//节点编号
		int id = count;
		double x = info.at(0).toDouble(&ok);
		if (!ok) return false;
		double y = info.at(1).toDouble(&ok);
		if (!ok) return false;
		double z = info.at(2).toDouble(&ok);
		if (!ok) return false;
		tool->addNode(id, x, y, z);
		return true;
    }
}


