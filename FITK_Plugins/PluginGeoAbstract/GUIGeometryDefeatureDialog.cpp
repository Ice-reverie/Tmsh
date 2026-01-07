#include "GUIGeometryDefeatureDialog.h"
#include "ui_GUIGeometryDefeatureDialog.h"
#include "GraphData/GraphDataPick/GraphDataPickInfo.h"
#include "GraphData/GraphDataPick/GraphPickData.h"
#include "GraphData/GraphDataPick/GraphPickProvider.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKGeoInterfaceFactory.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeomTools.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoModelOperSolid.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKGeoCommandList.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "ModelData/GeometryManager.h"
#include "FITK_Kernel/FITKCore/FITKActionOperator.h"
#include <QMessageBox>
namespace GUI {
    GUIGeometryDefeatureDialog::GUIGeometryDefeatureDialog(Core::FITKActionOperator* oper, QWidget* parent)
        :Core::FITKDialog(parent), _oper(oper)
    {
        _ui = new Ui::GUIGeometryDefeatureDialog();
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
            connect(pickProvider, &Graph::GraphPickProvider::sig_pickedData, this, &GUIGeometryDefeatureDialog::pickFinishedOper);
        }
        this->init();
    }

    GUIGeometryDefeatureDialog::~GUIGeometryDefeatureDialog()
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

    void GUIGeometryDefeatureDialog::init()
    {
        setWindowTitle(tr("Defeature"));
        //创建几何数据
        Interface::FITKInterfaceGeometryFactory* geoFactory = Interface::FITKInterfaceGeometryFactory::getInstance();
        if (!geoFactory) return;
        _obj = geoFactory->createCommand(Interface::FITKGeoEnum::FITKGeometryComType::FGTDefeature);
        if (!_obj) return;
        //获取几何管理器
        _geometryData = FITKAPP->getGlobalData()->getGeometryData<ModelData::GeometryManager>();
         
        if (_geometryData == nullptr) return;
        QString name = _geometryData->checkName(tr("Defeature-1"));
        _ui->lineEdit_Name->setText(name);
    }

    QString GUIGeometryDefeatureDialog::getNameText()
    {
        return _ui->lineEdit_Name->text();
    }

    void GUIGeometryDefeatureDialog::pickFinishedOper()
    {
        _faceVirtualTopos.clear();
        //获取拾取数据
        Graph::GraphPickProvider* provider = Graph::GraphPickProvider::getInstance();
        if (provider == nullptr)return;
        QList<Graph::GraphPickData*> pickList = provider->getPickedList();
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
                    _faceVirtualTopos.push_back(temp);
                }
            }
        }
        if (_faceVirtualTopos.isEmpty()) return;
        _ui->label_Num->setText(QString("Picked (%1)").arg(QString::number(_faceVirtualTopos.size())));
    }

    void GUIGeometryDefeatureDialog::createDefeatureData()
    {
        Interface::FITKAbsGeoModelDefeatureSolid* defeatureModel = dynamic_cast<Interface::FITKAbsGeoModelDefeatureSolid*>(_obj);
        if (defeatureModel == nullptr) return;
        //设置数据
        defeatureModel->setFaceTopos(_faceVirtualTopos);
        defeatureModel->setInputCmdId(_faceVirtualTopos.at(0).CmdId);
        defeatureModel->addReferenceCmdID(_faceVirtualTopos.at(0).CmdId);
        _objID = defeatureModel->getDataObjectID();
    }

    void GUIGeometryDefeatureDialog::on_pushButton_OK_clicked()
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
        this->createDefeatureData();
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

    void GUIGeometryDefeatureDialog::on_pushButton_Cancel_clicked()
    {
        this->reject();
    }

    void GUIGeometryDefeatureDialog::on_face_pick_clicked()
    {
        //设置拾取方式
        enablePickSig(Graph::PickObjType::PickGeoFace, Graph::PickMethod::PMIndividually);
    }

    void GUIGeometryDefeatureDialog::enablePickSig(Graph::PickObjType objType, Graph::PickMethod methodType)
    {
        Graph::GraphDataPickInfoStru pickInfo = Graph::GraphDataPickInfo::GetPickInfo();
        pickInfo._pickObjType = Graph::PickObjType(objType);
        pickInfo._pickMethod = Graph::PickMethod(methodType);
        //设置拾取方式
        Graph::GraphDataPickInfo::SetPickInfo(pickInfo);
    }
}