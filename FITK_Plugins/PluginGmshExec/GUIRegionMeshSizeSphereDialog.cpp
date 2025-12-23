#include "GUIRegionMeshSizeSphereDialog.h"
#include "ui_GUIRegionMeshSizeSphereDialog.h"
#include "GraphData/GraphDataPick/GraphDataPickInfo.h"
#include "GraphData/GraphDataPick/GraphPickData.h"
#include "GraphData/GraphDataPick/GraphPickProvider.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKGeoInterfaceFactory.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeomTools.h"
#include "FITK_Kernel/FITKCore/FITKDataRepo.h"
#include "FITK_Kernel/FITKCore/FITKActionOperator.h"
#include "FITK_Interface/FITKInterfaceMeshGen/FITKMeshGenInterface.h"
#include "FITK_Interface/FITKInterfaceMeshGen/FITKAbstractMeshSizeInfoGenerator.h"
#include "FITK_Interface/FITKInterfaceMeshGen/FITKRegionMeshSizeSphere.h"
namespace GUI {
    GUIRegionMeshSizeSphereDialog::GUIRegionMeshSizeSphereDialog(Core::FITKActionOperator* oper, QWidget* parent)
        :Core::FITKDialog(parent), _oper(oper)
    {
        _ui = new Ui::GUIRegionMeshSizeSphereDialog();
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
            connect(pickProvider, &Graph::GraphPickProvider::sig_pickedOver, this, &GUIRegionMeshSizeSphereDialog::pickFinishedOper);
        }
        this->init();
    }

    GUIRegionMeshSizeSphereDialog::~GUIRegionMeshSizeSphereDialog()
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

    void GUIRegionMeshSizeSphereDialog::init()
    { 
        setWindowTitle(tr("RegionMeshSizeSphere"));

    }

    void GUIRegionMeshSizeSphereDialog::pickFinishedOper()
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

    void GUIRegionMeshSizeSphereDialog::on_pushButton_OK_clicked()
    {
        //获取网格尺寸生成器
        Interface::FITKAbstractMeshSizeInfoGenerator* meshSizeInfoGenerator = Interface::FITKMeshGenInterface::getInstance()->getMeshSizeGenerator("GmshExec");
        if (meshSizeInfoGenerator == nullptr) return;
        //生成局部网格
        Interface::FITKRegionMeshSizeSphere* regionMeshSize = dynamic_cast<Interface::FITKRegionMeshSizeSphere*>(meshSizeInfoGenerator->createRegionMeshSize(Interface::FITKAbstractRegionMeshSize::RegionType::RegionSphere));
        if (regionMeshSize == nullptr) return;
        double point[3] = { _ui->lineEdit_X->text().toDouble(), _ui->lineEdit_Y->text().toDouble(), _ui->lineEdit_Z->text().toDouble() };
        regionMeshSize->setLocation(point);
        regionMeshSize->setRadius(_ui->lineEdit_Radius->text().toDouble());
        regionMeshSize->setInternalSize(_ui->lineEdit_InternalSize->text().toDouble());
        regionMeshSize->setExternalSize(_ui->lineEdit_ExternalSize->text().toDouble());
        regionMeshSize->setThickness(_ui->lineEdit_Thickness->text().toDouble());

        Interface::FITKRegionMeshSizeManager* regionMeshSizeManager = Interface::FITKMeshGenInterface::getInstance()->getRegionMeshSizeMgr("GmshExec");
        if (regionMeshSizeManager == nullptr) return;
        regionMeshSizeManager->appendDataObj(regionMeshSize);
        this->accept();
    }

    void GUIRegionMeshSizeSphereDialog::on_pushButton_Cancel_clicked()
    {
        this->reject();
    }

    void GUIRegionMeshSizeSphereDialog::on_point1_pick_clicked()
    {
        //设置拾取方式
        enablePickSig(Graph::PickObjType::PickGeoVertex, Graph::PickMethod::PMSingle);
    }

    void GUIRegionMeshSizeSphereDialog::enablePickSig(Graph::PickObjType objType, Graph::PickMethod methodType)
    {
        Graph::GraphDataPickInfoStru pickInfo = Graph::GraphDataPickInfo::GetPickInfo();
        pickInfo._pickObjType = Graph::PickObjType(objType);
        pickInfo._pickMethod = Graph::PickMethod(methodType);
        //设置拾取方式
        Graph::GraphDataPickInfo::SetPickInfo(pickInfo);
    }
}