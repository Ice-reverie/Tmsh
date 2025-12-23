/**********************************************************************
 * @file   FITKGmshAdaptorObject.h
 * @brief  Gmsh 适配器 用于读取msh文件和写出msh文件
 * @author liuzhonghua (liuzhonghuaszch@163.com)
 * @date   2024-11-29
 *********************************************************************/
#ifndef __FITKGMSHADAPTOROBJECT_R_H__
#define __FITKGMSHADAPTOROBJECT_R_H__

#include "FITKGmshAbstractAdaptor.h"
#include "FITK_Kernel/FITKAdaptor/FITKIOAdaptorFactory.h"

ForwardDeclarNS(Interface, FITKComponentManager)

namespace Gmsh
{
    /**
     * @brief  gmsh msh网格文件读取
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date   2024-11-29
     */
    class FITKGmshAdaptorObject : public FITKGmshAbstractAdaptor
    {
    public:
        explicit FITKGmshAdaptorObject() = default;
        virtual ~FITKGmshAdaptorObject() = default;

        /**
         * @brief    获取适配器数据类型名
         * @return   QString
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-11-29
         */
        QString getAdaptorClass() override;

        /**
         * @brief    适配器读取
         * @return   状态 true成功， false失败
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-11-29
         */
        bool adaptR() override;

        /**
         * @brief    适配器写出
         * @return   状态 true成功， false失败
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-11-29
         */
        bool adaptW() override;

        /**
         * @brief    设置集合管理器
         * @param[i] componentManager
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-12-06
         */
        void setComponentManager(Interface::FITKComponentManager* componentManager);

    private:
        /**
         * @brief    读取版本信息
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-11-29
         */
        bool readVersionInfo();
        /**
         * @brief    读取节点信息
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-11-29
         */
        bool readNodes();
        /**
         * @brief    读取单元信息
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-11-29
         */
        bool readElements();

    private:
        /**
         * @brief  集合管理
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-12-06
         */
        Interface::FITKComponentManager* m_componentMgr{};
        /**
         * @brief  版本信息
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-11-29
         */
        QString m_versionInfo{"4.1 0 8"};

        
    };

    Register2FITKIOAdaptorFactory(Gmsh, FITKGmshAdaptorObject, FITKGmshAdaptorObject)
}


#endif
