#include "OperatorExportGmshFile.h"

#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Kernel/FITKAppFramework/FITKComponents.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppSettings.h"
#include "FITK_Kernel/FITKCore/FITKThreadPool.h"
#include "FITK_Kernel/FITKCore/FITKVec3D.h"
#include "FITK_Kernel/FITKAppFramework/FITKMessage.h"

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
#include <QFile>
#include <QTextStream>

namespace ModelOper
{
    OperatorExportGmshFile::OperatorExportGmshFile()
    {
    }

    OperatorExportGmshFile::~OperatorExportGmshFile()
    {
    }

    bool OperatorExportGmshFile::execGUI()
    {
        QString wk;
        auto settings = FITKAPP->getAppSettings();
        if (settings) wk = settings->getWorkingDir();

        QFileDialog dlg(FITKAPP->getGlobalData()->getMainWindow(), QObject::tr("SaveAs Gmsh Mesh File"), wk, "Gmsh Mesh(*.msh)");
        dlg.setAcceptMode(QFileDialog::AcceptSave);
        dlg.setFileMode(QFileDialog::AnyFile);
        dlg.setOption(QFileDialog::DontUseNativeDialog);

        QList<QUrl> urls = dlg.sidebarUrls();
#ifdef Q_OS_WIN
        urls.append(QUrl::fromLocalFile(QStandardPaths::writableLocation(QStandardPaths::DesktopLocation)));
        urls.append(QUrl::fromLocalFile(QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation)));
        urls.append(QUrl::fromLocalFile(QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation)));
