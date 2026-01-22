#include "FITKTmshMshIOReader.h"
#include "FITKTmshAdaptorObject.h"
#include "FITKTmshMshIOReaderTool.h"
#include "FITK_Kernel/FITKAppFramework/FITKMessage.h"
#include "FITK_Interface/FITKInterfaceModel/FITKUnstructuredMesh.h"

#include "FITK_Interface/FITKInterfaceMeshGen/FITKMeshGenInterface.h"
#include "FITK_Interface/FITKInterfaceMeshGen/FITKAbstractMesherDriver.h"

namespace Tmsh
{
    void FITKTmshMshIOReader::setComponentManager(Interface::FITKComponentManager* componentManager)
    {
        _componentMgr = componentManager;
    }

    void FITKTmshMshIOReader::run()
    {
        *_resultMark = false;
        this->sendCurrentPercent(1);
        bool ok = this->loadFile();
        if (!ok || !this->_data) return;
        _tool = new FITKTmshMshIOReaderTool(dynamic_cast<Interface::FITKUnstructuredMesh*>(_data), _componentMgr);

		Interface::FITKMeshGenInterface* mf = Interface::FITKMeshGenInterface::getInstance();
		if (!mf) return;
		Interface::FITKAbstractMesherDriver* mesher = mf->getMesherDriver("TmshExec");
		int type = mesher->getValueT<int>("Method");
        bool readOK = this->read(type);
        if (_resultMark != nullptr)
            *_resultMark = readOK;
        
        _tool->update(_isFilterElement);
        delete _tool;
        _tool = nullptr;
        //读取完成
        this->sendCurrentPercent(100);
    }

    void FITKTmshMshIOReader::setReadFilterDim(QList<int> dim)
    {
        _filterDim = dim;
    }

    QList<int> FITKTmshMshIOReader::getReadFilterDim()
    {
        return _filterDim;
    }

    void FITKTmshMshIOReader::setFilterLowDimElement(bool isFilter)
    {
        _isFilterElement = isFilter;
    }

    FITKTmshMshIOReaderTool * FITKTmshMshIOReader::getReaderTool()
    {
        return _tool;
    }

    void FITKTmshMshIOReader::consoleMessage(int level, const QString& message)
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
    bool FITKTmshMshIOReader::read(const int type)
    {
        //读取数据
        auto adaptor = FITKIOADAPTORFACTORY->createT<FITKTmshAdaptorObject>("Tmsh", "FITKTmshAdaptorObject");
        if (adaptor == nullptr) return false;
        adaptor->setComponentManager(_componentMgr);
        adaptor->setFileReader(this);
        adaptor->setDataObject(this->_data);
		bool readOK = false;
		if (type == 1)
		{
			readOK = adaptor->adaptR();
		}
		else if (type == 2)
		{
			readOK = adaptor->adaptRT();
		}
        delete adaptor;
        return readOK;
    }
}


