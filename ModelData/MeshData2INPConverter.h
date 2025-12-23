/**
 * @file MeshData2INPConverter.h
 * @brief 网格数据到Abaqus INP格式转换器
 * @details 提供将内部网格数据转换为Abaqus INP文件格式的功能，支持结构化和非结构化网格的转换
 * @author libaojun
 * @date 2025-08-01
 */

#ifndef __MODEL_DATA_MESH_DATA_2_INP_CONVERTER_H__
#define __MODEL_DATA_MESH_DATA_2_INP_CONVERTER_H__

#include "ModelDataAPI.h"

namespace Interface
{
    class FITKAbstractMesh;
}
namespace AbaqusData
{
    class FITKDataCase;
    class FITKAbaqusPart;
}

namespace ModelData
{
    class MeshManager;
    class MeshKernel;

    /**
     * @class MeshData2INPConverter
     * @brief 网格数据到INP格式转换器
     * @details 负责将内部网格数据结构转换为Abaqus可识别的INP文件格式，
     *          支持多种网格类型的转换，包括结构化网格和非结构化网格
     * @author libaojun
     * @date 2025-08-01
     */
    class ModelDataAPI MeshData2INPConverter
    {
    public:
        /**
         * @brief 构造函数
         * @param[in] mgr 网格管理器指针，用于获取网格数据
         * @param[in] caseData Abaqus算例数据指针，用于存储转换后的数据
         * @details 初始化转换器，建立与网格管理器和Abaqus数据的关联
         * @author libaojun
         * @date 2025-08-01
         */
        explicit MeshData2INPConverter(MeshManager* mgr, AbaqusData::FITKDataCase* caseData);
        
        /**
         * @brief 析构函数
         * @details 清理资源，释放内存
         * @author libaojun
         * @date 2025-08-01
         */
        virtual ~MeshData2INPConverter() = default; 
        
        /**
         * @brief 执行数据转换
         * @return bool 转换是否成功，true表示成功，false表示失败
         * @details 将网格管理器中的所有网格数据转换为Abaqus格式，
         *          包括节点、单元、材料属性等信息的转换
         * @author libaojun
         * @date 2025-08-01
         */
        bool execDataConversion();

    private:
        /**
         * @brief 将网格内核转换为Abaqus部件
         * @param[in] kernel 待转换的网格内核指针
         * @param[out] abaqusPart 转换后的Abaqus部件指针
         * @param[in] index 部件索引，用于命名和标识
         * @return bool 转换是否成功
         * @details 将单个网格内核的数据转换为对应的Abaqus部件格式
         * @author libaojun
         * @date 2025-08-01
         */
        bool convertMeshKernelToAbaqusPart(MeshKernel* kernel, AbaqusData::FITKAbaqusPart* abaqusPart, 
                                            int index);

        /**
         * @brief 将非结构化网格转换为Abaqus部件
         * @param[in] mesh 非结构化网格对象指针
         * @param[out] abaqusPart 转换后的Abaqus部件指针
         * @details 处理非结构化网格（如四面体、六面体等）到Abaqus格式的转换
         * @author libaojun
         * @date 2025-08-01
         */
        void convertUnstructuredMeshToAbaqusPart(Interface::FITKAbstractMesh* mesh, AbaqusData::FITKAbaqusPart* abaqusPart);

        /**
         * @brief 将结构化网格转换为Abaqus部件
         * @param[in] mesh 结构化网格对象指针
         * @param[out] abaqusPart 转换后的Abaqus部件指针
         * @details 处理结构化网格（如规则六面体网格）到Abaqus格式的转换
         * @author libaojun
         * @date 2025-08-01
         */
        void convertStructuredMeshToAbaqusPart(Interface::FITKAbstractMesh* mesh, AbaqusData::FITKAbaqusPart* abaqusPart);
        
    private:
        /**
         * @brief 网格管理器指针
         * @details 用于获取和管理所有网格数据，包括网格内核、边界条件等信息
         * @author libaojun
         * @date 2025-08-01
         */
        ModelData::MeshManager* _meshManager;

        /**
         * @brief Abaqus算例数据指针
         * @details 存储转换后的Abaqus格式数据，包括部件、材料、边界条件等信息
         * @author libaojun
         * @date 2025-08-01
         */
        AbaqusData::FITKDataCase* _caseData;
    };
}

#endif