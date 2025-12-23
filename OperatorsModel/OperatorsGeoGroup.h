/**********************************************************************
 * @file   OperatorsGeoGroup.h
 * @brief  几何分组操作器
 * @author liuzhonghua (liuzhonghuaszch@163.com)
 * @date   2025-04-16
 *********************************************************************/
#ifndef _OPERATORSOPEN_H
#define _OPERATORSOPEN_H

#include "FITK_Kernel/FITKCore/FITKActionOperator.h"
#include "FITK_Kernel/FITKCore/FITKOperatorRepo.h"

namespace Interface
{
    class FITKAbsGeoCommand;
}

namespace ModelOper
{
    /**
     * @brief  打开工程文件操作器
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date   2025-04-11
     */
    class OperatorsGeoGroup :public Core::FITKActionOperator
    {
        Q_OBJECT;
    public:
        /**
         * @brief    Construct a new Operators GeoGroup object
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-04-11
         */
        explicit OperatorsGeoGroup() = default;
        /**
         * @brief    Destroy the Operators GeoGroup object
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-04-11
         */
        virtual ~OperatorsGeoGroup() = default;
        /**
         * @brief    执行界面操作
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-04-11
         */
        virtual bool execGUI() override;
        /**
         * @brief    执行读入操作
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-04-11
         */
        virtual bool execProfession() override;

    private:
        /**
         * @brief    获取几何命令数据
         * @return   Interface::FITKAbsGeoCommand *
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-04-16
         */
        Interface::FITKAbsGeoCommand* getGeometryCommand();
    };

    // 按钮注册相关操作
    Register2FITKOperatorRepo(actionTreeGroupModel, OperatorsGeoGroup);
}

#endif