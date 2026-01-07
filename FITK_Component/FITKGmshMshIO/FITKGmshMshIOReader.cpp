#include "FITKGmshMshIOReader.h"
#include "FITKGmshAdaptorObject.h"
#include "FITKGmshMshIOReaderTool.h"
#include "FITK_Kernel/FITKAppFramework/FITKMessage.h"
#include "FITK_Interface/FITKInterfaceModel/FITKUnstructuredMesh.h"

namespace Gmsh
{
    void FITKGmshMshIOReader::setComponentManager(Interface::FITKComponentManager* componentManager)
    {
        _componentMgr = componentManager;
    }

    void FITKGmshMshIOReader::run()
    {
        *_resultMark = false;
        this->sendCurrentPercent(1);
        bool ok = this->loadFile();
        if (!ok || !this->_data) return;
        _tool = new FITKGmshMshIOReaderTool(dynamic_cast<Interface::FITKUnstructuredMesh*>(_data), _componentMgr);
        bool readOK = this->read();
        if (_resultMark != nullptr)
            *_resultMark = readOK;
        
        _tool->update(_isFilterElement);
        delete _tool;
        _tool = nullptr;
        //读取完成
        this->sendCurrentPercent(100);
    }

    void FITKGmshMshIOReader::setReadFilterDim(QList<int> dim)
    {
        _filterDim = dim;
    }

    QList<int> FITKGmshMshIOReader::getReadFilterDim()
    {
        return _filterDim;
    }

    void FITKGmshMshIOReader::setFilterLowDimElement(bool isFilter)
    {
        _isFilterElement = isFilter;
    }

    FITKGmshMshIOReaderTool * FITKGmshMshIOReader::getReaderTool()
    {
        return _tool;
    }

    void FITKGmshMshIOReader::consoleMessage(int level, const QString& message)
    {
        switch (level)
        {
        case 1: AppFrame::FITKMessageNormal(message);
            break;
        case 2: AppFrame::FITKMessageWarning(message);
            break;
        case 3: AppFrame::FITKMessageError(message);
            break;
        default:
            break;
        }
    }
    bool FITKGmshMshIOReader::read()
    {
        //读取数据
        auto adaptor = FITKIOADAPTORFACTORY->createT<FITKGmshAdaptorObject>("Gmsh", "FITKGmshAdaptorObject");
        if (adaptor == nullptr) return false;
        adaptor->setComponentManager(_componentMgr);
        adaptor->setFileReader(this);
        adaptor->setDataObject(this->_data);
        bool readOK = adaptor->adaptR();
        delete adaptor;
        return readOK;
    }
}