#endif
        dlg.setSidebarUrls(urls);

        QVBoxLayout* mainlayout = dlg.findChild<QVBoxLayout*>();
        if (mainlayout)
            mainlayout->addLayout(this->getKernelMesh(&dlg));

        dlg.show();

        bool accept = false;
        QEventLoop loop;
        connect(&dlg, &QFileDialog::accepted, [&] {loop.quit(); accept = true;  });
        connect(&dlg, &QFileDialog::rejected, [&] {loop.quit(); accept = false;  });
        loop.exec();
        if (!accept) return false;

        QStringList files = dlg.selectedFiles();
        if (files.size() < 1) return false;
        QString filePath = files.at(0);
        if (filePath.isEmpty()) return false;

        if (!filePath.toLower().endsWith(".msh")) filePath += ".msh";
        this->setArgs("FileName", filePath);
        return true;
    }

    bool OperatorExportGmshFile::execProfession()
    {
        QString fileName;
        if (!this->argValue<QString>("FileName", fileName)) return false;
        int kernelID = 0;
        if (!this->argValue<int>("KernelID", kernelID)) return false;

        Core::FITKThreadPool* pool = Core::FITKThreadPool::getInstance();
        if (pool == nullptr) return false;

        ExportGmshWriteThread* exportThread = new ExportGmshWriteThread();
        exportThread->_fileName = fileName;
        exportThread->_kernelIDs.append(kernelID);
        pool->execTask(exportThread);
        this->clearArgs();
        return true;
    }

    QHBoxLayout* OperatorExportGmshFile::getKernelMesh(QWidget* parent)
    {
        ModelData::MeshManager* managerMesh = FITKAPP->getGlobalData()->getMeshData<ModelData::MeshManager>();
        if (managerMesh == nullptr) return nullptr;

        QHBoxLayout* layout = new QHBoxLayout(parent);
        layout->addWidget(new QLabel(tr("Mesh:"), parent));
        QComboBox* comboBox = new QComboBox(parent);
        layout->addWidget(comboBox);

        const int dataCount = managerMesh->getDataCount();
        for (int i = 0; i < dataCount; ++i)
        {
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
        QObject::connect(comboBox, QOverload<int>::of(&QComboBox::activated), [this, comboBox]() {
            int kernelID = comboBox->currentData().toInt();
            this->setArgs("KernelID", kernelID);
        });
        return layout;
    }

    void ExportGmshWriteThread::run()
    {
        ModelData::MeshManager* managerMesh = FITKAPP->getGlobalData()->getMeshData<ModelData::MeshManager>();
        if (managerMesh == nullptr) return;

        for (int i = 0; i < managerMesh->getDataCount(); ++i)
        {
            ModelData::MeshData* meshData = managerMesh->getDataByIndex(i);
            if (meshData == nullptr) continue;

            for (int j = 0; j < meshData->getDataCount(); ++j)
            {
                ModelData::MeshKernel* meshKernel = meshData->getDataByIndex(j);
                if (meshKernel == nullptr) continue;
                if (!_kernelIDs.contains(meshKernel->getDataObjectID())) continue;

                Interface::FITKUnstructuredMesh* mesh = new Interface::FITKUnstructuredMesh;
                QList<Interface::FITKModelSet*> setList;

                bool success = convertMeshKernelToGmsh(meshKernel, mesh, setList);
                if (success)
                {
                    writeGmshFile(_fileName, mesh, setList);
                }

                delete mesh;
                for (Interface::FITKModelSet* set : setList)
                {
                    if (set) delete set;
                }
            }
        }
        emit sigExportFinish();
    }

    bool ExportGmshWriteThread::convertMeshKernelToGmsh(ModelData::MeshKernel* meshKernel, Interface::FITKUnstructuredMesh* mesh, QList<Interface::FITKModelSet*>& setList)
    {
        if (meshKernel == nullptr || mesh == nullptr) return false;
        mesh->setDataObjectName(meshKernel->getDataObjectName());

        Interface::FITKAbstractMesh* dataModelMesh = meshKernel->getMesh();
        if (dataModelMesh == nullptr) return false;

        Interface::FITKModelEnum::AbsModelType modelType = dataModelMesh->getAbsModelType();
        switch (modelType)
        {
        case Interface::FITKModelEnum::AbsModelType::AMTunstructuredMesh:
        case Interface::FITKModelEnum::AbsModelType::AMTunstructuredMeshvtk:
            convertUnstructuredMeshToGmsh(meshKernel, mesh, setList);
            break;
        default:
            break;
        }
        return true;
    }

    void ExportGmshWriteThread::convertUnstructuredMeshToGmsh(ModelData::MeshKernel* meshKernel, Interface::FITKUnstructuredMesh* mesh, QList<Interface::FITKModelSet*>& setList)
    {
        if (!meshKernel) return;

        Interface::FITKUnstructuredMesh* unstructuredMesh = dynamic_cast<Interface::FITKUnstructuredMesh*>(meshKernel->getMesh());
        if (unstructuredMesh == nullptr) return;

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

        Interface::FITKComponentManager* componentMgr = meshKernel->getComponentManager();
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

    bool ExportGmshWriteThread::writeGmshFile(const QString& fileName, Interface::FITKUnstructuredMesh* mesh, const QList<Interface::FITKModelSet*>& setList)
    {
        if (mesh == nullptr) return false;

        QFile file(fileName);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            AppFrame::FITKMessageError(QObject::tr("Failed to open file: %1").arg(fileName));
            return false;
        }

        QTextStream out(&file);
        out.setRealNumberPrecision(15);

        out << "$MeshFormat\n";
        out << "2.2 1 8\n";
        out << "$EndMeshFormat\n";

        out << "$Nodes\n";
        out << mesh->getNodeCount() << "\n";
        for (int i = 0; i < mesh->getNodeCount(); ++i)
        {
            Core::FITKNode* node = mesh->getNodeAt(i);
            if (node)
                out << node->getNodeID() << " " << node->x() << " " << node->y() << " " << node->z() << "\n";
        }
        out << "$EndNodes\n";

        out << "$Elements\n";
        out << mesh->getElementCount() << "\n";
        for (int i = 0; i < mesh->getElementCount(); ++i)
        {
            Interface::FITKAbstractElement* ele = mesh->getElementAt(i);
            if (ele == nullptr) continue;

            int gmshType = 0;
            Interface::FITKModelEnum::FITKEleType eleType = ele->getEleType();
            switch (eleType)
            {
            case Interface::FITKModelEnum::FITKEleType::Line2:   gmshType = 1; break;
            case Interface::FITKModelEnum::FITKEleType::Tri3:    gmshType = 2; break;
            case Interface::FITKModelEnum::FITKEleType::Quad4:   gmshType = 3; break;
            case Interface::FITKModelEnum::FITKEleType::Tet4:    gmshType = 4; break;
            case Interface::FITKModelEnum::FITKEleType::Hex8:    gmshType = 5; break;
            case Interface::FITKModelEnum::FITKEleType::Wedge6:  gmshType = 6; break;
            default: gmshType = 0; break;
            }

            if (gmshType == 0) continue;

            QList<int> nodeIDs = ele->getAllNodes();
            out << ele->getEleID() << " " << gmshType << " 0";
            for (int nid : nodeIDs)
                out << " " << nid;
            out << "\n";
        }
        out << "$EndElements\n";

        file.close();
        AppFrame::FITKMessageNormal(QObject::tr("Gmsh file exported successfully: %1").arg(fileName));
        return true;
    }
}
