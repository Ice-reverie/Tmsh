#include "GUIGeometryLineSplitterDialog.h"
#include "ui_GUIGeometryLineSplitterDialog.h"
#include "GraphData/GraphDataPick/GraphDataPickInfo.h"
#include "GraphData/GraphDataPick/GraphPickData.h"
#include "GraphData/GraphDataPick/GraphPickProvider.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKGeoInterfaceFactory.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeomTools.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoSplitter.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKGeoCommandList.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsVirtualTopo.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Kernel/FITKCore/FITKDataRepo.h"
#include "ModelData/GeometryManager.h"
#include "FITK_Kernel/FITKCore/FITKActionOperator.h"
#include <QMessageBox>
namespace GUI {
    GUIGeometryLineSplitterDialog::GUIGeometryLineSplitterDialog(Core::FITKActionOperator* oper, QWidget* parent)
        :Core::FITKDialog(parent), _oper(oper)
    {
        _ui = new Ui::GUIGeometryLineSplitterDialog();
        _ui->setupUi(this);
        // 设置对话框关闭时自动删除
        setAttribute(Qt::WA_DeleteOnClose);
        //去掉问号
        Qt::WindowFlags flags = windowFlags();
        flags &= ~Qt::WindowContextHelpButtonHint;
        setWindowFlags(flags);
        //获取拾取器，绑定拾取结束信号
        Graph::GraphPickProvider* pickProvider = Graph::GraphPickProvider::getInstance();
        //清除拾取对象所有的连接
        if (pickProvider != nullptr)
        {
            connect(pickProvider, &Graph::GraphPickProvider::sig_pickedOver, this, &GUIGeometryLineSplitterDialog::pickFinishedOper);
        }
        this->init();
    }

    GUIGeometryLineSplitterDialog::~GUIGeometryLineSplitterDialog()
    {
        if (_ui) {
            delete _ui;
            _ui = nullptr;
        }
        //所有的类在执行完毕后，将清空对3维的操作
        Graph::GraphPickProvider* provider = Graph::GraphPickProvider::getInstance();
        if (provider == nullptr)return;
        provider->clearPickedData();//清空拾取数据
        //拾取对象 关闭
        //拾取方式 空
        Graph::GraphDataPickInfoStru pickInfo = Graph::GraphDataPickInfo::GetPickInfo();
        pickInfo._pickObjType = Graph::PickObjType::PickNone;
        pickInfo._pickMethod = Graph::PickMethod::PMNone;
        //设置拾取方式
        Graph::GraphDataPickInfo::SetPickInfo(pickInfo);
    }

    void GUIGeometryLineSplitterDialog::init()
    {
        setWindowTitle(tr("Line Splitter"));

        Interface::FITKInterfaceGeometryFactory* geoFactory = Interface::FITKInterfaceGeometryFactory::getInstance();
        if (!geoFactory) return;
        _obj = geoFactory->createCommand(Interface::FITKGeoEnum::FITKGeometryComType::FGTCurveSplitter);
        if (!_obj) return;
        //获取几何管理器
        _geometryData = FITKAPP->getGlobalData()->getGeometryData<ModelData::GeometryManager>();
        if (_geometryData == nullptr) return;
        QString name = _geometryData->checkName(tr("CurveSplitter-1"));
        _ui->lineEdit_Name->setText(name);
    }

    QString GUIGeometryLineSplitterDialog::getNameText()
    {
        return _ui->lineEdit_Name->text();
    }

