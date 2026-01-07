#include "FITKMeshGenerateProcessorTmshExec.h"

#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKComponents.h"

#include "FITK_Interface/FITKInterfaceModel/FITKComponentManager.h"
#include "FITK_Interface/FITKInterfaceModel/FITKUnstructuredMesh.h"
#include "FITK_Component/FITKGmshMshIO/FITKGmshMshIOInterface.h"

namespace Tmsh
{
    FITKMeshGenerateProcessorTmshExec::FITKMeshGenerateProcessorTmshExec()
    {
    }

    FITKMeshGenerateProcessorTmshExec::~FITKMeshGenerateProcessorTmshExec()
    {
    }

    void FITKMeshGenerateProcessorTmshExec::start(QStringList info)
    {
//        Q_UNUSED(info);

//        QString file = this->getValueT<QString>("File");
//        QList<QVariant> dim = this->getValueT<QList<QVariant>>("FilterDim");
//        bool isFilterLowDimElement = this->getValueT<bool>("LowDimElement");
//        Interface::FITKUnstructuredMesh* mesh = dynamic_cast<Interface::FITKUnstructuredMesh*>(_dataObject.value("Mesh"));
//        Interface::FITKComponentManager* comp = dynamic_cast<Interface::FITKComponentManager*>(_dataObject.value("ComponentManager"));

//        AppFrame::FITKCmponents* componentMge = FITKAPP->getComponents();
//        if (!componentMge) return;

//        Gmsh::FITKGmshMshIOInterface* gmshMshIO = dynamic_cast<Gmsh::FITKGmshMshIOInterface*>(componentMge->getComponentByName("GmshMshIO"));
//        if (!gmshMshIO) return;

//        QList<int> filterDim;
//        for (const QVariant& d : dim)
//        {
//            bool ok = false;
//            int v = d.toInt(&ok);
//            if (!ok) continue;
//            filterDim.append(v);
//        }

//        gmshMshIO->setFileName(file);
//        gmshMshIO->setDataObject("ComponentManager", comp);
//        gmshMshIO->setDataObject("MeshData", mesh);
//        gmshMshIO->setReadFilterDim(filterDim);
//        gmshMshIO->setFilterLowDimElement(isFilterLowDimElement);
//        gmshMshIO->runInThread(false);
//        gmshMshIO->exec(1);
    }
}

