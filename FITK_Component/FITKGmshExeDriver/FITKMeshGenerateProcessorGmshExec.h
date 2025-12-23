/**********************************************************************
 * @file   FITKMeshGenerateProcessorGmshExec.h
 * @brief  gmsh网格划分处理器
 * @author liuzhonghua (liuzhonghuaszch@163.com)
 * @date   2024-10-12
 *********************************************************************/
#ifndef _FITKMESHGENERATEPROCESSORGMSHEXEC_H_
#define _FITKMESHGENERATEPROCESSORGMSHEXEC_H_

#include "FITK_Interface/FITKInterfaceMeshGen/FITKAbstractMeshProcessor.h"

namespace Interface
{
    class FITKUnstructuredMesh;
    class FITKComponentManager;
};

namespace Gmsh
{

    /**
     * @brief  gmsh网格划分处理器
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date   2024-10-12
     */
    class FITKMeshGenerateProcessorGmshExec : public Interface::FITKAbstractMeshProcessor
    {
    public:
        /**
         * @brief Construct a new FITKMeshGenerateGmsh object
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-10-12
         */
        explicit FITKMeshGenerateProcessorGmshExec();
        /**
         * @brief    Destroy the FITKMeshGenerateGmsh object
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-10-12
         */
        virtual ~FITKMeshGenerateProcessorGmshExec();

        /**
         * @brief 开始执行处理
         * @param[i]  info           附加信息
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-10-12
         */
        virtual void start(QStringList info = QStringList());
    };
}


#endif // !_FITKMESHGENERATEPROCESSORGMSH_H_
