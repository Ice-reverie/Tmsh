#include "FITKMeshGenerateProcessorGmshExec.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKComponents.h"
#include "FITK_Kernel/FITKAppFramework/FITKComponentInterface.h"

#include "FITK_Interface/FITKInterfaceModel/FITKComponentManager.h"
#include "FITK_Interface/FITKInterfaceModel/FITKUnstructuredMesh.h"
#include "FITK_Interface/FITKInterfaceModel/FITKElementFactory.h"
#include "FITK_Interface/FITKInterfaceModel/FITKAbstractElement.h"
#include "FITK_Interface/FITKInterfaceModel/FITKModelEnum.h"
#include "FITK_Component/FITKGmshMshIO/FITKGmshMshIOInterface.h"

#include <QDir>

namespace Gmsh
{
    FITKMeshGenerateProcessorGmshExec::FITKMeshGenerateProcessorGmshExec()
    {

    }

    FITKMeshGenerateProcessorGmshExec::~FITKMeshGenerateProcessorGmshExec()
    {

    }

    void FITKMeshGenerateProcessorGmshExec::start(QStringList info)
    {
        Q_UNUSED(info);
        //获取数据
        QString file = this->getValueT<QString>("File");
        QList<QVariant> dim = this->getValueT<QList<QVariant>>("FilterDim");
        bool isFilterLowDimElement = this->getValueT<bool>("LowDimElement");
        Interface::FITKUnstructuredMesh * mesh = dynamic_cast<Interface::FITKUnstructuredMesh *>(_dataObject.value("Mesh"));
        Interface::FITKComponentManager * comp = dynamic_cast<Interface::FITKComponentManager *>(_dataObject.value("ComponentManager"));
        //获取读取接口
        AppFrame::FITKCmponents* componentMge = FITKAPP->getComponents();
        if (!componentMge) return;
        Gmsh::FITKGmshMshIOInterface* fitkAbaIO = dynamic_cast<Gmsh::FITKGmshMshIOInterface*>(componentMge->getComponentByName("GmshMshIO"));
        if (!fitkAbaIO) return;
        //设置参数
        QList<int> filterDim;
        for (QVariant d : dim)
        {
            bool ok = false;
            int v = d.toInt(&ok);
            if (!ok) continue;
            filterDim.append(v);
        }
        fitkAbaIO->setFileName(file);
        fitkAbaIO->setDataObject("ComponentManager", comp);
        fitkAbaIO->setDataObject("MeshData", mesh);
        fitkAbaIO->setReadFilterDim(filterDim);
        fitkAbaIO->setFilterLowDimElement(isFilterLowDimElement);
        //不在线程中运行并执行
        fitkAbaIO->runInThread(false);
        fitkAbaIO->exec(1);
    }

}
