/**********************************************************************
  * @file    OperatorExportAbaqusINPFile.h
  * @brief   导出AbaqusINP文件操作器类
  * @author  lilongyuan (lilongyuan@diso.cn)
  * @date    2025-07-28
  *********************************************************************/
#ifndef __OPERATOR_EXPORT_ABAQUS_INP_FILE_H__
#define __OPERATOR_EXPORT_ABAQUS_INP_FILE_H__

#include "FITK_Kernel/FITKCore/FITKActionOperator.h"
#include "FITK_Kernel/FITKCore/FITKOperatorRepo.h"
#include "FITK_Interface/FITKInterfaceModel/FITKModelEnum.h"
#include "FITK_Interface/FITKInterfaceStructural/FITKAbaMeshPartVTK.h"

namespace AbaqusData
{
    class FITKDataCase;
    class FITKAbaqusPart;
}
namespace Interface
{
    class FITKAbaMeshPartVTK;
    class FITKParticalMesh;
    class FITKUnstructuredMesh;
    class FITKStructuredMesh;
    class FITKUnstructuredMeshVTK;    
}

namespace ModelData
{
    class MeshManager;
    class MeshKernel;
}

namespace ModelOper
{
    /**
    * @brief   导出AbaqusINP文件操作器类
    * @author  lilongyuan (lilongyuan@diso.cn)
    * @date    2025-07-28
    */
    class OperatorExportAbaqusINPFile : public Core::FITKActionOperator
    {
        Q_OBJECT;
    public:
        /**
        * @brief   构造函数
        * @author  lilongyuan (lilongyuan@diso.cn)
        * @date    2025-07-28
        */
        explicit OperatorExportAbaqusINPFile() = default;
        /**
        * @brief   析构函数
        * @author  lilongyuan (lilongyuan@diso.cn)
        * @date    2025-07-28
        */
        virtual ~OperatorExportAbaqusINPFile() = default;
        /**
        * @brief   执行界面操作
        * @return  bool 执行结果
        * @author  lilongyuan (lilongyuan@diso.cn)
        * @date    2025-07-28
        */
        virtual bool execGUI();
        /**
        * @brief   执行业务逻辑
        * @return  bool 执行结果
        * @author  lilongyuan (lilongyuan@diso.cn)
        * @date    2025-07-28
        */
        virtual bool execProfession();
    private slots:
        /**
        * @brief   线程结束信号槽
        * @author  lilongyuan (lilongyuan@diso.cn)
        * @date    2025-07-28
        */
        void ioThreadFinishedSlot();
    private:
        /**
        * @brief   Abaqus算例数据对象
        * @author  lilongyuan (lilongyuan@diso.cn)
        * @date    2025-07-29
        */
        AbaqusData::FITKDataCase* _case{};
    };
    // 按钮注册相关操作
    Register2FITKOperatorRepo(actionExportAbaqusINPFile, OperatorExportAbaqusINPFile);
    
}
#endif  // __OPERATOR_EXPORT_ABAQUS_INP_FILE_H__