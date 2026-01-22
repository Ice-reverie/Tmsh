/**********************************************************************
 * @file   FITKTmshAdaptorObject.h
 * @brief  Tmsh 适配器 用于读取msh文件和写出msh文件
 * @author liuzhonghua (liuzhonghuaszch@163.com)
 * @date   2024-11-29
 *********************************************************************/
#ifndef __FITKTMSHADAPTOROBJECT_R_H__
#define __FITKTMSHADAPTOROBJECT_R_H__

#include "FITKTmshAbstractAdaptor.h"
#include "FITK_Kernel/FITKAdaptor/FITKIOAdaptorFactory.h"

ForwardDeclarNS(Interface, FITKComponentManager)

namespace Tmsh
{
    /**
     * @brief  Tmsh msh网格文件读取
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date   2024-11-29
     */
    class FITKTmshAdaptorObject : public FITKTmshAbstractAdaptor
    {
    public:
        explicit FITKTmshAdaptorObject() = default;
        virtual ~FITKTmshAdaptorObject() = default;

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

		bool adaptRT();

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

		bool readTNodes();

        /**
         * @brief    读取单元信息
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-11-29
         */
        bool readElements();

		bool readTElements();
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

    Register2FITKIOAdaptorFactory(Tmsh, FITKTmshAdaptorObject, FITKTmshAdaptorObject)
}


#endif
