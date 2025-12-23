#include "OperatorExportCGNSFile.h"

#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Kernel/FITKAppFramework/FITKComponents.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppSettings.h"
#include "FITK_Kernel/FITKCore/FITKThreadPool.h"
#include "FITK_Kernel/FITKCore/FITKVec3D.h"

#include "FITK_Interface/FITKInterfaceModel/FITKUnstructuredMesh.h"
#include "FITK_Interface/FITKInterfaceModel/FITKElementFactory.h"
#include "FITK_Interface/FITKInterfaceModel/FITKAbstractElement.h"
#include "FITK_Interface/FITKInterfaceModel/FITKModelSet.h"
#include "FITK_Interface/FITKInterfaceModel/FITKComponentManager.h"

#include "ModelData/MeshManager.h"
#include "ModelData/MeshData.h"
#include "ModelData/MeshKernel.h"

#include <QComboBox>
#include <QFileDialog>
#include <QEventLoop>
#include <QStandardPaths>
#include <QVBoxLayout>
#include <QLabel>

namespace ModelOper
{
    OperatorExportCGNSFile::OperatorExportCGNSFile()
    {

    }

    OperatorExportCGNSFile::~OperatorExportCGNSFile()
    {

    }

    bool OperatorExportCGNSFile::execGUI()
    {
        //获取工作目录
        QString wk;
        auto settings = FITKAPP->getAppSettings();
        if (settings) wk = settings->getWorkingDir();

        // 文件对话框
        QFileDialog dlg(FITKAPP->getGlobalData()->getMainWindow(), QObject::tr("SaveAs Abaqus INP File"), wk, "CGNS(*.cgns)");
        dlg.setAcceptMode(QFileDialog::AcceptSave);
        // 允许输入新文件名以便直接创建
        dlg.setFileMode(QFileDialog::AnyFile);
        dlg.setOption(QFileDialog::DontUseNativeDialog);//不使用操作系统提供的原生文件对话框，而是使用 Qt 自带的文件对话框。
        //左侧文件夹快捷方式添加
        QList<QUrl> urls = dlg.sidebarUrls();
#ifdef Q_OS_WIN
        urls.append(QUrl::fromLocalFile(QStandardPaths::writableLocation(QStandardPaths::DesktopLocation)));
        urls.append(QUrl::fromLocalFile(QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation)));
        urls.append(QUrl::fromLocalFile(QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation)));
