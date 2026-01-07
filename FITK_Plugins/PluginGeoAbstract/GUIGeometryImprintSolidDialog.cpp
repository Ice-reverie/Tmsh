#include "GUIGeometryImprintSolidDialog.h"
#include "ui_GUIGeometryImprintSolidDialog.h"
#include "GraphData/GraphDataPick/GraphDataPickInfo.h"
#include "GraphData/GraphDataPick/GraphPickData.h"
#include "GraphData/GraphDataPick/GraphPickProvider.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKGeoInterfaceFactory.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeomTools.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoModelOperImprintSolid.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKGeoCommandList.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "ModelData/GeometryManager.h"
#include "FITK_Kernel/FITKCore/FITKActionOperator.h"
#include <QMessageBox>
namespace GUI {
    GUIGeometryImprintSolidDialog::GUIGeometryImprintSolidDialog(Core::FITKActionOperator* oper, QWidget* parent)
        :Core::FITKDialog(parent), _oper(oper)
    {
        _ui = new Ui::GUIGeometryImprintSolidDialog();
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
            connect(pickProvider, &Graph::GraphPickProvider::sig_pickedData, this, &GUIGeometryImprintSolidDialog::pickFinishedOper);
        }
        this->init();
    }

    GUIGeometryImprintSolidDialog::~GUIGeometryImprintSolidDialog()
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

    void GUIGeometryImprintSolidDialog::init()
    {
        setWindowTitle(tr("Imprint Solid"));
        //创建几何数据
        Interface::FITKInterfaceGeometryFactory* geoFactory = Interface::FITKInterfaceGeometryFactory::getInstance();
        if (!geoFactory) return;
        _obj = geoFactory->createCommand(Interface::FITKGeoEnum::FITKGeometryComType::FGTImprintSolid);
        if (!_obj) return;
        //获取几何管理器
        _geometryData = FITKAPP->getGlobalData()->getGeometryData<ModelData::GeometryManager>();
         
        if (_geometryData == nullptr) return;
        QString name = _geometryData->checkName(tr("Imprint-1"));
        _ui->lineEdit_Name->setText(name);
    }

    QString GUIGeometryImprintSolidDialog::getNameText()
    {
        return _ui->lineEdit_Name->text();
    }

    void GUIGeometryImprintSolidDialog::pickFinishedOper()
    {
        _solidVirtualTopos.clear();
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
                    _solidVirtualTopos.push_back(temp);
                }
            }
        }
        if (_solidVirtualTopos.isEmpty()) return;
        _ui->label_Num->setText(QString("Picked (%1)").arg(QString::number(_solidVirtualTopos.size())));
    }

    void GUIGeometryImprintSolidDialog::createImprintSolidData()
    {
        Interface::FITKAbsGeoModelOperImprintSolid* imprintSolidModel = dynamic_cast<Interface::FITKAbsGeoModelOperImprintSolid*>(_obj);
        if (imprintSolidModel == nullptr) return;
        //设置数据
        imprintSolidModel->set(_solidVirtualTopos);
        imprintSolidModel->setType(Interface::FITKAbsGeoModelOperImprintSolid::GBTImprintSolid);
        for (Interface::VirtualShape addShape : _solidVirtualTopos)
        {
            imprintSolidModel->addReferenceCmdID(addShape.CmdId);
        }
        _objID = imprintSolidModel->getDataObjectID();
    }

    void GUIGeometryImprintSolidDialog::on_pushButton_OK_clicked()
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
        this->createImprintSolidData();
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

    void GUIGeometryImprintSolidDialog::on_pushButton_Cancel_clicked()
    {
        this->reject();
    }

    void GUIGeometryImprintSolidDialog::on_solid_pick_clicked()
    {
        //设置拾取方式
        enablePickSig(Graph::PickObjType::PickGeoSolid, Graph::PickMethod::PMIndividually);
    }

    void GUIGeometryImprintSolidDialog::enablePickSig(Graph::PickObjType objType, Graph::PickMethod methodType)
    {
        Graph::GraphDataPickInfoStru pickInfo = Graph::GraphDataPickInfo::GetPickInfo();
        pickInfo._pickObjType = Graph::PickObjType(objType);
        pickInfo._pickMethod = Graph::PickMethod(methodType);
        //设置拾取方式
        Graph::GraphDataPickInfo::SetPickInfo(pickInfo);
    }
}