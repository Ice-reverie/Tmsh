#include "FITKExecProgramDriver.h"
#include "FITKAppFramework.h"
#include "FITKMessage.h"
#include "FITKSignalTransfer.h"
#include <QProcess>
#include <QFile>

namespace AppFrame
{
    namespace
    {
        constexpr int kMaxEmitBytes = 32 * 1024;

        int findLineBreak(const QByteArray& data)
        {
            const int n = data.size();
            for (int i = 0; i < n; ++i)
            {
                const char c = data.at(i);
                if (c == '\n') return i + 1;
                if (c == '\r')
                {
                    if (i + 1 < n && data.at(i + 1) == '\n') return i + 2;
                    return i + 1;
                }
            }
            return -1;
        }

        void appendAndTakeLines(QByteArray& pending, const QByteArray& chunk, QByteArray& out)
        {
            if (!chunk.isEmpty())
            {
                pending.append(chunk);
            }

            while (out.size() < kMaxEmitBytes)
            {
                const int takeCount = findLineBreak(pending);
                if (takeCount < 0) break;

                out.append(pending.left(takeCount));
                pending.remove(0, takeCount);
            }

            // If producer outputs long chunks without line breaks, still stream it out.
            if (out.isEmpty() && pending.size() >= kMaxEmitBytes)
            {
                out.append(pending.left(kMaxEmitBytes));
                pending.remove(0, kMaxEmitBytes);
            }
        }
    }

    FITKExecProgramDriver::FITKExecProgramDriver()
    {
        //创建进程对象
        _process = new QProcess;
        // _process->setReadChannel(QProcess::StandardOutput);

        //消息输出
        connect(_process, &QProcess::readyReadStandardOutput, this, &FITKExecProgramDriver::sendMessageSlot);
        connect(_process, &QProcess::readyReadStandardError, this, &FITKExecProgramDriver::sendMessageSlot);
        connect(_process,
                QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
                this,
                &FITKExecProgramDriver::flushPendingOutputSlot);
    }

    FITKExecProgramDriver::~FITKExecProgramDriver()
    {
        //杀死进程
        if (!_process) return;
        disconnect(_process, &QProcess::readyReadStandardOutput, this, &FITKExecProgramDriver::sendMessageSlot);
        disconnect(_process, &QProcess::readyReadStandardError, this, &FITKExecProgramDriver::sendMessageSlot);
        disconnect(_process,
                   QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
                   this,
                   &FITKExecProgramDriver::flushPendingOutputSlot);
        disconnect(_process, SIGNAL(finished(int)), this, SIGNAL(sig_Finish()));
        _process->kill();
        _process->waitForFinished();
       // 释放进程对象 
//       delete _process;
//       _process = nullptr;
    }

    void FITKExecProgramDriver::setExecProgram(const QString & program)
    {
        _program = program;
    }

    QString FITKExecProgramDriver::getExecProgram() const
    {
        return _program;
    }

    void FITKExecProgramDriver::setWorkingDirectory(const QString & path)
    {
        _workDir = path;
    }

    QString FITKExecProgramDriver::getWorkingDirectory() const
    {
        return _workDir;
    } 

    void FITKExecProgramDriver::start()
    {
        //启动进程
        if (_process == nullptr) return;
        QStringList args;
        if (_inputInfo != nullptr)
            args = _inputInfo->args();

        //检查可执行程序是否存在
        if (!QFile::exists(_program))
            AppFrame::FITKMessageError(QString("Executable program is not exist: %1").arg(_program));

        // 设置工作路径。
        if (!_workDir.isEmpty())
        {
            _process->setWorkingDirectory(_workDir);
        }

        //信号关联（避免多次 start() 重复连接导致重复触发）
        disconnect(_process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), this, &FITKExecProgramDriver::sig_Finish);
        connect(_process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), this, &FITKExecProgramDriver::sig_Finish, Qt::UniqueConnection);

        // Merge stdout/stderr to keep output ordering closer to terminal behavior.
        _process->setProcessChannelMode(QProcess::MergedChannels);
        _process->start(_program, args);
    }

    void FITKExecProgramDriver::stop()
    {
        //终止进程
        if (_process == nullptr) return;
        _process->kill();
        _process->waitForFinished();
        this->flushPendingOutputSlot();
    }

    void FITKExecProgramDriver::enableSendMessage(bool s)
    {
        _sendMessage = s;
    }

    
    bool FITKExecProgramDriver::isSendMessage() const
    {
        return _sendMessage;
    }

    void FITKExecProgramDriver::sendMessageSlot()
    {
        //读取信息
        if (!_sendMessage ||  _process == nullptr) return;

        QByteArray outBytes;
        appendAndTakeLines(_stdoutPending, _process->readAllStandardOutput(), outBytes);
        if (!outBytes.isEmpty())
        {
            this->sendMessage(0, QString::fromLocal8Bit(outBytes));
        }

        QByteArray errBytes;
        appendAndTakeLines(_stderrPending, _process->readAllStandardError(), errBytes);
        if (!errBytes.isEmpty())
        {
            this->sendMessage(0, QString::fromLocal8Bit(errBytes));
        }
    }

    void FITKExecProgramDriver::flushPendingOutputSlot()
    {
        if (!_sendMessage || _process == nullptr) return;

        // Drain remaining bytes from the process first.
        const QByteArray outTail = _process->readAllStandardOutput();
        if (!outTail.isEmpty()) _stdoutPending.append(outTail);
        const QByteArray errTail = _process->readAllStandardError();
        if (!errTail.isEmpty()) _stderrPending.append(errTail);

        if (!_stdoutPending.isEmpty())
        {
            this->sendMessage(0, QString::fromLocal8Bit(_stdoutPending));
            _stdoutPending.clear();
        }
        if (!_stderrPending.isEmpty())
        {
            this->sendMessage(0, QString::fromLocal8Bit(_stderrPending));
            _stderrPending.clear();
        }
    }
}