#endif
        dlg.setSidebarUrls(urls);

        //追加数据过滤界面
        QVBoxLayout *mainlayout = dlg.findChild<QVBoxLayout *>(); //获取QFileDialog对话框的主布局。
        if (mainlayout)
            mainlayout->addLayout(this->getKernelMesh(&dlg));

        dlg.show();

        //阻塞，否则键盘事件处理会出问题
        bool accept = false;
        QEventLoop loop;
        connect(&dlg, &QFileDialog::accepted, [&] {loop.quit(); accept = true;  });
        connect(&dlg, &QFileDialog::rejected, [&] {loop.quit(); accept = false;  });
        loop.exec();
        if (!accept) return false;
        
        // 获取文件保存路径
        QStringList files = dlg.selectedFiles(); //选择的文件名称
        if (files.size() < 1) return false;
        QString filePath = files.at(0);
        if (filePath.isEmpty()) return false;

        // 自动添加后缀
        if (!filePath.toLower().endsWith(".cgns")) filePath += ".cgns";
        // 保存参数
        this->setArgs("FileName", filePath);
        return true;
    }

    bool OperatorExportCGNSFile::execProfession()
    {
        // 获取文件保存路径
        QString fileName;
        if (!this->argValue<QString>("FileName", fileName)) return false;
        int kernelID = 0;
        if (!this->argValue<int>("KernelID", kernelID)) return false;

        //获取线程池
        Core::FITKThreadPool* pool = Core::FITKThreadPool::getInstance();
        if (pool == nullptr)return false;
        //创建导出CGNS文件线程
        ExportCGNSWriteThread* exportThread = new ExportCGNSWriteThread();
        exportThread->_fileName = fileName;
        exportThread->_kernelIDs.append(kernelID);
        //添加到线程池
        pool->execTask(exportThread);
        //清理参数
        this->clearArgs();
        return true;
    }

    QHBoxLayout * OperatorExportCGNSFile::getKernelMesh(QWidget * parent)
    {
        //获取网格管理器
        ModelData::MeshManager* managerMesh = FITKAPP->getGlobalData()->getMeshData<ModelData::MeshManager>();
        if (managerMesh == nullptr) return nullptr;
        //创建过滤器
        QHBoxLayout* layout = new QHBoxLayout(parent);
        layout->addWidget(new QLabel(tr("Mesh:"), parent));
        QComboBox* comboBox = new QComboBox(parent);
        layout->addWidget(comboBox);
        //设置数据
        const int dataCount = managerMesh->getDataCount();
        for (int i = 0; i < dataCount; ++i)
        {
            //获取网格数据
            ModelData::MeshData* meshData = managerMesh->getDataByIndex(i);
            if (meshData == nullptr) continue;
            const int meshKernelCount = meshData->getDataCount();
            for (int j = 0; j < meshKernelCount; ++j)
            {
                ModelData::MeshKernel* meshKernel = meshData->getDataByIndex(j);
                if (meshKernel == nullptr) continue;
                QString name = meshKernel->getDataObjectName();
                if (name.isEmpty()) name = tr("Mesh %1").arg(j + 1);
                comboBox->addItem(name, meshKernel->getDataObjectID());
            }
        }
        this->setArgs("KernelID", comboBox->currentData().toInt());
        QObject::connect(comboBox, QOverload<int>::of(&QComboBox::activated), [this, comboBox] () {
            int kernelID = comboBox->currentData().toInt();
            this->setArgs("KernelID", kernelID);
        });
        return layout;
    }

    void ExportCGNSWriteThread::run()
    {
        //设置网格处理所需的数据参数
        IO::CGNSDataIO data;
        data.CreateMeshFun = [&]() {return new Interface::FITKUnstructuredMesh; };
        //预处理
        this->writeCGNSMeshPreProcessing(data);
        //获取写出CGNS组件
        auto ofMeshWriter = FITKAPP->getComponents()->getComponentTByName<IO::FITKCGNSIOInterface>("IO::FITKCGNSIOInterface");
        if (ofMeshWriter == nullptr) return;
        ofMeshWriter->setFileName(_fileName);
        ofMeshWriter->setCGNSDataIO(&data);
        ofMeshWriter->exec(2);
        //处理写出完之后的数据
        this->writeCGNSMeshFinishProcessing();
    }

    void ExportCGNSWriteThread::writeCGNSMeshPreProcessing(IO::CGNSDataIO & data)
    {
        //获取网格管理器
        ModelData::MeshManager* managerMesh = FITKAPP->getGlobalData()->getMeshData<ModelData::MeshManager>();
        if (managerMesh == nullptr) return;
        const int dataCount = managerMesh->getDataCount();
        for (int i = 0; i < dataCount; ++i)
        {
            //获取网格数据
            ModelData::MeshData* meshData = managerMesh->getDataByIndex(i);
            if (meshData == nullptr) continue;
            const int meshKernelCount = meshData->getDataCount();
            for (int j = 0; j < meshKernelCount; ++j)
            {
                ModelData::MeshKernel* meshKernel = meshData->getDataByIndex(j);
                if (meshKernel == nullptr) continue;
                if (!_kernelIDs.contains(meshKernel->getDataObjectID())) continue;
                Interface::FITKUnstructuredMesh* mesh = dynamic_cast<Interface::FITKUnstructuredMesh*>(data.CreateMeshFun());
                QList<Interface::FITKModelSet*> setList;
                //数据转换
                bool conversionSuccess = convertMeshKernelToCGNS(meshKernel, mesh, setList);
                if (conversionSuccess)
                {
                    data._mesh.append(mesh);
                    data._componentManager.append(setList);
                }
                else
                {
                    delete mesh;
                    for (Interface::FITKModelSet* set : setList)
                    {
                        if (set) delete set;
                    }
                }
            }
        }
    }

    void ExportCGNSWriteThread::writeCGNSMeshFinishProcessing()
    {
        //获取读取接口
        IO::FITKCGNSIOInterface* interfaceIO = FITKAPP->getComponents()->getComponentTByName<IO::FITKCGNSIOInterface>("IO::FITKCGNSIOInterface");
        if (!interfaceIO) return;
        IO::CGNSDataIO data = interfaceIO->getCGNSDataIO();
        //清空数据
        for (int i = 0; i < data._mesh.size(); ++i)
        {
            for (int j = 0; j < data._componentManager[i].size(); ++j)
            {
                if (data._componentManager[i][j])
                    delete data._componentManager[i][j];
            }
            if (data._mesh.at(i))
                delete data._mesh[i];
        }
        interfaceIO->clearCGNSDataIO();
    }

    bool ExportCGNSWriteThread::convertMeshKernelToCGNS(ModelData::MeshKernel * meshKernel, Interface::FITKUnstructuredMesh * mesh, QList<Interface::FITKModelSet*>& setList)
    {
        if (meshKernel == nullptr || mesh == nullptr) return false;
        mesh->setDataObjectName(meshKernel->getDataObjectName());
        // 获取模型网格数据
        Interface::FITKAbstractMesh* dataModelMesh = meshKernel->getMesh();
        if (dataModelMesh == nullptr) return false;
        Interface::FITKModelEnum::AbsModelType modelType = dataModelMesh->getAbsModelType();
        // 根据模型类型设置Abaqus部件的属性
        switch (modelType)
        {
        case Interface::FITKModelEnum::AbsModelType::AMTunstructuredMesh:
        case Interface::FITKModelEnum::AbsModelType::AMTunstructuredMeshvtk:
            // 处理非结构化网格
            this->convertUnstructuredMeshToCGNS(meshKernel, mesh, setList);
            break;
        case Interface::FITKModelEnum::AbsModelType::AMTStructuredMesh:
            // 处理结构化网格
            this->convertStructuredMeshToCGNS(meshKernel, mesh, setList);
            break;
        }
        return true;
    }

    void ExportCGNSWriteThread::convertUnstructuredMeshToCGNS(ModelData::MeshKernel * meshKernel, Interface::FITKUnstructuredMesh * mesh, QList<Interface::FITKModelSet*>& setList)
    {
        if (!meshKernel) return;
        // 获取网格数据
        Interface::FITKUnstructuredMesh* unstructuredMesh = dynamic_cast<Interface::FITKUnstructuredMesh*>(meshKernel->getMesh());
        if (unstructuredMesh == nullptr) return;
        // 转换节点数据
        QHash<int, int> nodeMapping;
        const int nodeCount = unstructuredMesh->getNodeCount();
        for (int i = 0; i < nodeCount; i++)
        {
            Core::FITKNode* node = unstructuredMesh->getNodeAt(i);
            if (node == nullptr) continue;
            int id = node->getNodeID();
            int nodeID = mesh->addNode(node->x(), node->y(), node->z());
            nodeMapping.insert(id, nodeID);
        }
        // 转换单元数据
        QHash<int, int> elementMapping;
        int eleMaxID = mesh->getElementMaxID();
        const int elementCount = unstructuredMesh->getElementCount();
        for (int i = 0; i < elementCount; i++)
        {
            Interface::FITKAbstractElement* element = unstructuredMesh->getElementAt(i);
            if (element == nullptr) continue;
            int eID = element->getEleID();
            QList<int> ids = element->getAllNodes();
            QList<int> nodeIDs;
            for (int id : ids)
                nodeIDs.append(nodeMapping[id]);
            Interface::FITKAbstractElement* ele = Interface::FITKElementFactory::createElement(element->getEleType());
            ele->setNodeID(nodeIDs);
            ele->setEleID(++eleMaxID);
            mesh->appendElement(ele);
            int eleID = ele->getEleID();
            elementMapping.insert(eID, eleID);
        }
        //边界数据转换
        Interface::FITKComponentManager * componentMgr = meshKernel->getComponentManager();
        if (!componentMgr) return;
        for (int i = 0; i < componentMgr->getDataCount(); ++i)
        {
            Interface::FITKModelSet* component = dynamic_cast<Interface::FITKModelSet*>(componentMgr->getDataByIndex(i));
            if (!component || component->getModelSetType() != Interface::FITKModelEnum::FITKModelSetType::FMSElem) continue;
            QList<int> abMember = component->getAbsoluteMember();
            QList<int> ids;
            for (int id : abMember)
                ids.append(elementMapping[id]);
            Interface::FITKModelSet* set = new Interface::FITKModelSet(Interface::FITKModelEnum::FITKModelSetType::FMSElem);
            set->setDataObjectName(component->getDataObjectName());
            set->setAbsoluteMember(ids);
            setList.append(set);
        }
    }

    void ExportCGNSWriteThread::convertStructuredMeshToCGNS(ModelData::MeshKernel * meshKernel, Interface::FITKUnstructuredMesh * mesh, QList<Interface::FITKModelSet*>& setList)
    {
    }

}

