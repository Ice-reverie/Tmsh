#include "FITKGmshAdaptorObject.h"
#include "FITKGmshAdaptorNodes.h"
#include "FITKGmshAdaptorElements.h"
#include "FITK_Interface/FITKInterfaceIO/FITKAbstractTextReader.h"
#include "FITK_Interface/FITKInterfaceIO/FITKAbstractTextWriter.h"
//#include "FITK_Interface/FITKInterfaceStructural/FITKAbaMeshPartVTK.h"

namespace Gmsh
{
    QString FITKGmshAdaptorObject::getAdaptorClass()
    {
        return "FITKGmshAdaptorObject";
    }

    bool FITKGmshAdaptorObject::adaptR()
    {
        if (!_dataObj) return false;
        bool readOK = true;
        //开始读取
        while (!_reader->atEnd())
        {
            QString line = _reader->readLine();
            //获取关键字
            QString keywords = line.trimmed().toLower();
            if (!keywords.contains("$"))continue;
            keywords.remove("$");
            //发送计算进度信号
            _reader->sendCurrentPercent();

            //获取版本信息
            if (keywords == "meshformat")
                readOK &= this->readVersionInfo();
            //解析节点
            else if (keywords == "nodes")
                readOK &= this->readNodes();
            //解析单元
            else if (keywords == "elements")
                readOK &= this->readElements();
            //其余关键字
            else
                continue;
            
            if (readOK)
            {
                //调用解析函数后要执行 backLine函数。
                _reader->backLine();
            }
            else
            {
                //解析出错目前不做处理
                //return false;
            }
        }
        return readOK;
    }

    bool FITKGmshAdaptorObject::adaptW()
    {
        return true;
    }

    void FITKGmshAdaptorObject::setComponentManager(Interface::FITKComponentManager* componentManager)
    {
        m_componentMgr = componentManager;
    }

    bool FITKGmshAdaptorObject::readVersionInfo()
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

    bool FITKGmshAdaptorObject::readNodes()
    {
        if (!_dataObj) return false;
        auto adaptor = FITKIOADAPTORFACTORY->createT<FITKGmshAdaptorNodes>("Gmsh", "FITKGmshAdaptorNodes");
        if (adaptor == nullptr)
        {
            _reader->consoleMessage(3, "Gmsh Nodes Read Error.");
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

    bool FITKGmshAdaptorObject::readElements()
    {
        if (!_dataObj) return false;
        auto adaptor = FITKIOADAPTORFACTORY->createT<FITKGmshAdaptorElements>("Gmsh", "FITKGmshAdaptorElements");
        if (adaptor == nullptr)
        {
            _reader->consoleMessage(3, "Gmsh Elements Read Error.");
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
}