    void GUIGeometryLineSplitterDialog::pickFinishedOper()
    {
        //获取拾取数据
        Graph::GraphPickProvider* provider = Graph::GraphPickProvider::getInstance();
        if (provider == nullptr)return;
        QList<Graph::GraphPickData*> pickList = provider->getPickedList();
        QList<Interface::VirtualShape> virtualShape;
        for (Graph::GraphPickData* pickData : pickList) {
            if (pickData == nullptr)continue;
            int cmdID = pickData->getPickedDataObjId();
            QHash<int, int>OtherIDs = pickData->getPickedOtherIDs();//key PickedId  value index
            for (int i : pickData->getPickedIDs())
            {
                if (OtherIDs.contains(i) == true)
                {
                    //添加拾取数据
                    Interface::VirtualShape temp(cmdID, i, OtherIDs.value(i));
                    virtualShape.push_back(temp);
                }
            }
        }
        if (virtualShape.isEmpty()) return;
        if (_pickFlag == PickFlag::Target)
        {
            _virtualTarget = virtualShape.at(0);
            _virtualTarget.Type = Interface::FITKGeoEnum::VTopoShapeType::VSFace;
            _ui->label_Num->setText(_virtualTarget.isNull() ? tr("Picked (0)") : tr("Picked (1)"));
        }
        else if (_pickFlag == PickFlag::Tool)
        {
            _virtualTool = virtualShape.at(0);
            //获取坐标
            Interface::FITKInterfaceGeometryFactory*factory = Interface::FITKInterfaceGeometryFactory::getInstance();
            if (factory == nullptr) return;
            Interface::FITKAbstractGeomToolsCreator*createor = factory->getGeomToolsCreator();
            if (!createor) return;
            Interface::FITKAbstractGeomPointTool *pointTool = createor->createPointTool();
            if (!pointTool) return;
            double PointArray[3];
            //pointTool->getXYZ(virtualShape[0].VirtualTopoId, PointArray);
            pointTool->getXYZ(FITKDATAREPO->getTDataByID<Interface::FITKAbsVirtualTopo>(virtualShape[0].VirtualTopoId), PointArray);
            _ui->lineEdit_X->setText(QString::number(PointArray[0]));
            _ui->lineEdit_Y->setText(QString::number(PointArray[1]));
            _ui->lineEdit_Z->setText(QString::number(PointArray[2]));
        }
    }

    void GUIGeometryLineSplitterDialog::createLineSplitterData()
    {
        Interface::FITKAbsGeoCurveSplitter *lineSplitterOper = dynamic_cast<Interface::FITKAbsGeoCurveSplitter *>(_obj);
        if (lineSplitterOper == nullptr) return;
        lineSplitterOper->setSourceShape(_virtualTarget);
        std::array<double, 3> point = { _ui->lineEdit_X->text().toDouble(),
            _ui->lineEdit_Y->text().toDouble(),
            _ui->lineEdit_Z->text().toDouble() };
        lineSplitterOper->setToolPoint(point);
        lineSplitterOper->addReferenceCmdID(_virtualTarget.CmdId);
        _objID = lineSplitterOper->getDataObjectID();
    }

    void GUIGeometryLineSplitterDialog::on_pushButton_OK_clicked()
    {
        if (!_geometryData || !_obj) return ;
        //获取数据名称
        QString name = this->getNameText();
        if (_geometryData->getDataByName(name)) {
            QMessageBox::warning(this, "", tr("\"%1\" already exists and cannot be overwritten.").arg(name), QMessageBox::Ok);
            return ;
        }
        //创建几何数据
        _obj->setDataObjectName(name);
        this->createLineSplitterData();
        if (_obj->update() == false)
        {
            //创建失败不将该数据append
            QMessageBox::warning(this, "", tr("The geometry could not be created properly").arg(name), QMessageBox::Ok);
            return ;
        }
        _geometryData->appendDataObj(_obj);
        //更新数据
        _oper->setArgs("objID", _objID);
        this->accept();
    }

    void GUIGeometryLineSplitterDialog::on_pushButton_Cancel_clicked()
    {
        this->reject();
    }

    void GUIGeometryLineSplitterDialog::on_Target_pick_clicked()
    {
        //设置拾取方式
        _pickFlag = PickFlag::Target;
        enablePickSig(Graph::PickObjType::PickGeoCurve, Graph::PickMethod::PMSingle);
    }

    void GUIGeometryLineSplitterDialog::on_Tool_pick_clicked()
    {
        //设置拾取方式
        _pickFlag = PickFlag::Tool;
        enablePickSig(Graph::PickObjType::PickGeoVertex, Graph::PickMethod::PMSingle);
    }

    void GUIGeometryLineSplitterDialog::enablePickSig(Graph::PickObjType objType, Graph::PickMethod methodType)
    {
        Graph::GraphDataPickInfoStru pickInfo = Graph::GraphDataPickInfo::GetPickInfo();
        pickInfo._pickObjType = Graph::PickObjType(objType);
        pickInfo._pickMethod = Graph::PickMethod(methodType);
        //设置拾取方式
        Graph::GraphDataPickInfo::SetPickInfo(pickInfo);
    }
}