/**********************************************************************
 * @file   FITKMesherDriverGmshExecExec.h
 * @brief  Gmsh Exec网格驱动器
 * @author liuzhonghua (liuzhonghuaszch@163.com)
 * @date   2024-10-15
 *********************************************************************/
#ifndef __FITKMESHERDRIVERGMSHEXEC_H___
#define __FITKMESHERDRIVERGMSHEXEC_H___

#include "FITK_Interface/FITKInterfaceMeshGen/FITKAbstractMesherDriver.h"
#include "FITK_Kernel/FITKCore/FITKThreadTask.h"

class TopoDS_Shape;
class QTimer;

namespace Interface
{
    class FITKAbstractRegionMeshSize;
}

namespace Core
{
    class FITKAbstractDataObject;
}

namespace Gmsh
{
    /**
     * @brief Gmsh网格驱动器
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date 2024-10-15
     */
    class FITKMesherDriverGmshExec : public Interface::FITKAbstractMesherDriver
    {
        Q_OBJECT
    public:
        /**
         * @brief Construct a new FITKMesherDriverGmshExec object
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date 2024-10-15
         */
        explicit FITKMesherDriverGmshExec();
        /**
         * @brief Destroy the FITKMesherDriverGmshExec object
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date 2024-10-15
         */
        virtual ~FITKMesherDriverGmshExec();
        /**
         * @brief 启动网格划分程序
         * @param[i]  info           附加信息
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date 2024-07-18
         */
        virtual void startMesher(QStringList info = QStringList()) override;
        /**
         * @brief 结束网格划分程序
         * @param[i]  info           附加信息
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date 2024-07-18
         */
        virtual void stopMesher(QStringList info = QStringList()) override;

    protected:
        /**
         * @brief    进行gmsh网格生成设置-弹出网格设置窗口
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-10-17
         */
        void gmshMeshGenSetting();
        /**
         * @brief    开始驱动Gmsh进行网格划分
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-10-17
         */
        void startGmshMeshGenerationDriven();

    private slots:
        /**
         * @brief  写出几何文件模型与geo脚本文件
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-10-20
         */
        void writerModelGeoScriptFileDriven();
    };
}


#endif
