/**********************************************************************
 * @file   OperatorsGeoRemove.h
 * @brief  几何删除操作器
 * @author liuzhonghua (liuzhonghuaszch@163.com)
 * @date   2025-12-08
 *********************************************************************/
#ifndef OPERATORS_GEO_REMOVE_H
#define OPERATORS_GEO_REMOVE_H

#include "FITK_Kernel/FITKCore/FITKActionOperator.h"
#include "FITK_Kernel/FITKCore/FITKOperatorRepo.h"

class QTreeWidgetItem;

namespace ModelOper
{
    /**
     * @brief  几何删除操作器
     */
    class OperatorsGeoRemove : public Core::FITKActionOperator
    {
        Q_OBJECT;
    public:
        explicit OperatorsGeoRemove() = default;
        virtual ~OperatorsGeoRemove() = default;

        bool execGUI() override;
        bool execProfession() override;

    private:
        /**
         * @brief 获取右键选中的几何ID
         */
        int getSelectedGeoId();

    private:
        int _geoId = -1;
    };

    // 注册到操作器仓库
    Register2FITKOperatorRepo(actionTreeRemoveModel, OperatorsGeoRemove);
}

#endif // OPERATORS_GEO_REMOVE_H
