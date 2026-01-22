/**********************************************************************
 * @file   FITKTmshAdaptorNodes.h
 * @brief  Tmsh msh 节点读写适配器
 * @author liuzhonghua (liuzhonghuaszch@163.com)
 * @date   2024-11-29
 *********************************************************************/
#ifndef __FITKTMSHADAPTORNODES_R_H__
#define __FITKTMSHADAPTORNODES_R_H__

#include "FITKTmshAbstractAdaptor.h"
#include "FITK_Kernel/FITKAdaptor/FITKIOAdaptorFactory.h"

namespace Tmsh
{
    /**
     * @brief  Tmsh msh 节点读写适配器
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date   2024-11-29
     */
    class FITKTmshAdaptorNodes : public FITKTmshAbstractAdaptor
    {
    public:
        explicit FITKTmshAdaptorNodes() = default;
        virtual ~FITKTmshAdaptorNodes() = default;

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

    private:
        /**
         * @brief    读取所有节点的信息
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-11-29
         */
        bool readNodesInfo();
        /**
         * @brief    读取节点
         * @param[i] info
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-11-29
         */
        bool readNode(QStringList info);

    private:
		int count;
    };

    Register2FITKIOAdaptorFactory(Tmsh, FITKTmshAdaptorNodes, FITKTmshAdaptorNodes)
}


#endif
