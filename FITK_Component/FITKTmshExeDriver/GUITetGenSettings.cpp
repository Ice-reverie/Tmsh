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
#include <algorithm>
#include <QTimer>
#include <QDateTime>

namespace GUI
{
    namespace
    {
        constexpr const char* kMesherKey = "TmshExec";
    }

    GUITetGenSettings::GUITetGenSettings(QWidget* parent/* = nullptr*/) : Core::FITKDialog(parent), _process(new QProcess(this)), _isDestroying(false)
    {
        _ui = new Ui::GUITetGenSettings();
        _ui->setupUi(this);
        setAttribute(Qt::WA_DeleteOnClose);
        Qt::WindowFlags flags = windowFlags();
        flags &= ~Qt::WindowContextHelpButtonHint;
        setWindowFlags(flags);
        
        _timeoutTimer = new QTimer(this);
        _progressTimer = new QTimer(this);
        _timeoutSeconds = 300;
        _noOutputTimeoutSeconds = 60;
        _lastOutputTime = QDateTime::currentSecsSinceEpoch();

        connect(_process, &QProcess::readyReadStandardOutput, this, &GUITetGenSettings::onReadyReadOutput);
        connect(_process, &QProcess::readyReadStandardError, this, &GUITetGenSettings::onReadyReadError);
        connect(_process, static_cast<void (QProcess::*)(int, QProcess::ExitStatus)>(&QProcess::finished),
                this, &GUITetGenSettings::onProcessFinished);
        
        this->init();
    }

    GUITetGenSettings::GUITetGenSettings(Interface::FITKAbstractMesherDriver* driver, QWidget* parent/* = nullptr*/)
        : Core::FITKDialog(parent), _driver(driver), _process(new QProcess(this)), _isDestroying(false)
    {
        _ui = new Ui::GUITetGenSettings();
        _ui->setupUi(this);
        setAttribute(Qt::WA_DeleteOnClose);
        Qt::WindowFlags flags = windowFlags();
        flags &= ~Qt::WindowContextHelpButtonHint;
        setWindowFlags(flags);

        _timeoutTimer = new QTimer(this);
        _progressTimer = new QTimer(this);
        _timeoutSeconds = 300;
        _noOutputTimeoutSeconds = 60;
        _lastOutputTime = QDateTime::currentSecsSinceEpoch();

        connect(_process, &QProcess::readyReadStandardOutput, this, &GUITetGenSettings::onReadyReadOutput);
        connect(_process, &QProcess::readyReadStandardError, this, &GUITetGenSettings::onReadyReadError);
        connect(_process, static_cast<void (QProcess::*)(int, QProcess::ExitStatus)>(&QProcess::finished),
                this, &GUITetGenSettings::onProcessFinished);

        this->init();
    }

    GUITetGenSettings::~GUITetGenSettings()
    {
        _isDestroying = true;
        
        if (_timeoutTimer)
        {
            _timeoutTimer->stop();
            disconnect(_timeoutTimer, nullptr, this, nullptr);
        }
        
        if (_progressTimer)
        {
            _progressTimer->stop();
            disconnect(_progressTimer, nullptr, this, nullptr);
        }
        
        if (_process)
        {
            disconnect(_process, nullptr, this, nullptr);
            if (_process->state() != QProcess::NotRunning)
            {
                _process->kill();
                _process->waitForFinished(3000);
            }
        }
        
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
        if (signalTransfer)
        {
            emit signalTransfer->setPickableObjTypeSig(-1);
        }
    }


