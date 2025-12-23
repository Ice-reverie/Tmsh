#include "OperatorExportAbaqusINPFile.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppSettings.h"
#include "FITK_Kernel/FITKAppFramework/FITKComponents.h"
#include "FITK_Kernel/FITKAppFramework/FITKMessage.h"
#include "FITK_Kernel/FITKCore/FITKThreadPool.h" 
#include "FITK_Component/FITKAbaqusIOINP/FITKAbaqusIOINPInterface.h"
#include "FITK_Component/FITKAbaqusData/FITKDataCase.h" 
#include "ModelData/MeshManager.h"
#include "ModelData/MeshData2INPConverter.h"
#include "GUIFrame/MainWindow.h"
#include <QFileDialog>

namespace ModelOper
{
    bool OperatorExportAbaqusINPFile::execGUI()
    {
        // 1.获取文件保存路径
        // 判断是否是需要导出Abaqus的INI文件
        if (_emitter == nullptr || _emitter->objectName() != "actionExportAbaqusINPFile") return false;
        
        // 初始化文件路径
        QString filePath;

        // 从全局设置中获取工作目录
        AppFrame::FITKAppSettings* setting = FITKAPP->getAppSettings();
        if (setting != nullptr) filePath = setting->getWorkingDir();
        else AppFrame::FITKMessageWarning(QObject::tr("Failed to get the working path. Please check if the work path has been set successfully!"));

        // 文件对话框
        QFileDialog dialog(FITKAPP->getGlobalData()->getMainWindow(), QObject::tr("SaveAs Abaqus INP File"), filePath, "Abaqus INP(*.inp)");
        dialog.setAcceptMode(QFileDialog::AcceptSave);
        dialog.show();

        // 阻塞，否则键盘事件处理会出问题
        bool accept = false;
        QEventLoop loop;
        connect(&dialog, &QFileDialog::accepted, [&] {loop.quit(); accept = true;  });
        connect(&dialog, &QFileDialog::rejected, [&] {loop.quit(); accept = false;  });
        loop.exec();
        if (!accept) return false;

        // 获取文件保存路径
        QStringList files = dialog.selectedFiles(); //选择的文件名称
        if (files.size() < 1) return false;
        filePath = files.at(0);
        if (filePath.isEmpty()) return false;
           
        // 自动添加后缀
        if (!filePath.toLower().endsWith(".inp")) filePath += ".inp";
        AppFrame::FITKMessageNormal(QObject::tr("The file will be saved to the following path: %1.").arg(filePath));
        // 保存参数
        this->setArgs("FileName", filePath);

        return true;
    }

    bool OperatorExportAbaqusINPFile::execProfession()
    {
        // 3.写出AbaqusINP文件
        // 获取文件保存路径
        QString fileName;
        if (!this->argValue<QString>("FileName", fileName)) return false;

        // 数据转换：ModelData》》》AbaqusData
          ModelData::MeshManager* managerMesh = FITKAPP->getGlobalData()->getMeshData<ModelData::MeshManager>();
        if (managerMesh == nullptr) return false;
        _case = new AbaqusData::FITKDataCase;
        
        // 数据转换
        ModelData::MeshData2INPConverter converter(managerMesh, _case);
        if (!converter.execDataConversion())
        {
            AppFrame::FITKMessageError(QObject::tr("Data conversion failed."));
            return false;
        }
        //输出信息
        AppFrame::FITKMessageNormal(QString(tr("Saving Abaqus INP file %1 ...")).arg(fileName));

        // 获取AbaqusINP文件读写组件
        IO::FITKAbaqusINPIO* ioAbaqusINP = FITKAPP->getComponents()->getComponentTByName<IO::FITKAbaqusINPIO>("AbaqusINPIO");
        if (ioAbaqusINP == nullptr) return false;

        // 等待线程结束
        Core::FITKThreadPool::getInstance()->wait();
        
        //传入参数
        ioAbaqusINP->setFileName(fileName);
        ioAbaqusINP->setDataObject(_case);
        ioAbaqusINP->setRunInThread(true); //设置为线程运行
        
        //关联结束读取槽函数
        connect(ioAbaqusINP, SIGNAL(ioThreadFinishedSig()), this, SLOT(ioThreadFinishedSlot()), Qt::UniqueConnection);
        //执行写出
        ioAbaqusINP->exec(2);
        // 返回执行结果
        return true;
    }

    void OperatorExportAbaqusINPFile::ioThreadFinishedSlot()
    {
        // 4.清除临时转换数据
        // 清除临时数据
        _case->clear();
        delete _case;
        _case = nullptr;
        // 清除参数
        this->clearArgs();
        // 获取执行结果
        IO::FITKAbaqusINPIO* io = dynamic_cast<IO::FITKAbaqusINPIO*>(sender());
        if (io->isSuccess()) 
            AppFrame::FITKMessageNormal(QObject::tr("The file has been written successfully."));
        else 
            AppFrame::FITKMessageError(QObject::tr("File writing failed."));
    }

}

