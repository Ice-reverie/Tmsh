#include "GUITetGenSettings.h"
#include "ui_GUITetGenSettings.h"

#include "FITK_Interface/FITKInterfaceMeshGen/FITKMeshGenInterface.h"
#include "FITK_Interface/FITKInterfaceMeshGen/FITKGlobalMeshSizeInfo.h"
#include "FITK_Interface/FITKInterfaceMeshGen/FITKGlobalMeshGenerateAlgorithmInfo.h"
#include "FITK_Interface/FITKInterfaceMeshGen/FITKAbstractMesherDriver.h"
#include "FITKTmshGlobalMeshGenerateAlgorithmInfo.h"

#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKSignalTransfer.h"
#include "FITK_Kernel/FITKCore/FITKDataRepo.h"
#include "FITK_Kernel/FITKAppFramework/FITKMessage.h"

#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoCommand.h"

#include "GUITetGenHelp.h"

#include <QMessageBox>
#include <QFileDialog>
#include <QApplication>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>
#include <QDebug>
#include <QDir>
#include <QProcess>
#include <QString>

namespace GUI
{
    namespace
    {
        constexpr const char* kMesherKey = "TmshExec";
    }

    GUITetGenSettings::GUITetGenSettings(QWidget* parent/* = nullptr*/) : Core::FITKDialog(parent), _process(new QProcess(this))
    {
        _ui = new Ui::GUITetGenSettings();
        _ui->setupUi(this);
        setAttribute(Qt::WA_DeleteOnClose);
        Qt::WindowFlags flags = windowFlags();
        flags &= ~Qt::WindowContextHelpButtonHint;
        setWindowFlags(flags);
        
        /*
        QString appRunDir = QDir::currentPath();
        QString tempPath = QDir(appRunDir).filePath("../Tmsh");
        QString defaultPath = QDir::cleanPath(tempPath);
        _ui->lineEdit_FilePath->setText(defaultPath);
        */

        connect(_process, &QProcess::readyReadStandardOutput, this, &GUITetGenSettings::onReadyReadOutput);
        connect(_process, &QProcess::readyReadStandardError, this, &GUITetGenSettings::onReadyReadError);

                connect(_process, static_cast<void (QProcess::*)(int, QProcess::ExitStatus)>(&QProcess::finished),
                this, [=](int exitCode, QProcess::ExitStatus exitStatus)
        {
            if (!_stdOutBuffer.isEmpty())
            {
                AppFrame::FITKMessageNormal(_stdOutBuffer.trimmed());
                _stdOutBuffer.clear();
            }
            if (!_stdErrBuffer.isEmpty())
            {
                AppFrame::FITKMessageWarning(_stdErrBuffer.trimmed());
                _stdErrBuffer.clear();
            }

            if(exitCode == 0 && exitStatus == QProcess::NormalExit)
            {
                QMessageBox::information(this, tr("Success"), tr("Mesh file processing is complete!"), QMessageBox::Ok);
                this->accept();
            }
            else
            {
                QMessageBox::critical(this, tr("Faliure"), tr("Failure in processing: %1").arg(_stdErrBuffer), QMessageBox::Ok);
            }
            _ui->pushButton_OK->setEnabled(true);
        });
        
        this->init();
    }

    GUITetGenSettings::GUITetGenSettings(Interface::FITKAbstractMesherDriver* driver, QWidget* parent/* = nullptr*/)
        : Core::FITKDialog(parent), _driver(driver), _process(new QProcess(this))
    {
        _ui = new Ui::GUITetGenSettings();
        _ui->setupUi(this);
        setAttribute(Qt::WA_DeleteOnClose);
        Qt::WindowFlags flags = windowFlags();
        flags &= ~Qt::WindowContextHelpButtonHint;
        setWindowFlags(flags);

        connect(_process, &QProcess::readyReadStandardOutput, this, &GUITetGenSettings::onReadyReadOutput);
        connect(_process, &QProcess::readyReadStandardError, this, &GUITetGenSettings::onReadyReadError);
        
        connect(_process, static_cast<void (QProcess::*)(int, QProcess::ExitStatus)>(&QProcess::finished),
                this, [=](int exitCode, QProcess::ExitStatus exitStatus)
        {
            if (!_stdOutBuffer.isEmpty())
            {
                AppFrame::FITKMessageNormal(_stdOutBuffer.trimmed());
                _stdOutBuffer.clear();
            }
            if (!_stdErrBuffer.isEmpty())
            {
                AppFrame::FITKMessageWarning(_stdErrBuffer.trimmed());
                _stdErrBuffer.clear();
            }

            if(exitCode == 0 && exitStatus == QProcess::NormalExit)
            {
                QMessageBox::information(this, tr("Success"), tr("Mesh file processing is complete!"), QMessageBox::Ok);
                this->accept();
            }
            else
            {
                QMessageBox::critical(this, tr("Faliure"), tr("Failure in processing: %1").arg(_stdErrBuffer), QMessageBox::Ok);
            }
            _ui->pushButton_OK->setEnabled(true);
            });

        this->init();
    }

