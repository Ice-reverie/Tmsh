#include "OperatorImportGeometry.h"

#include "OperatorsInterface/GraphEventOperator.h"
#include "GraphData/GraphDataPick/GraphPickProvider.h"
#include "GraphData/GraphDataPick/GraphPickData.h"

#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppSettings.h"
#include "FITK_Kernel/FITKCore/FITKThreadPool.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKGeoCommandList.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKGeoInterfaceFactory.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoModelImport.h"

#include "GUIFrame/MainWindow.h"
#include "GUIFrame/ControlPanel.h"
#include "GUIWidget/GUIEnumType.h"
#include "ModelData/GeometryManager.h"
#include <QFileDialog>

namespace ModelOper
{
    OperatorsImportGeometry::OperatorsImportGeometry()
    {

    }

    OperatorsImportGeometry::~OperatorsImportGeometry()
    {

    }

    bool OperatorsImportGeometry::execGUI()
    {
        //if (_emitter->objectName() == "actionWorkingDir") {
        //    Graph::GraphPickProvider* provider = Graph::GraphPickProvider::getInstance();
        //    if (provider == nullptr)return false;

        //    Graph::GraphDataPickInfoStru pickStr;
        //    pickStr._pickObjType = Graph::PickObjType::PickGeoFace;
        //    pickStr._pickMethod = Graph::PickMethod::PMIndividually;
        //    Graph::GraphDataPickInfo::ClearPickInfo();
        //    Graph::GraphDataPickInfo::SetPickInfo(pickStr);
        //    return false;
        //}

        //主界面
        QWidget* mw = FITKAPP->getGlobalData()->getMainWindow();
        //获取工作目录
        QString wk;
        auto settings = FITKAPP->getAppSettings();
        if (settings)
            wk = settings->getWorkingDir();
        QFileDialog dlg(mw, QObject::tr("Import Model File"), wk, "Geometry File(*.brep ; *.stp ; *.step ; *.igs ; *.iges)");
        dlg.setAcceptMode(QFileDialog::AcceptOpen);  //打开模式
        dlg.show();
        //阻塞，否则键盘事件处理会出问题
        bool accept = false;
        QEventLoop loop;
        connect(&dlg, &QFileDialog::accepted, [&] {loop.quit(); accept = true;  });
        connect(&dlg, &QFileDialog::rejected, [&] {loop.quit(); accept = false;  });
        loop.exec();
        if (!accept) return false;
        QStringList files = dlg.selectedFiles(); //选择的文件名称
        if (files.isEmpty()) return false;
        QString fileName = files.at(0);

        if (fileName.isEmpty())
            return false;
        //存储信息
        //if (_emitter == nullptr) return false;
        //存储
        this->setArgs("FileName", fileName);
        return true;
    }

    bool OperatorsImportGeometry::execProfession()
    {
        //获取线程池
        Core::FITKThreadPool* pool = Core::FITKThreadPool::getInstance();
        if (pool == nullptr)return false;
        QString fileName;
        this->argValue<QString>("FileName", fileName);
        if (fileName.isEmpty()) return false;

        ImportReadThread* importThread = new ImportReadThread();
        importThread->_fileName = fileName;
        connect(importThread, SIGNAL(sigImportFinish(bool, int)), this, SLOT(slotImportFinish(bool, int)));
        pool->execTask(importThread);
        //       this->clearArgs();
        return true;
    }

    void OperatorsImportGeometry::slotImportFinish(bool result, int objID)
    {
        if (result == false)return;
        if (objID < 0)return;


        EventOper::GraphEventOperator* graphOper = FITKOPERREPO->getOperatorT<EventOper::GraphEventOperator>("GraphPreprocess");
        if (graphOper) {
            graphOper->updateGraph(objID);
        }

        //刷新树
        GUI::MainWindow* mw = GUI::MainWindow::GetMainWindowFromFramework();
        if (mw == nullptr) return;
        GUI::ControlPanel* cp = mw->getControlPanel();
        if (cp == nullptr) return;
        GUI::TreeWidgetPage* tw = cp->getTreeWidget(GUI::TreeWidgetType::PageModel);
        if (tw == nullptr) return;
        tw->updatePage();
    }

    void ImportReadThread::run()
    {
        Interface::FITKGeoCommandList*  geometryData = FITKAPP->getGlobalData()->
            getGeometryData<ModelData::GeometryManager>();
        if (geometryData == nullptr) return;

        Interface::FITKInterfaceGeometryFactory* geoFactory = Interface::FITKInterfaceGeometryFactory::getInstance();
        if (geoFactory == nullptr)return;
        auto geoObj = geoFactory->createCommandT<Interface::FITKAbsGeoModelImport>(Interface::FITKGeoEnum::FITKGeometryComType::FGTImport);
        if (geoObj == nullptr)return;
        geoObj->setFileName(_fileName);


        if (geoObj->getDataObjectName().isEmpty())
        {
            QFileInfo fileInfo(_fileName);
            // 获取文件名称（不包含路径与文件类型）
            QString name = fileInfo.baseName();
            geoObj->setDataObjectName(geometryData->checkName(name));
        }
        bool result = geoObj->update();
        int objID = geoObj->getDataObjectID();

        if (result)//读取成功
        {
            geometryData->appendDataObj(geoObj);
            emit sigImportFinish(result, objID);
        }
        else //读取失败
        {
            if (geoObj != nullptr) delete geoObj;
        }

    }
}

