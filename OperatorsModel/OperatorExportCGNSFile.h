/**********************************************************************
 * @file   OperatorExportCGNSFile.h
 * @brief  写出CGNS格式模型
 * @author liuzhonghua (liuzhonghuaszch@163.com)
 * @date   2025-08-06
 *********************************************************************/
#ifndef _OPERATOREXPORTCGNSFILE_H
#define _OPERATOREXPORTCGNSFILE_H

#include "FITK_Kernel/FITKCore/FITKOperatorRepo.h"
#include "FITK_Kernel/FITKCore/FITKActionOperator.h"
#include "OperatorsModelAPI.h"
#include "FITK_Component/FITKCGNSIO/FITKCGNSIOInterface.h"
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
     * @brief  写出CGNS格式模型
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date   2025-08-06
     */
    class OperatorExportCGNSFile :public Core::FITKActionOperator
    {
        Q_OBJECT;
    public:
        /**
         * @brief    构造函数
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-08-06
         */
        explicit OperatorExportCGNSFile();
        /**
         * @brief    析构函数
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-08-06
         */
        virtual ~OperatorExportCGNSFile();
        /**
         * @brief    执行界面操作
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-08-06
         */
        virtual bool execGUI();
        /**
         * @brief    执行业务逻辑
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-08-06
         */
        virtual bool execProfession();
    private:
        /**
         * @brief  过滤写出的MeshKernel
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-08-11
         */
        QHBoxLayout* getKernelMesh(QWidget* parent);
    };

    /**
     * @brief  导出CGNS文件写出线程
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date   2025-08-05
     */
    class ExportCGNSWriteThread :public Core::FITKThreadTask
    {
        Q_OBJECT;
    public:
        /**
         * @brief  Construct a new Export CGNS Write Thread object
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-08-05
         */
        ExportCGNSWriteThread() = default;
        /**
         * @brief  Destroy the Export CGNS Write Thread object
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-08-05
         */
        ~ExportCGNSWriteThread() = default;
        /**
         * @brief  执行函数
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-08-05
         */
        void run();
    signals:
        ;
        /**
         * @brief  写出完成信号
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-08-05
         */
        void sigExportFinish();

    private:
        /**
         * @brief    写出CGNS网格预处理
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-07-07
         */
        void writeCGNSMeshPreProcessing(IO::CGNSDataIO& data);
        /**
         * @brief    读取CGNS网格完成处理
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-07-07
         */
        void writeCGNSMeshFinishProcessing();
        /**
         * @brief    数据转换
         * @param[i] meshKernel 
         * @param[i] mesh 
         * @param[i] setList 
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-08-06
         */
        bool convertMeshKernelToCGNS(ModelData::MeshKernel* meshKernel, Interface::FITKUnstructuredMesh* mesh, QList<Interface::FITKModelSet*>& setList);
        /**
         * @brief    处理结构化网格
         * @param[i] meshKernel 
         * @param[i] mesh 
         * @param[i] setList 
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-08-06
         */
        void convertUnstructuredMeshToCGNS(ModelData::MeshKernel* meshKernel, Interface::FITKUnstructuredMesh* mesh, QList<Interface::FITKModelSet*>& setList);
        /**
         * @brief    处理非结构化网格
         * @param[i] meshKernel 
         * @param[i] mesh 
         * @param[i] setList 
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-08-06
         */
        void convertStructuredMeshToCGNS(ModelData::MeshKernel* meshKernel, Interface::FITKUnstructuredMesh* mesh, QList<Interface::FITKModelSet*>& setList);

    public:
        /**
         * @brief  文件名称
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-08-05
         */
        QString _fileName = "";
        /**
         * @brief  写出的mesh数据ID
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-08-11
         */
        QList<int> _kernelIDs{};
    };
    // 按钮注册相关操作
    Register2FITKOperatorRepo(actionExportCGNSFile, OperatorExportCGNSFile);
}

#endif