    GUITetGenSettings::~GUITetGenSettings()
    {
        if (_ui)
        {
            delete _ui;
            _ui = nullptr;
        }
        if (_detailedDlg)
        {
            delete _detailedDlg;
            _detailedDlg = nullptr;
        }
        AppFrame::FITKSignalTransfer* signalTransfer = FITKAPP->getSignalTransfer();
        if (!signalTransfer) return;
        emit signalTransfer->setPickableObjTypeSig(-1);
    }


    void GUITetGenSettings::init()
    {
        setWindowTitle(tr("TetGen Settings"));

        Interface::FITKGlobalMeshSizeInfo* meshSizeInfo = Interface::FITKMeshGenInterface::getInstance()->getGlobalMeshSizeInfo(kMesherKey);
        Interface::FITKGlobalMeshGenerateAlgorithmInfo* meshGenerateAlgorithmInfo = Interface::FITKMeshGenInterface::getInstance()->getGlobalMeshGenerateAlgorithmInfo(kMesherKey);
        if (!meshSizeInfo || !meshGenerateAlgorithmInfo) return;

        // Tmsh 专用算法信息（由 FITKMeshAlgorithmGeneratorTmshExec 创建）
        Tmsh::FITKTmshGlobalMeshGenerateAlgorithmInfo* tmshAlg =
            dynamic_cast<Tmsh::FITKTmshGlobalMeshGenerateAlgorithmInfo*>(meshGenerateAlgorithmInfo);
        if (tmshAlg)
        {
            _ui->lineEdit_Option->text().trimmed();
        }
    }

    void GUITetGenSettings::on_pushButton_OK_clicked()
    {
        Interface::FITKGlobalMeshSizeInfo* meshSizeInfo = Interface::FITKMeshGenInterface::getInstance()->getGlobalMeshSizeInfo(kMesherKey);
        if (meshSizeInfo == nullptr) return;

        Interface::FITKGlobalMeshGenerateAlgorithmInfo* meshGenerateAlgorithmInfo = Interface::FITKMeshGenInterface::getInstance()->getGlobalMeshGenerateAlgorithmInfo(kMesherKey);
        if (meshGenerateAlgorithmInfo == nullptr) return;

        QVariant v = QVariant::fromValue(_virtualTopos);
        if (_driver)
        {
            _driver->setValue("virtualTopos", v);
        }

        QString userInputOption = _ui->lineEdit_Option->text().trimmed();
        QString meshFilePath = _ui->lineEdit_FilePath->text().trimmed();
        QFileInfo meshFileInfo(meshFilePath);

        //提取带后缀的文件名存储到全局接口
        if(meshFileInfo.exists() && meshFileInfo.isFile())
        {
            QString fileName = meshFileInfo.fileName();
            Interface::FITKMeshGenInterface::setMeshFileName(fileName);
        }

        if(userInputOption.isEmpty())
        {
            QMessageBox::warning(this, tr("Warning"), tr("Please enter the parameters!"));
            return;
        }
        if(meshFilePath.isEmpty() || !meshFileInfo.exists() || !meshFileInfo.isFile())
        {
            QMessageBox::warning(this, tr("Warning"), tr("File does not exist or is not a file!"));
            return;
        }

        _driver->setValue("tetgenOptions", userInputOption);

        _ui->pushButton_OK->setEnabled(false);

        if (_process == nullptr)
        {
           _process = new QProcess(this);
           // 重新连接信号（如果_process是新建的）
           connect(_process, &QProcess::readyReadStandardOutput, this, &GUITetGenSettings::onReadyReadOutput);
           connect(_process, &QProcess::readyReadStandardError, this, &GUITetGenSettings::onReadyReadError);
           connect(_process, static_cast<void (QProcess::*)(int, QProcess::ExitStatus)>(&QProcess::finished),
                   this, [=](int exitCode, QProcess::ExitStatus exitStatus)
           {
               if(exitCode == 0 && exitStatus == QProcess::NormalExit)
               {
                   QMessageBox::information(this, tr("Success"), tr("Mesh file processing is complete!"), QMessageBox::Ok);
                   this->accept();
               }
               else
               {
                   QMessageBox::critical(this, tr("Failure"), tr("Failure in processing: %1").arg(_stdErrBuffer), QMessageBox::Ok);
                   _ui->pushButton_OK->setEnabled(true);
               }    });
            }
        else
        {
            if (_process->state() != QProcess::NotRunning)
            {
                _process->kill();
                _process->waitForFinished(1000);
             }
             _process->close();
        }

        QString exePath = "D:/tetgen/tetgen.exe";
        QFileInfo tetGenInfo(exePath);
        if (!tetGenInfo.exists() || !tetGenInfo.isFile())
        {
            QString errorMsg = tr("TetGen executable not found at: %1").arg(exePath);
            qCritical() << errorMsg;
            AppFrame::FITKMessageError(errorMsg);
            QMessageBox::critical(this, tr("Error"), errorMsg, QMessageBox::Ok);
            _ui->pushButton_OK->setEnabled(true);
            return;
        }

         QString optionStr = _driver->getValue("tetgenOptions").toString().trimmed();
         QStringList cmdArgs;
         cmdArgs << optionStr.split(" ", Qt::SkipEmptyParts)
                 << meshFilePath;

         _process->start(exePath, cmdArgs);

        //this->accept();
    }

