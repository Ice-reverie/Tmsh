#include "GUIGeometryRecPatternDialog.h"
#include "ui_GUIGeometryRecPatternDialog.h"
#include "GraphData/GraphDataPick/GraphDataPickInfo.h"
#include "GraphData/GraphDataPick/GraphPickData.h"
#include "GraphData/GraphDataPick/GraphPickProvider.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKGeoInterfaceFactory.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeomTools.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoTransformation.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKGeoCommandList.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsVirtualTopo.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Kernel/FITKCore/FITKDataRepo.h"
#include "ModelData/GeometryManager.h"
#include "FITK_Kernel/FITKCore/FITKActionOperator.h"
#include <QMessageBox>

#define _RECTANGULAR_IS_ 0
#define _RECTANGULAR_IL_ 1
#define _RECTANGULAR_SL_ 2

namespace GUI {
    GUIGeometryRecPatternDialog::GUIGeometryRecPatternDialog(Core::FITKActionOperator* oper, QWidget* parent)
        :Core::FITKDialog(parent), _oper(oper)
    {
        _ui = new Ui::GUIGeometryRecPatternDialog();
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
            connect(pickProvider, &Graph::GraphPickProvider::sig_pickedOver, this, &GUIGeometryRecPatternDialog::pickFinishedOper);
        }
        this->init();
    }

    GUIGeometryRecPatternDialog::~GUIGeometryRecPatternDialog()
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

    void GUIGeometryRecPatternDialog::init()
    {
        setWindowTitle(tr("Rectangular Pattern"));
        _ui->comboBox_Target->addItem(tr("Curve"), int(Graph::PickObjType::PickGeoCurve));
        _ui->comboBox_Target->addItem(tr("Surface"), int(Graph::PickObjType::PickGeoFace));
        _ui->comboBox_Target->addItem(tr("Solid"), int(Graph::PickObjType::PickGeoSolid));
        _ui->tabWidget->setTabText(0, "First Direction");
        _ui->tabWidget->setTabText(1, "Second Direction");
        _ui->comboBox_First_Parameter->addItem(tr("Instances & Spacing"), _RECTANGULAR_IS_);
        _ui->comboBox_First_Parameter->addItem(tr("Instances & Length"), _RECTANGULAR_IL_);
        _ui->comboBox_First_Parameter->addItem(tr("Spacing & Length"), _RECTANGULAR_SL_);

        _ui->comboBox_Second_Parameter->addItem(tr("Instances & Spacing"), _RECTANGULAR_IS_);
        _ui->comboBox_Second_Parameter->addItem(tr("Instances & Length"), _RECTANGULAR_IL_);
        _ui->comboBox_Second_Parameter->addItem(tr("Spacing & Length"), _RECTANGULAR_SL_);

        _ui->spinBox_First_Instance->setMinimum(2);
        _ui->spinBox_Second_Instance->setMinimum(1);
        Interface::FITKInterfaceGeometryFactory* geoFactory = Interface::FITKInterfaceGeometryFactory::getInstance();
        if (!geoFactory) return;
        _obj = geoFactory->createCommand(Interface::FITKGeoEnum::FITKGeometryComType::FGTRectangularPattern);
        if (!_obj) return;
        //获取几何管理器
        _geometryData = FITKAPP->getGlobalData()->getGeometryData<ModelData::GeometryManager>();
        if (_geometryData == nullptr) return;
        QString name = _geometryData->checkName(tr("Pattern-1"));
        _ui->lineEdit_Name->setText(name);
    }

    QString GUIGeometryRecPatternDialog::getNameText()
    {
        return _ui->lineEdit_Name->text();
    }

    void GUIGeometryRecPatternDialog::pickFinishedOper()
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
            if(Graph::PickObjType(_ui->comboBox_Target->currentData().toInt()) == Graph::PickObjType::PickGeoCurve)
                _virtualTarget.Type = Interface::FITKGeoEnum::VTopoShapeType::VSEdge;
            else if (Graph::PickObjType(_ui->comboBox_Target->currentData().toInt()) == Graph::PickObjType::PickGeoFace)
                _virtualTarget.Type = Interface::FITKGeoEnum::VTopoShapeType::VSFace;
            else if (Graph::PickObjType(_ui->comboBox_Target->currentData().toInt()) == Graph::PickObjType::PickGeoSolid)
                _virtualTarget.Type = Interface::FITKGeoEnum::VTopoShapeType::VSSolid;
            _ui->label_Num->setText(_virtualTarget.isNull() ? tr("Picked (0)") : tr("Picked (1)"));
        }
        else if (_pickFlag == PickFlag::FirstDirection)
        {
            std::array<double, 3> xyz = this->getPointXYZ(virtualShape[0].VirtualTopoId);
            _ui->lineEdit_First_X->setText(QString::number(xyz.at(0)));
            _ui->lineEdit_First_Y->setText(QString::number(xyz.at(1)));
            _ui->lineEdit_First_Z->setText(QString::number(xyz.at(2)));
        }
        else if (_pickFlag == PickFlag::SecondDirection)
        {
            std::array<double, 3> xyz = this->getPointXYZ(virtualShape[0].VirtualTopoId);
            _ui->lineEdit_Second_X->setText(QString::number(xyz.at(0)));
            _ui->lineEdit_Second_Y->setText(QString::number(xyz.at(1)));
            _ui->lineEdit_Second_Z->setText(QString::number(xyz.at(2)));
        }
    }

    void GUIGeometryRecPatternDialog::createRecPatternData()
    {
        int firstInstances = 0;
        double firstSpacing = 0;
        double firstLength = 0;
        int secondInstances = 0;
        double secondSpacing = 0;
        double secondLength = 0;
        Interface::FITKAbsGeoModelRectangularPattern *recPatternOper = dynamic_cast<Interface::FITKAbsGeoModelRectangularPattern *>(_obj);
        if (recPatternOper == nullptr) return;
        recPatternOper->setSourceShape(_virtualTarget);
        //设置第一方向
        double x = _ui->lineEdit_First_X->text().toDouble();
        double y = _ui->lineEdit_First_Y->text().toDouble();
        double z = _ui->lineEdit_First_Z->text().toDouble();
        recPatternOper->setFirstDirection(x, y, z);
        //设置第二方向
        x = _ui->lineEdit_Second_X->text().toDouble();
        y = _ui->lineEdit_Second_Y->text().toDouble();
        z = _ui->lineEdit_Second_Z->text().toDouble();
        recPatternOper->setSecondDirection(x, y, z);
        if (_ui->comboBox_First_Parameter->currentData().toInt() == _RECTANGULAR_IS_)
        {
            recPatternOper->setFirstDefinition(Interface::FITKAbsGeoModelPattern::Definition::InstanceAndSpacing);
            firstInstances = _ui->spinBox_First_Instance->text().toInt();
            firstSpacing = _ui->lineEdit_First_Spacing->text().toDouble();
            firstLength = firstInstances * firstSpacing;
        }
        else if (_ui->comboBox_First_Parameter->currentData().toInt() == _RECTANGULAR_IL_)
        {
            recPatternOper->setFirstDefinition(Interface::FITKAbsGeoModelPattern::Definition::InstanceAndTotal);
            firstInstances = _ui->spinBox_First_Instance->text().toInt();
            firstLength = _ui->lineEdit_First_Length->text().toDouble();
            firstSpacing = firstLength / firstInstances;
        }
        else if (_ui->comboBox_First_Parameter->currentData().toInt() == _RECTANGULAR_SL_)
        {
            recPatternOper->setFirstDefinition(Interface::FITKAbsGeoModelPattern::Definition::SpacingAndTotal);
            firstSpacing = _ui->lineEdit_First_Spacing->text().toDouble();
            firstLength = _ui->lineEdit_First_Length->text().toDouble();
            firstInstances = (firstLength / firstSpacing) / 1;
        }
        if (_ui->comboBox_Second_Parameter->currentData().toInt() == _RECTANGULAR_IS_)
        {
            recPatternOper->setSecondDefinition(Interface::FITKAbsGeoModelPattern::Definition::InstanceAndSpacing);
            secondInstances = _ui->spinBox_Second_Instance->text().toInt();
            secondSpacing = _ui->lineEdit_Second_Spacing->text().toDouble();
            secondLength = secondInstances * secondSpacing;
        }
        else if (_ui->comboBox_Second_Parameter->currentData().toInt() == _RECTANGULAR_IL_)
        {
            recPatternOper->setSecondDefinition(Interface::FITKAbsGeoModelPattern::Definition::InstanceAndTotal);
            secondInstances = _ui->spinBox_Second_Instance->text().toInt();
            secondLength = _ui->lineEdit_Second_Length->text().toDouble();
            secondSpacing = secondLength / secondInstances;
        }
        else if (_ui->comboBox_Second_Parameter->currentData().toInt() == _RECTANGULAR_SL_)
        {
            recPatternOper->setSecondDefinition(Interface::FITKAbsGeoModelPattern::Definition::SpacingAndTotal);
            secondLength = _ui->lineEdit_Second_Length->text().toDouble();
            secondSpacing = _ui->lineEdit_Second_Spacing->text().toDouble();
            secondInstances = (secondLength / secondSpacing) / 1;
        }
        recPatternOper->setFirstCount(firstInstances);
        recPatternOper->setFirstSpacing(firstSpacing);
        recPatternOper->setFirstTotal(firstLength);
        recPatternOper->setSecondCount(secondInstances);
        recPatternOper->setSecondSpacing(secondSpacing);
        recPatternOper->setSecondTotal(secondLength);
        _objID = recPatternOper->getDataObjectID();
    }

    void GUIGeometryRecPatternDialog::on_pushButton_OK_clicked()
    {
        if (!this->checkData()) return;
        if (!_geometryData || !_obj) return ;
        //获取数据名称
        QString name = this->getNameText();
        if (_geometryData->getDataByName(name)) {
            QMessageBox::warning(this, "", tr("\"%1\" already exists and cannot be overwritten.").arg(name), QMessageBox::Ok);
            return ;
        }
        //创建几何数据
        _obj->setDataObjectName(name);
        this->createRecPatternData();
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

    void GUIGeometryRecPatternDialog::on_pushButton_Cancel_clicked()
    {
        this->reject();
    }

    void GUIGeometryRecPatternDialog::on_Target_pick_clicked()
    {
        //设置拾取方式
        _pickFlag = PickFlag::Target;
        enablePickSig(Graph::PickObjType(_ui->comboBox_Target->currentData().toInt()), Graph::PickMethod::PMSingle);
    }

    void GUIGeometryRecPatternDialog::on_First_Direction_pick_clicked()
    {
        //设置拾取方式
        _pickFlag = PickFlag::FirstDirection;
        enablePickSig(Graph::PickObjType::PickGeoVertex, Graph::PickMethod::PMSingle);
    }
    void GUIGeometryRecPatternDialog::on_Second_Direction_pick_clicked()
    {
        //设置拾取方式
        _pickFlag = PickFlag::SecondDirection;
        enablePickSig(Graph::PickObjType::PickGeoVertex, Graph::PickMethod::PMSingle);
    }
    void GUIGeometryRecPatternDialog::on_comboBox_First_Parameter_currentIndexChanged(int index)
    {
        if (index == _RECTANGULAR_IS_)
        {
            _ui->spinBox_First_Instance->setEnabled(true);
            _ui->lineEdit_First_Spacing->setEnabled(true);
            _ui->lineEdit_First_Length->setEnabled(false);
        }
        else if (index == _RECTANGULAR_IL_)
        {
            _ui->spinBox_First_Instance->setEnabled(true);
            _ui->lineEdit_First_Spacing->setEnabled(false);
            _ui->lineEdit_First_Length->setEnabled(true);
        }
        else if (index == _RECTANGULAR_SL_)
        {
            _ui->spinBox_First_Instance->setEnabled(false);
            _ui->lineEdit_First_Spacing->setEnabled(true);
            _ui->lineEdit_First_Length->setEnabled(true);
        }
    }
    void GUIGeometryRecPatternDialog::on_comboBox_Second_Parameter_currentIndexChanged(int index)
    {
        if (index == _RECTANGULAR_IS_)
        {
            _ui->spinBox_Second_Instance->setEnabled(true);
            _ui->lineEdit_Second_Spacing->setEnabled(true);
            _ui->lineEdit_Second_Length->setEnabled(false);
        }
        else if (index == _RECTANGULAR_IL_)
        {
            _ui->spinBox_Second_Instance->setEnabled(true);
            _ui->lineEdit_Second_Spacing->setEnabled(false);
            _ui->lineEdit_Second_Length->setEnabled(true);
        }
        else if (index == _RECTANGULAR_SL_)
        {
            _ui->spinBox_Second_Instance->setEnabled(false);
            _ui->lineEdit_Second_Spacing->setEnabled(true);
            _ui->lineEdit_Second_Length->setEnabled(true);
        }
    }
    void GUIGeometryRecPatternDialog::enablePickSig(Graph::PickObjType objType, Graph::PickMethod methodType)
    {
        Graph::GraphDataPickInfoStru pickInfo = Graph::GraphDataPickInfo::GetPickInfo();
        pickInfo._pickObjType = Graph::PickObjType(objType);
        pickInfo._pickMethod = Graph::PickMethod(methodType);
        //设置拾取方式
        Graph::GraphDataPickInfo::SetPickInfo(pickInfo);
    }
    std::array<double, 3> GUIGeometryRecPatternDialog::getPointXYZ(int id)
    {
        std::array<double, 3> xyz = { -1,-1,-1 };

        Interface::FITKInterfaceGeometryFactory*factory = Interface::FITKInterfaceGeometryFactory::getInstance();
        if (factory == nullptr)
        {
            return xyz;
        }
        Interface::FITKAbstractGeomToolsCreator*createor = factory->getGeomToolsCreator();
        if (!createor)return xyz;
        Interface::FITKAbstractGeomPointTool *pointTool = createor->createPointTool();
        double PointArray[3];
        //pointTool->getXYZ(virtualShape[0].VirtualTopoId, PointArray);
        pointTool->getXYZ(FITKDATAREPO->getTDataByID<Interface::FITKAbsVirtualTopo>(id), PointArray);
        xyz.at(0) = PointArray[0];
        xyz.at(1) = PointArray[1];
        xyz.at(2) = PointArray[2];
        delete pointTool;
        return xyz;
    }
    bool GUIGeometryRecPatternDialog::checkData()
    {
        if (_ui->comboBox_First_Parameter->currentData().toInt() == _RECTANGULAR_IS_ && _ui->lineEdit_First_Spacing->text().toDouble() <= 0)
        {
            QMessageBox::warning(this, tr("warn"), tr("Please set the correct parameters"));
            return false;
        }
        if (_ui->comboBox_First_Parameter->currentData().toInt() == _RECTANGULAR_IL_ && _ui->lineEdit_First_Length->text().toDouble() <= 0)
        {
            QMessageBox::warning(this, tr("warn"), tr("Please set the correct parameters"));
            return false;
        }
        if (_ui->comboBox_First_Parameter->currentData().toInt() == _RECTANGULAR_SL_ && (_ui->lineEdit_First_Length->text().toDouble() / _ui->lineEdit_First_Spacing->text().toDouble()) < 2)
        {
            QMessageBox::warning(this, tr("warn"), tr("Please set the correct parameters"));
            return false;
        }
        if (_ui->comboBox_Second_Parameter->currentData().toInt() == _RECTANGULAR_IS_ && _ui->lineEdit_Second_Spacing->text().toDouble() <= 0)
        {
            QMessageBox::warning(this, tr("warn"), tr("Please set the correct parameters"));
            return false;
        }
        if (_ui->comboBox_Second_Parameter->currentData().toInt() == _RECTANGULAR_IL_ && _ui->lineEdit_Second_Length->text().toDouble() <= 0)
        {
            QMessageBox::warning(this, tr("warn"), tr("Please set the correct parameters"));
            return false;
        }
        if (_ui->comboBox_Second_Parameter->currentData().toInt() == _RECTANGULAR_SL_ && (_ui->lineEdit_Second_Length->text().toDouble() / _ui->lineEdit_Second_Spacing->text().toDouble()) < 2)
        {
            QMessageBox::warning(this, tr("warn"), tr("Please set the correct parameters"));
            return false;
        }
        return true;
    }
}