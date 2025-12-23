#include "ui_WorkingDirDlg.h"
#include "WorkingDirDlg.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppSettings.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QDir>

namespace GUI
{
    WorkingDirDlg::WorkingDirDlg(QWidget* p)
        :Core::FITKDialog(p)
    {
        //禁用关闭按钮
        this->setWindowFlags((windowFlags() & ~Qt::WindowCloseButtonHint));
        
        _ui = new Ui::WorkingDirDlg;
        _ui->setupUi(this);
        //初始化
        this->init();
        //设置默认按钮
        _ui->OKButton->setDefault(true);
    }
     
    WorkingDirDlg::~WorkingDirDlg()
    {
        if (_ui) delete _ui;
    }

    void WorkingDirDlg::on_BrowseButton_clicked()
    {
        const QString path = _ui->lineEdit->text();
        //浏览目录
        const QString d = QFileDialog::getExistingDirectory(this, tr("Working Dir"), path);
        if (d.isEmpty()) return;
        //设置到页面显示
        _ui->lineEdit->setText(d);
    }

    void WorkingDirDlg::on_OKButton_clicked()
    {
        //路径检查
        const QString text = _ui->lineEdit->text();
        const QDir dir(text);
        //路径不存在
        if (!dir.exists())
        {
            QMessageBox::critical(this, tr("Error"), QString(tr("%1 is not exist!")).arg(text));
            return;
        }
        //存储选择的路径
        AppFrame::FITKAppSettings* setting = FITKAPP->getAppSettings();
        if (setting == nullptr) return;
        setting->setWorkingDir(text);
        this->accept();
    }

    void WorkingDirDlg::on_QuitButton_clicked()
    {
        this->reject();
    }

    void WorkingDirDlg::init()
    {
        //获取工作路径
        AppFrame::FITKAppSettings* setting = FITKAPP->getAppSettings();
        if (setting == nullptr) return;
        const QString wk = setting->getWorkingDir();
        //显示到界面
        _ui->lineEdit->setText(wk);
    }

}



