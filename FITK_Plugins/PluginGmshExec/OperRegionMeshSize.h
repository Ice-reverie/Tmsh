/**********************************************************************
 * @file   OperRegionMeshSize.h
 * @brief  区域网格尺寸操作器
 * @author guqingtao (15598887859@163.com)
 * @date   2025-06-16
 *********************************************************************/
#ifndef __OPERREGIONMESHSIZE____
#define __OPERREGIONMESHSIZE____

#include "FITK_Kernel/FITKCore/FITKActionOperator.h"
#include "FITK_Kernel/FITKCore/FITKOperatorRepo.h"

namespace ModelOper
{
    class   OperRegionMeshSize :public Core::FITKActionOperator
    {
        Q_OBJECT;
    public:
        /**
         * @brief    区域网格尺寸操作器
         * @return   void
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-06-16
         */
        explicit OperRegionMeshSize() = default;
        virtual ~OperRegionMeshSize() = default;

        /**
         * @brief 执行界面操作
         * @return true     执行成功
         * @return false     执行失败
         * @author guqingtao (15598887859@163.com)
         * @date 2025-06-16
         */
        virtual bool execGUI() override;
        /**
         * @brief 执行读入操作
         * @return true     执行成功
         * @return false     执行失败
         * @author guqingtao (15598887859@163.com)
         * @date 2025-06-16
         */
        virtual bool execProfession() override;

    private:
        bool updateMeshSizeBox();
        bool updateMeshSizeCylinder();
        bool updateMeshSizeSphere();
        bool updateMeshInfo();

    };
}
#endif