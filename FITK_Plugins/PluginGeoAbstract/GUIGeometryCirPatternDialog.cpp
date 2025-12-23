#include "GUIGeometryCirPatternDialog.h"
#include "ui_GUIGeometryCirPatternDialog.h"
#include "GraphData/GraphDataPick/GraphDataPickInfo.h"
#include "GraphData/GraphDataPick/GraphPickData.h"
#include "GraphData/GraphDataPick/GraphPickProvider.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKGeoInterfaceFactory.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeomTools.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoTransformation.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKGeoCommandList.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "ModelData/GeometryManager.h"
#include "FITK_Kernel/FITKCore/FITKActionOperator.h"
#include <QMessageBox>

#define _RECTANGULAR_IS_ 0
#define _RECTANGULAR_IL_ 1
#define _RECTANGULAR_SL_ 2

namespace GUI {
    GUIGeometryCirPatternDialog::GUIGeometryCirPatternDialog(Core::FITKActionOperator* oper, QWidget* parent)
        :Core::FITKDialog(parent), _oper(oper)
    {
        _ui = new Ui::GUIGeometryCirPatternDialog();
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
            connect(pickProvider, &Graph::GraphPickProvider::sig_pickedOver, this, &GUIGeometryCirPatternDialog::pickFinishedOper);
        }
        this->init();
    }

    GUIGeometryCirPatternDialog::~GUIGeometryCirPatternDialog()
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

    void GUIGeometryCirPatternDialog::init()
    {
        setWindowTitle(tr("Circular Pattern"));
        _ui->comboBox_Target->addItem(tr("Curve"), int(Graph::PickObjType::PickGeoCurve));
        _ui->comboBox_Target->addItem(tr("Surface"), int(Graph::PickObjType::PickGeoFace));
        _ui->comboBox_Target->addItem(tr("Solid"), int(Graph::PickObjType::PickGeoSolid));
        _ui->tabWidget->setTabText(0, "Axial Reference");
        _ui->tabWidget->setTabText(1, "Crown Definition");
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
        _obj = geoFactory->createCommand(Interface::FITKGeoEnum::FITKGeometryComType::FGTCircularPattern);
        if (!_obj) return;
        //获取几何管理器
        _geometryData = FITKAPP->getGlobalData()->getGeometryData<ModelData::GeometryManager>();
        if (_geometryData == nullptr) return;
        QString name = _geometryData->checkName(tr("Pattern-1"));
        _ui->lineEdit_Name->setText(name);
    }

    QString GUIGeometryCirPatternDialog::getNameText()
    {
        return _ui->lineEdit_Name->text();
    }

    void GUIGeometryCirPatternDialog::pickFinishedOper()
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
        else if (_pickFlag == PickFlag::AxisDirection)
        {
            _virtualAxis = virtualShape.at(0);
            _virtualAxis.Type = Interface::FITKGeoEnum::VTopoShapeType::VSEdge;
            _ui->label_Num_2->setText(_virtualAxis.isNull() ? tr("Picked (0)") : tr("Picked (1)"));
        }
    }

    void GUIGeometryCirPatternDialog::createCirPatternData()
    {
        int firstInstances = 0;
        double firstSpacing = 0;
        double firstLength = 0;
        int secondInstances = 0;
        double secondSpacing = 0;
        double secondLength = 0;
        Interface::FITKAbsGeoModelCircularPattern *cirPatternOper = dynamic_cast<Interface::FITKAbsGeoModelCircularPattern *>(_obj);
        if (cirPatternOper == nullptr) return;
        cirPatternOper->setSourceShape(_virtualTarget);
        cirPatternOper->setAxis(_virtualAxis);
        if (_ui->comboBox_First_Parameter->currentData().toInt() == _RECTANGULAR_IS_)
        {
            cirPatternOper->setFirstDefinition(Interface::FITKAbsGeoModelPattern::Definition::InstanceAndSpacing);
            firstInstances = _ui->spinBox_First_Instance->text().toInt();
            firstSpacing = _ui->lineEdit_First_Spacing->text().toDouble();
            firstLength = firstInstances * firstSpacing;
        }
        else if (_ui->comboBox_First_Parameter->currentData().toInt() == _RECTANGULAR_IL_)
        {
            cirPatternOper->setFirstDefinition(Interface::FITKAbsGeoModelPattern::Definition::InstanceAndTotal);
            firstInstances = _ui->spinBox_First_Instance->text().toInt();
            firstLength = _ui->lineEdit_First_Length->text().toDouble();
            firstSpacing = firstLength / firstInstances;
        }
        else if (_ui->comboBox_First_Parameter->currentData().toInt() == _RECTANGULAR_SL_)
        {
            cirPatternOper->setFirstDefinition(Interface::FITKAbsGeoModelPattern::Definition::SpacingAndTotal);
            firstSpacing = _ui->lineEdit_First_Spacing->text().toDouble();
            firstLength = _ui->lineEdit_First_Length->text().toDouble();
            firstInstances = (firstLength / firstSpacing) / 1;
        }
        if (_ui->comboBox_Second_Parameter->currentData().toInt() == _RECTANGULAR_IS_)
        {
            cirPatternOper->setSecondDefinition(Interface::FITKAbsGeoModelPattern::Definition::InstanceAndSpacing);
            secondInstances = _ui->spinBox_Second_Instance->text().toInt();
            secondSpacing = _ui->lineEdit_Second_Spacing->text().toDouble();
            secondLength = secondInstances * secondSpacing;
        }
        else if (_ui->comboBox_Second_Parameter->currentData().toInt() == _RECTANGULAR_IL_)
        {
            cirPatternOper->setSecondDefinition(Interface::FITKAbsGeoModelPattern::Definition::InstanceAndTotal);
            secondInstances = _ui->spinBox_Second_Instance->text().toInt();
            secondLength = _ui->lineEdit_Second_Length->text().toDouble();
            secondSpacing = secondLength / secondInstances;
        }
        else if (_ui->comboBox_Second_Parameter->currentData().toInt() == _RECTANGULAR_SL_)
        {
            cirPatternOper->setSecondDefinition(Interface::FITKAbsGeoModelPattern::Definition::SpacingAndTotal);
            secondLength = _ui->lineEdit_Second_Length->text().toDouble();
            secondSpacing = _ui->lineEdit_Second_Spacing->text().toDouble();
            secondInstances = (secondLength / secondSpacing) / 1;
        }
        cirPatternOper->setAxisCount(firstInstances);
        cirPatternOper->setAxisSpacing(firstSpacing);
        cirPatternOper->setAxisTotal(firstLength);
        cirPatternOper->setCrownCount(secondInstances);
        cirPatternOper->setCrownSpacing(secondSpacing);
        cirPatternOper->setCrownTotal(secondLength);
        _objID = cirPatternOper->getDataObjectID();
    }

    void GUIGeometryCirPatternDialog::on_pushButton_OK_clicked()
    {
        //if (!this->checkData()) return;
        if (!_geometryData || !_obj) return ;
        //获取数据名称
        QString name = this->getNameText();
        if (_geometryData->getDataByName(name)) {
            QMessageBox::warning(this, "", tr("\"%1\" already exists and cannot be overwritten.").arg(name), QMessageBox::Ok);
            return ;
        }
        //创建几何数据
        _obj->setDataObjectName(name);
        this->createCirPatternData();
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

    void GUIGeometryCirPatternDialog::on_pushButton_Cancel_clicked()
    {
        this->reject();
    }

    void GUIGeometryCirPatternDialog::on_Target_pick_clicked()
    {
        //设置拾取方式
        _pickFlag = PickFlag::Target;
        enablePickSig(Graph::PickObjType(_ui->comboBox_Target->currentData().toInt()), Graph::PickMethod::PMSingle);
    }

    void GUIGeometryCirPatternDialog::on_Direction_pick_clicked()
    {
        //设置拾取方式
        _pickFlag = PickFlag::AxisDirection;
        enablePickSig(Graph::PickObjType::PickGeoCurve, Graph::PickMethod::PMSingle);
    }

    void GUIGeometryCirPatternDialog::on_comboBox_First_Parameter_currentIndexChanged(int index)
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
    void GUIGeometryCirPatternDialog::on_comboBox_Second_Parameter_currentIndexChanged(int index)
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
    void GUIGeometryCirPatternDialog::enablePickSig(Graph::PickObjType objType, Graph::PickMethod methodType)
    {
        Graph::GraphDataPickInfoStru pickInfo = Graph::GraphDataPickInfo::GetPickInfo();
        pickInfo._pickObjType = Graph::PickObjType(objType);
        pickInfo._pickMethod = Graph::PickMethod(methodType);
        //设置拾取方式
        Graph::GraphDataPickInfo::SetPickInfo(pickInfo);
    }

    bool GUIGeometryCirPatternDialog::checkData()
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