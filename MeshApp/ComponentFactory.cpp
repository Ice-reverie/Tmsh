/**
 * @file ComponentFactory.cpp
 * @brief 组件工厂
 * @author YanZhiHui (chanyuantiandao@126.com)
 * @date 2024-04-19
 */
#include "ComponentFactory.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Component/FITKCompMessageWidget/FITKConsoleComponent.h"
#include "FITK_Component/FITKRenderWindowVTK/FITKGraph3DWindowInterface.h"
#include "FITK_Component/FITKGeoCompOCC/FITKGeoCompOCCInterface.h"
#include "FITK_Component/FITKCGNSIO/FITKCGNSIOInterface.h"
#include "OperatorsModel/OperatorModelInterface.h"
#include "PreWindowInitializer.h"
#include "OperatorsModel/OpersModelInterface.h"
#include "OperatorsGUI/OpersGUIInterface.h"
#include "HDF5IO/HDF5IOInterface.h"
#include "FITK_Component/FITKGmshMshIO/FITKGmshMshIOInterface.h"
//#include "FITK_Component/FITKAbaqusIOINP/FITKAbaqusIOINPInterface.h"

QList<AppFrame::FITKComponentInterface *> ComponentFactory::createComponents()
{
    // 自定义组件列表
    QList<AppFrame::FITKComponentInterface *> componentList;
    // 消息窗口组件
    componentList << new Comp::ConsoleComponent(FITKAPP->getGlobalData()->getMainWindow());
    // 3D图形窗口组件
    auto compVTKrender = new Comp::FITKGraph3DWindowInterface;
    Comp::FITKGraph3DWindowInterface::SetResolveCoincidentType(Comp::ResolveCoincidentType::RCT_PolygonOffset);
    componentList << compVTKrender;
    //occ 建模
 //   componentList << new OCC::FITKGeoCompOCCInterface;

    // VTK窗口初始化器
    compVTKrender->addInitializer(1, new PreWindowInitializer);

    //操作器
    componentList << new OperModel::OpersModelInterface;
    componentList << new GUIOper::OperGUIInterface;
    //工程文件IO
    componentList << new IO::HDF5IOInterface;
    componentList << new IO::FITKCGNSIOInterface;
    componentList << new Gmsh::FITKGmshMshIOInterface;
    //componentList << new IO::FITKAbaqusINPIO;


    return componentList;
}
