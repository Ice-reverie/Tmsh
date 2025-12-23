#include "GUIGmshGlobalSize.h"
#include "ui_GUIGmshGlobalSize.h"
#include "FITK_Kernel/FITKCore/FITKActionOperator.h"
#include "FITK_Interface/FITKInterfaceMeshGen/FITKMeshGenInterface.h"
#include "FITK_Interface/FITKInterfaceMeshGen/FITKGlobalMeshSizeInfo.h"

#define PROCESSNUM 5
namespace GUI {
    GUIGmshGlobalSize::GUIGmshGlobalSize(Core::FITKActionOperator* oper, QWidget* parent)
        :Core::FITKDialog(parent), _oper(oper)
    {
        _ui = new Ui::GUIGmshGlobalSize();
        _ui->setupUi(this);
        // 设置对话框关闭时自动删除
        setAttribute(Qt::WA_DeleteOnClose);
        //去掉问号
        Qt::WindowFlags flags = windowFlags();
        flags &= ~Qt::WindowContextHelpButtonHint;
        setWindowFlags(flags);
        this->init();
    }

    GUIGmshGlobalSize::~GUIGmshGlobalSize()
    {
        if (_ui) {
            delete _ui;
            _ui = nullptr;
        }
    }

    void GUIGmshGlobalSize::init()
    {
        //填充网格划分信息
        Interface::FITKGlobalMeshSizeInfo* meshSize = Interface::FITKMeshGenInterface::getInstance()->getGlobalMeshSizeInfo("GmshExec");
        if (meshSize == nullptr) {
            return;
        }
        //设置最小值
        _ui->lineEdit_Min->setText(QString::number(meshSize->getMinSize()));
        //设置最大值
        _ui->lineEdit_Max->setText(QString::number(meshSize->getMaxSize()));
    }

    void GUIGmshGlobalSize::on_pushButton_OK_clicked()
    {
        if (_oper == nullptr) {
            return;
        }
        //填充网格划分信息
        Interface::FITKGlobalMeshSizeInfo* meshSize = Interface::FITKMeshGenInterface::getInstance()->getGlobalMeshSizeInfo("GmshExec");
 
        //设置网格划分尺寸
        meshSize->setMaxSize(_ui->lineEdit_Max->text().toDouble());
        meshSize->setMinSize(_ui->lineEdit_Min->text().toDouble());
        this->accept();
    }

    void GUIGmshGlobalSize::on_pushButton_Cancel_clicked()
    {
        this->reject();
    }
}