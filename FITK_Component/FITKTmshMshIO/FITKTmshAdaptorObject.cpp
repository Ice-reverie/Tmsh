#include "FITKTmshAdaptorObject.h"
#include "FITKTmshAdaptorNodes.h"
#include "FITKTmshAdaptorElements.h"
#include "FITK_Interface/FITKInterfaceIO/FITKAbstractTextReader.h"
#include "FITK_Interface/FITKInterfaceIO/FITKAbstractTextWriter.h"
//#include "FITK_Interface/FITKInterfaceStructural/FITKAbaMeshPartVTK.h"

namespace Tmsh
{
    QString FITKTmshAdaptorObject::getAdaptorClass()
    {
        return "FITKTmshAdaptorObject";
    }

	bool FITKTmshAdaptorObject::adaptR()
	{
		if (!_dataObj) return false;
		bool readOK = true;
		//开始读取
		while (!_reader->atEnd())
		{
			QString line = _reader->readLine().trimmed();
			// 跳过空行
			if (line.isEmpty())
				continue;
			//发送计算进度信号
			_reader->sendCurrentPercent();
			//解析节点
			if (line == "Vertices")
			{
				readOK &= this->readNodes();
				line = _reader->previousLine();
			}
			//解析面
			if (line == "Triangles")
			{
				readOK &= this->readElements();
			}
			if (!readOK)
			{
				return false;
			}
		}
		return readOK;
	}

	bool FITKTmshAdaptorObject::adaptRT()
	{
		if (!_dataObj) return false;
		bool readOK = true;
		//开始读取
		while (!_reader->atEnd())
		{
			QString line = _reader->readLine().trimmed();
			// 跳过空行
			if (line.isEmpty())
				continue;
			//发送计算进度信号
			_reader->sendCurrentPercent();
			//解析节点
			if (line == "Vertices")
			{
				readOK &= this->readTNodes();
			}
			//解析单元
			else if (line == "Tetrahedra")
			{
				readOK &= this->readTElements();
			}
			if (!readOK)
			{
				return false;
			}
		}
		return readOK;
	}

    bool FITKTmshAdaptorObject::adaptW()
    {
        return true;
    }

    void FITKTmshAdaptorObject::setComponentManager(Interface::FITKComponentManager* componentManager)
    {
        m_componentMgr = componentManager;
    }

    bool FITKTmshAdaptorObject::readVersionInfo()
    {
        if (!_reader) return false;
        m_versionInfo = _reader->readLine();
        while (!_reader->atEnd())
        {
            QString line = _reader->readLine();
            QString keywords = line.trimmed().toLower();
            if (keywords.contains("$"))break;
        }
        return true;
    }

    bool FITKTmshAdaptorObject::readNodes()
    {
		if (!_dataObj) return false;
		auto adaptor = FITKIOADAPTORFACTORY->createT<FITKTmshAdaptorNodes>("Tmsh", "FITKTmshAdaptorNodes");
		if (adaptor == nullptr)
		{
			_reader->consoleMessage(3, "mesh Nodes Read Error.");
			return false;
		}
		//设置读取器
		adaptor->setFileReader(_reader);
		adaptor->setDataObject(_dataObj);
		bool ok = adaptor->adaptR();

		if (adaptor != nullptr)
			delete adaptor;
		return ok;
    }

	bool FITKTmshAdaptorObject::readTNodes()
	{
		if (!_dataObj) return false;
		auto adaptor = FITKIOADAPTORFACTORY->createT<FITKTmshAdaptorNodes>("Tmsh", "FITKTmshAdaptorNodes");
		if (adaptor == nullptr)
		{
			_reader->consoleMessage(3, "mesh Nodes Read Error.");
			return false;
		}
		//设置读取器
		adaptor->setFileReader(_reader);
		adaptor->setDataObject(_dataObj);
		bool ok = adaptor->adaptRT();

		if (adaptor != nullptr)
			delete adaptor;
		return ok;
	}

    bool FITKTmshAdaptorObject::readElements()
    {
		if (!_dataObj) return false;
		auto adaptor = FITKIOADAPTORFACTORY->createT<FITKTmshAdaptorElements>("Tmsh", "FITKTmshAdaptorElements");
		if (adaptor == nullptr)
		{
			_reader->consoleMessage(3, "mesh Elements Read Error.");
			return false;
		}
		//设置读取器
		adaptor->setFileReader(_reader);
		adaptor->setComponentManager(m_componentMgr);
		adaptor->setDataObject(_dataObj);
		bool ok = adaptor->adaptR();

		if (adaptor != nullptr)
			delete adaptor;
		return ok;
    }

	bool FITKTmshAdaptorObject::readTElements()
	{
		if (!_dataObj) return false;
		auto adaptor = FITKIOADAPTORFACTORY->createT<FITKTmshAdaptorElements>("Tmsh", "FITKTmshAdaptorElements");
		if (adaptor == nullptr)
		{
			_reader->consoleMessage(3, "mesh Elements Read Error.");
			return false;
		}
		//设置读取器
		adaptor->setFileReader(_reader);
		adaptor->setComponentManager(m_componentMgr);
		adaptor->setDataObject(_dataObj);
		bool ok = adaptor->adaptRT();

		if (adaptor != nullptr)
			delete adaptor;
		return ok;
	}
}