    void GUITetGenSettings::init()
    {
        setWindowTitle(tr("TetGen Settings"));

        Interface::FITKGlobalMeshSizeInfo* meshSizeInfo = Interface::FITKMeshGenInterface::getInstance()->getGlobalMeshSizeInfo(kMesherKey);
        Interface::FITKGlobalMeshGenerateAlgorithmInfo* meshGenerateAlgorithmInfo = Interface::FITKMeshGenInterface::getInstance()->getGlobalMeshGenerateAlgorithmInfo(kMesherKey);
        if (!meshSizeInfo || !meshGenerateAlgorithmInfo) return;

        Tmsh::FITKTmshGlobalMeshGenerateAlgorithmInfo* tmshAlg =
            dynamic_cast<Tmsh::FITKTmshGlobalMeshGenerateAlgorithmInfo*>(meshGenerateAlgorithmInfo);
        if (tmshAlg)
        {
            _ui->lineEdit_Option->text().trimmed();
        }

        connect(_ui->checkBox_p, &QCheckBox::stateChanged, this, &GUITetGenSettings::onOptionChanged);
        connect(_ui->checkBox_Y, &QCheckBox::stateChanged, this, &GUITetGenSettings::onOptionChanged);
        connect(_ui->checkBox_r, &QCheckBox::stateChanged, this, &GUITetGenSettings::onOptionChanged);
        connect(_ui->checkBox_q, &QCheckBox::stateChanged, this, &GUITetGenSettings::onOptionChanged);
        connect(_ui->doubleSpinBox_ratio, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &GUITetGenSettings::onOptionChanged);
        connect(_ui->doubleSpinBox_angle, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &GUITetGenSettings::onOptionChanged);
        connect(_ui->checkBox_a, &QCheckBox::stateChanged, this, &GUITetGenSettings::onOptionChanged);
        connect(_ui->doubleSpinBox_volume, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &GUITetGenSettings::onOptionChanged);
        connect(_ui->checkBox_m, &QCheckBox::stateChanged, this, &GUITetGenSettings::onOptionChanged);
        connect(_ui->checkBox_O, &QCheckBox::stateChanged, this, &GUITetGenSettings::onOptionChanged);
        connect(_ui->spinBox_level, QOverload<int>::of(&QSpinBox::valueChanged), this, &GUITetGenSettings::onOptionChanged);
        connect(_ui->checkBox_flip, &QCheckBox::stateChanged, this, &GUITetGenSettings::onOptionChanged);
        connect(_ui->checkBox_smooth, &QCheckBox::stateChanged, this, &GUITetGenSettings::onOptionChanged);
        connect(_ui->checkBox_vertex, &QCheckBox::stateChanged, this, &GUITetGenSettings::onOptionChanged);
        connect(_ui->spinBox_iter, QOverload<int>::of(&QSpinBox::valueChanged), this, &GUITetGenSettings::onOptionChanged);
        connect(_ui->checkBox_A, &QCheckBox::stateChanged, this, &GUITetGenSettings::onOptionChanged);
        connect(_ui->checkBox_f, &QCheckBox::stateChanged, this, &GUITetGenSettings::onOptionChanged);
        connect(_ui->checkBox_e, &QCheckBox::stateChanged, this, &GUITetGenSettings::onOptionChanged);
        connect(_ui->checkBox_n, &QCheckBox::stateChanged, this, &GUITetGenSettings::onOptionChanged);
        connect(_ui->checkBox_k, &QCheckBox::stateChanged, this, &GUITetGenSettings::onOptionChanged);
        connect(_ui->checkBox_g, &QCheckBox::stateChanged, this, &GUITetGenSettings::onOptionChanged);
        connect(_ui->checkBox_z, &QCheckBox::stateChanged, this, &GUITetGenSettings::onOptionChanged);
        connect(_ui->checkBox_o2, &QCheckBox::stateChanged, this, &GUITetGenSettings::onOptionChanged);
        connect(_ui->lineEdit_Option, &QLineEdit::textChanged, this, &GUITetGenSettings::onOptionChanged);

        updatePreview();
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

        if(meshFileInfo.exists() && meshFileInfo.isFile())
        {
            QString fileName = meshFileInfo.fileName();
            Interface::FITKMeshGenInterface::setMeshFileName(fileName);
        }

        QString optionStr;
        if (!userInputOption.isEmpty())
        {
            optionStr = userInputOption;
        }
        else
        {
            optionStr = generateCommand();
            if(optionStr.isEmpty())
            {
                QMessageBox::warning(this, tr("Warning"), tr("Please configure at least one option!"));
                return;
            }
        }
        if(meshFilePath.isEmpty() || !meshFileInfo.exists() || !meshFileInfo.isFile())
        {
            QMessageBox::warning(this, tr("Warning"), tr("File does not exist or is not a file!"));
            return;
        }

        QString fileSuffix = meshFileInfo.suffix().toLower();
        QStringList supportedFormats = {"poly", "smesh", "node", "stl", "ply", "off", "mesh"};
        if (!supportedFormats.contains(fileSuffix))
        {
            QMessageBox::warning(this, tr("Warning"), 
                tr("Unsupported file format: .%1\nSupported formats: .poly, .smesh, .node, .stl, .ply, .off, .mesh").arg(fileSuffix));
            return;
        }

        _driver->setValue("tetgenOptions", optionStr);

        _ui->pushButton_OK->setEnabled(false);

        if (_process->state() != QProcess::NotRunning)
        {
            _process->kill();
            _process->waitForFinished(3000);
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

         QStringList cmdArgs;
         cmdArgs << optionStr.split(" ", Qt::SkipEmptyParts)
                 << meshFilePath;

         qDebug() << "Starting TetGen:" << exePath;
         qDebug() << "Arguments:" << cmdArgs;
         qDebug() << "Working directory:" << QFileInfo(meshFilePath).absolutePath();
         qDebug() << "Input file exists:" << QFileInfo(meshFilePath).exists();
         qDebug() << "Input file size:" << QFileInfo(meshFilePath).size();
         
         // Set working directory to input file's directory
         _process->setWorkingDirectory(QFileInfo(meshFilePath).absolutePath());
         
         // Try using startDetached for debugging
         _process->start(exePath, cmdArgs);
         
         if (!_process->waitForStarted(5000))
         {
             QString errMsg = tr("Failed to start TetGen: %1").arg(_process->errorString());
             qCritical() << errMsg;
             QMessageBox::critical(this, tr("Error"), errMsg, QMessageBox::Ok);
             _ui->pushButton_OK->setEnabled(true);
             return;
         }
         
         qDebug() << "TetGen started successfully, waiting for completion...";
         
         _lastOutputTime = QDateTime::currentSecsSinceEpoch();
         
         connect(_timeoutTimer, &QTimer::timeout, this, &GUITetGenSettings::onProcessTimeout);
         _timeoutTimer->start(_timeoutSeconds * 1000);
         
         connect(_progressTimer, &QTimer::timeout, this, &GUITetGenSettings::onProgressTimeout);
         _progressTimer->start(5000);

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

        QString filePath = QFileDialog::getOpenFileName(nullptr, "Import Model File", defaultOpenDir,
            "All Supported Files (*.poly *.smesh *.node *.stl *.ply *.off *.mesh);;"
            "POLY Files (*.poly);;"
            "SMESH Files (*.smesh);;"
            "NODE Files (*.node);;"
            "STL Files (*.stl);;"
            "PLY Files (*.ply);;"
            "OFF Files (*.off);;"
            "MESH Files (*.mesh);;"
            "All Files (*.*)");
        if(!filePath.isEmpty())
        {
            _ui->lineEdit_FilePath->setText(filePath);
        }
    }

    void GUITetGenSettings::onReadyReadOutput()
    {
        if (_isDestroying || !_process) return;
        
        QByteArray data = _process->readAllStandardOutput();
        if (data.isEmpty()) return;
        _lastOutputTime = QDateTime::currentSecsSinceEpoch();
        _stdOutBuffer += QString::fromUtf8(data);
        
        int pos = 0;
        while ((pos = _stdOutBuffer.indexOf('\n')) != -1 && pos >= 0)
        {
            QString line = _stdOutBuffer.left(pos);
            if (line.endsWith('\r'))
                line.chop(1);
            _stdOutBuffer.remove(0, pos + 1);
            line = line.trimmed();
            if (!line.isEmpty())
            {
                AppFrame::FITKMessageNormal(line);
            }
        }
    }

    void GUITetGenSettings::onReadyReadError()
    {
        if (_isDestroying || !_process) return;
        
        QByteArray data = _process->readAllStandardError();
        if (data.isEmpty()) return;
        
        _lastOutputTime = QDateTime::currentSecsSinceEpoch();
        qDebug() << "TetGen stderr:" << data;
        _stdErrBuffer += QString::fromUtf8(data);
        
        int pos = 0;
        while ((pos = _stdErrBuffer.indexOf('\n')) != -1 && pos >= 0)
        {
            QString line = _stdErrBuffer.left(pos);
            if (line.endsWith('\r'))
                line.chop(1);
            _stdErrBuffer.remove(0, pos + 1);
            line = line.trimmed();
            if (!line.isEmpty())
            {
                AppFrame::FITKMessageWarning(line);
            }
        }
    }

    void GUITetGenSettings::onProcessTimeout()
    {
        if (_isDestroying) return;
        
        QString timeoutMsg = tr("TetGen process exceeded maximum execution time of %1 seconds!").arg(_timeoutSeconds);
        qCritical() << timeoutMsg;
        AppFrame::FITKMessageError(timeoutMsg);
        
        QMessageBox::critical(nullptr, tr("Process Timeout"), 
            tr("TetGen process has been running for more than %1 seconds.\n\n"
               "This may indicate:\n"
               "- Complex geometry causing long processing time\n"
               "- Process is stuck in an infinite loop\n"
               "- Insufficient system resources\n\n"
               "The process will be terminated.").arg(_timeoutSeconds), 
            QMessageBox::Ok);
        
        if (_process && _process->state() != QProcess::NotRunning)
        {
            _process->kill();
            _process->waitForFinished(3000);
        }
        
        if (_timeoutTimer) _timeoutTimer->stop();
        if (_progressTimer) _progressTimer->stop();
        if (_ui) _ui->pushButton_OK->setEnabled(true);
    }

    void GUITetGenSettings::onProcessFinished(int exitCode, QProcess::ExitStatus exitStatus)
    {
        if (_isDestroying) return;
        
        qDebug() << "TetGen finished with exitCode:" << exitCode << "exitStatus:" << exitStatus;
        
        if (_timeoutTimer) _timeoutTimer->stop();
        if (_progressTimer) _progressTimer->stop();
        
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

        if (_ui)
        {
            _ui->pushButton_OK->setEnabled(true);
        }
        
        if(exitCode == 0 && exitStatus == QProcess::NormalExit)
        {
            QMessageBox::information(nullptr, tr("Success"), tr("Mesh file processing is complete!"), QMessageBox::Ok);
            this->accept();
        }
        else
        {
            QString errMsg = _stdErrBuffer.isEmpty() ? tr("Process exited with code %1").arg(exitCode) : _stdErrBuffer;
            QMessageBox::critical(nullptr, tr("Failure"), tr("Failure in processing: %1").arg(errMsg), QMessageBox::Ok);
        }
    }

    void GUITetGenSettings::onProgressTimeout()
    {
        if (_isDestroying || !_process || !_ui) return;
        
        qint64 currentTime = QDateTime::currentSecsSinceEpoch();
        qint64 elapsed = currentTime - _lastOutputTime;
        
        qDebug() << "Process state:" << _process->state() 
                 << "pid:" << _process->processId()
                 << "bytesAvailable:" << _process->bytesAvailable()
                 << "No output for:" << elapsed << "seconds";
        
        if (_process->state() == QProcess::NotRunning)
        {
            qDebug() << "Process is NotRunning but finished signal not received!";
            if (_timeoutTimer) _timeoutTimer->stop();
            if (_progressTimer) _progressTimer->stop();
            return;
        }
        
        if (elapsed >= _noOutputTimeoutSeconds)
        {
            QString timeoutMsg = tr("TetGen process has no output for %1 seconds, possible hang detected!").arg(elapsed);
            qCritical() << timeoutMsg;
            AppFrame::FITKMessageError(timeoutMsg);
            QMessageBox::critical(nullptr, tr("Process Timeout"), 
                tr("TetGen process appears to be stuck (no output for %1 seconds).\n\n"
                   "This may indicate:\n"
                   "- Geometry issues causing infinite loops\n"
                   "- Insufficient memory\n"
                   "- Invalid input parameters\n\n"
                   "The process will be terminated.").arg(elapsed), 
                QMessageBox::Ok);
            
            if (_process->state() != QProcess::NotRunning)
            {
                _process->kill();
                _process->waitForFinished(3000);
            }
            
            if (_timeoutTimer) _timeoutTimer->stop();
            if (_progressTimer) _progressTimer->stop();
            if (_ui) _ui->pushButton_OK->setEnabled(true);
        }
    }

    void GUITetGenSettings::onOptionChanged()
    {
        if (_isDestroying) return;
        updatePreview();
    }

    void GUITetGenSettings::on_pushButton_Preview_clicked()
    {
        if (_isDestroying) return;
        QString cmd = generateCommand();
        QMessageBox::information(this, tr("Generated Command"), 
            tr("Generated TetGen command:\n\n%1").arg(cmd), QMessageBox::Ok);
    }

    void GUITetGenSettings::updatePreview()
    {
        if (_isDestroying || !_ui) return;
        
        QString manualInput = _ui->lineEdit_Option->text().trimmed();
        if (!manualInput.isEmpty())
        {
            _ui->lineEdit_Preview->setText(manualInput);
        }
        else
        {
            QString cmd = generateCommand();
            _ui->lineEdit_Preview->setText(cmd);
        }
    }

    QString GUITetGenSettings::generateCommand()
    {
        if (_isDestroying || !_ui) return "";
        
        QStringList options;
        
        options << generateBasicOptions();
        options << generateQualityOptions();
        options << generateAdaptiveOptions();
        options << generateOptimizeOptions();
        options << generateRegionOptions();
        options << generateOutputOptions();
        
        options.removeAll("");
        
        return options.join(" ");
    }

    QString GUITetGenSettings::generateBasicOptions()
    {
        if (_isDestroying || !_ui) return "";
        
        QStringList basic;
        
        if (_ui->checkBox_p->isChecked())
        {
            basic << "-p";
        }
        
        if (_ui->checkBox_Y->isChecked())
        {
            basic << "-Y";
        }
        
        if (_ui->checkBox_r->isChecked())
        {
            basic << "-r";
        }
        
        return basic.join(" ");
    }

    QString GUITetGenSettings::generateQualityOptions()
    {
        if (_isDestroying || !_ui) return "";
        
        if (!_ui->checkBox_q->isChecked())
        {
            return "";
        }
        
        double ratio = _ui->doubleSpinBox_ratio->value();
        double angle = _ui->doubleSpinBox_angle->value();
        
        QString qOpt = QString("-q%1").arg(ratio);
        if (angle > 0.0)
        {
            qOpt += QString("/%1").arg(static_cast<int>(angle));
        }
        
        return qOpt;
    }

    QString GUITetGenSettings::generateAdaptiveOptions()
    {
        if (_isDestroying || !_ui) return "";
        
        QStringList adaptive;
        
        if (_ui->checkBox_a->isChecked())
        {
            double volume = _ui->doubleSpinBox_volume->value();
            adaptive << QString("-a%1").arg(volume);
        }
        
        if (_ui->checkBox_m->isChecked())
        {
            adaptive << "-m";
        }
        
        return adaptive.join(" ");
    }

    QString GUITetGenSettings::generateOptimizeOptions()
    {
        if (_isDestroying || !_ui) return "";
        
        if (!_ui->checkBox_O->isChecked())
        {
            return "";
        }
        
        int level = _ui->spinBox_level->value();
        int iter = _ui->spinBox_iter->value();
        
        bool flip = _ui->checkBox_flip->isChecked();
        bool smooth = _ui->checkBox_smooth->isChecked();
        bool vertex = _ui->checkBox_vertex->isChecked();
        
        QString OOpt = QString("-O%1").arg(level);
        
        QStringList ops;
        if (flip) ops << "1";
        if (smooth) ops << "2";
        if (vertex) ops << "3";
        
        if (!ops.isEmpty())
        {
            OOpt += "/" + ops.join("");
        }
        
        OOpt += QString("/%1").arg(iter);
        
        return OOpt;
    }

    QString GUITetGenSettings::generateRegionOptions()
    {
        if (_isDestroying || !_ui) return "";
        
        if (_ui->checkBox_A->isChecked())
        {
            return "-A";
        }
        return "";
    }

    QString GUITetGenSettings::generateOutputOptions()
    {
        if (_isDestroying || !_ui) return "";
        
        QStringList output;
        
        if (_ui->checkBox_f->isChecked())
        {
            output << "-f";
        }
        
        if (_ui->checkBox_e->isChecked())
        {
            output << "-e";
        }
        
        if (_ui->checkBox_n->isChecked())
        {
            output << "-n";
        }
        
        if (_ui->checkBox_k->isChecked())
        {
            output << "-k";
        }
        
        if (_ui->checkBox_g->isChecked())
        {
            output << "-g";
        }
        
        if (_ui->checkBox_z->isChecked())
        {
            output << "-z";
        }
        
        if (_ui->checkBox_o2->isChecked())
        {
            output << "-o2";
        }
        
        return output.join(" ");
    }
}
