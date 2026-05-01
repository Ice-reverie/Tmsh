/**********************************************************************
 * @file   OperatorExportGmshFile.h
 * @brief  导出Gmsh网格文件操作器
 * @author An
 * @date   2026-04-29
 *********************************************************************/
#ifndef _OPERATOREXPORTGMSHFILE_H
#define _OPERATOREXPORTGMSHFILE_H

#include "FITK_Kernel/FITKCore/FITKActionOperator.h"
#include "FITK_Kernel/FITKCore/FITKOperatorRepo.h"
#include "OperatorsModelAPI.h"
#include "FITK_Kernel/FITKCore/FITKThreadTask.h"
#include <QHBoxLayout>

namespace Interface
{
    class FITKUnstructuredMesh;
    class FITKModelSet;
}

namespace ModelData
{
    class MeshKernel;
}

namespace ModelOper
{
    /**
     * @brief  导出Gmsh网格文件操作器
     * @author CodeArts
     * @date   2025-04-29
     */
    class OperatorExportGmshFile : public Core::FITKActionOperator
    {
        Q_OBJECT;
    public:
        explicit OperatorExportGmshFile();
        virtual ~OperatorExportGmshFile();
        virtual bool execGUI();
        virtual bool execProfession();
    private:
        QHBoxLayout* getKernelMesh(QWidget* parent);
    };

    /**
     * @brief  导出Gmsh文件写出线程
     * @author CodeArts
     * @date   2025-04-29
     */
    class ExportGmshWriteThread : public Core::FITKThreadTask
    {
        Q_OBJECT;
    public:
        ExportGmshWriteThread() = default;
        ~ExportGmshWriteThread() = default;
        void run();
    signals:
        void sigExportFinish();
    private:
        bool convertMeshKernelToGmsh(ModelData::MeshKernel* meshKernel, Interface::FITKUnstructuredMesh* mesh, QList<Interface::FITKModelSet*>& setList);
        void convertUnstructuredMeshToGmsh(ModelData::MeshKernel* meshKernel, Interface::FITKUnstructuredMesh* mesh, QList<Interface::FITKModelSet*>& setList);
        bool writeGmshFile(const QString& fileName, Interface::FITKUnstructuredMesh* mesh, const QList<Interface::FITKModelSet*>& setList);
    public:
        QString _fileName = "";
        QList<int> _kernelIDs{};
    };

    Register2FITKOperatorRepo(actionExportGmshFile, OperatorExportGmshFile);
}

#endif