    void GUITetGenSettings::on_pushButton_Cancel_clicked()
    {
        QList<Interface::VirtualShape> virtualTopos{};
        QVariant v = QVariant::fromValue(virtualTopos);
        if (_driver)
        {
            _driver->setValue("virtualTopos", v);
        }
        this->reject();
    }


    void GUITetGenSettings::on_pushButton_Help_clicked()
    {
        GUITetGenHelp *_detailedDlg = new GUITetGenHelp;
        _detailedDlg->show();
    }

    void GUITetGenSettings::on_pushButton_Browse_clicked()
    {
        QString currentPath = _ui->lineEdit_FilePath->text();
        QString targetFolderName = "Tmsh";
        QString defaultOpenDir;
        QFileInfo fileInfo(currentPath);
        if(fileInfo.isFile())
        {
            QDir parentDir = fileInfo.dir();
            defaultOpenDir = parentDir.filePath("../" + targetFolderName);
        }
        else
        {
            QDir currentDir(currentPath);
            defaultOpenDir = currentDir.filePath("../" + targetFolderName);
        }

        QString filePath = QFileDialog::getOpenFileName(nullptr, "Import Model File", defaultOpenDir, "Geometry File(*.mesh)");
        if(!filePath.isEmpty())
        {
            _ui->lineEdit_FilePath->setText(filePath);
        }
    }

    void GUITetGenSettings::onReadyReadOutput()
    {
        _stdOutBuffer += QString::fromLocal8Bit(_process->readAllStandardOutput());
        const QChar cr = '\r';
        const QChar lf = '\n';
        int lineEndPos = -1;

        while ((lineEndPos = _stdOutBuffer.indexOf(cr)) != -1 || (lineEndPos = _stdOutBuffer.indexOf(lf)) != -1)
        {
            QString line = _stdOutBuffer.left(lineEndPos).trimmed(); // trimmed()去除首尾空格/空行
            _stdOutBuffer.remove(0, lineEndPos + 1);
            if (!line.isEmpty())
            {
                AppFrame::FITKMessageNormal(line);
            }
        }
    }

    void GUITetGenSettings::onReadyReadError()
    {
        _stdErrBuffer += QString::fromLocal8Bit(_process->readAllStandardError());
        const QChar cr = '\r';
        const QChar lf = '\n';
        int lineEndPos = -1;

        while ((lineEndPos = _stdErrBuffer.indexOf(cr)) != -1 || (lineEndPos = _stdErrBuffer.indexOf(lf)) != -1)
        {
            QString line = _stdErrBuffer.left(lineEndPos).trimmed();
            _stdErrBuffer.remove(0, lineEndPos + 1);

            if (!line.isEmpty())
            {
                AppFrame::FITKMessageWarning(line);
            }
        }
    }
}

