#ifndef _MESH_KERNEL_H_
#define _MESH_KERNEL_H_

#include "ModelDataAPI.h"
#include "FITK_Kernel/FITKCore/FITKAbstractNDataObject.h"
#include "FITK_Interface/FITKInterfaceModel/FITKabstractmodel.h"
#include <QString>

namespace Interface
{
    class FITKAbstractMesh;
    class FITKComponentManager;
};
 
namespace ModelData
{
    /**     
     * @file MeshKernel.h
     * @brief 网格内核类，用于处理底层网格操作
     * @details 提供网格数据的基本操作和管理功能，支持2D、3D和混合网格类型，
     *          作为网格数据的核心管理组件，负责网格的创建、修改、查询等操作
     * @author libaojun
     * @date 2025-04-10
     */
    class ModelDataAPI MeshKernel : public Interface::FITKAbstractModel
    {
        FITKCLASS(ModelData, MeshKernel);
        Q_OBJECT
    public:
    /**
     * @enum MeshType
     * @brief 网格类型枚举
     * @details 定义支持的网格维度类型
     */
    enum MeshType
    {
        D2,    ///< 二维网格类型，用于平面问题分析
        D3,    ///< 三维网格类型，用于空间问题分析
        Mixed, ///< 混合网格类型，包含多种维度的网格元素
        // 添加其他网格类型
    };
    Q_ENUM(MeshType)

    public:
        /**         
         * @brief 构造函数
         * @details 初始化网格内核对象，设置默认的网格类型为3D，初始化成员变量
         * @author libaojun
         * @date 2025-04-10
         */
        explicit MeshKernel();
        
        /**         
         * @brief 析构函数
         * @details 清理网格数据和相关资源，释放内存
         * @author libaojun
         * @date 2025-04-10
         */
        virtual ~MeshKernel();
        
        /**
         * @brief 获取模型类型
         * @return Interface::FITKModelEnum::AbsModelType 抽象模型类型
         * @details 返回当前网格内核的抽象模型类型，用于类型识别和转换
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-02-27
         */
        virtual Interface::FITKModelEnum::AbsModelType getAbsModelType() override;;
        
        /**          
         * @brief 设置网格对象
         * @param[in] mesh 网格对象指针，不能为空
         * @details 设置当前网格内核管理的网格对象，替换原有的网格数据
         * @note 调用此方法后，原有的网格对象将被替换，请确保正确管理内存
         * @author libaojun
         * @date 2025-04-10
         */
        void setMesh(Interface::FITKAbstractMesh* mesh);
        
        /** 
         * @brief 获取网格对象
         * @return Interface::FITKAbstractMesh* 网格对象指针，可能为nullptr
         * @details 返回当前网格内核管理的网格对象指针
         * @author libaojun
         * @date 2025-04-10
         */
        Interface::FITKAbstractMesh* getMesh() const;

        /**
         * @brief 获取指定类型的网格对象（模板方法）
         * @tparam T 目标网格类型，必须继承自FITKAbstractMesh
         * @return T* 转换后的网格对象指针，转换失败时返回nullptr
         * @details 使用动态转换获取指定类型的网格对象，提供类型安全的访问方式
         * @author libaojun
         * @date 2025-04-10
         */
        template<typename T>
        T* getMeshT() const
        {
            return dynamic_cast<T*>(_mesh);
        }

        /**         
         * @brief 获取网格类型
         * @return MeshType 当前网格的类型（D2、D3或Mixed）
         * @details 返回当前网格内核的网格类型，用于确定网格的维度特性
         * @author libaojun
         * @date 2025-04-10
         */
        MeshType getMeshType() const;

        /**         
         * @brief 设置网格类型
         * @param[in] type 要设置的网格类型
         * @details 设置网格内核的类型，影响后续的网格操作和渲染方式
         * @author libaojun
         * @date 2025-04-10
         */
        void setMeshType(MeshType type);

        /**         
         * @brief 获取组件管理器
         * @return Interface::FITKComponentManager* 组件管理器指针，可能为nullptr
         * @details 返回与当前网格内核关联的组件管理器，用于管理网格相关的组件
         * @author libaojun
         * @date 2025-04-10
         */
        Interface::FITKComponentManager* getComponentManager() override;
        
        /**         
         * @brief 清除网格数据
         * @details 清除当前网格内核中的所有网格数据，重置为初始状态
         * @note 此操作不可逆，请在调用前确保数据已保存
         * @author libaojun
         * @date 2025-04-10
         */
        void clear();

    protected:
        MeshType _meshType{MeshType::D3}; ///< 网格类型，默认为3D网格
        Interface::FITKAbstractMesh* _mesh{}; ///< 网格对象指针，管理具体的网格数据
        Interface::FITKComponentManager* _componentManager{}; ///< 组件管理器指针，管理网格相关组件
    };
}

#endif
