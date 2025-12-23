/**********************************************************************
 * @file   OperGeometryBox.h
 * @brief  几何体操作器
 * @author guqingtao (15598887859@163.com)
 * @date   2025-04-01
 *********************************************************************/

#ifndef __OPER_GEOMETRY_BOX_____
#define __OPER_GEOMETRY_BOX_____

#include "PluginGeoAbstractAPI.h"

#include "FITK_Kernel/FITKCore/FITKActionOperator.h"
#include "FITK_Kernel/FITKCore/FITKOperatorRepo.h"

namespace ModelOper
{
    class PluginGeoAbstractAPI OperGeometryBox :public Core::FITKActionOperator
    {
        Q_OBJECT;
    public:
        /**
         * @brief    几何体操作器
         * @return   void
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-04-01
         */
        explicit OperGeometryBox() = default;
        virtual ~OperGeometryBox() = default;

        /**
         * @brief 执行界面操作
         * @return true     执行成功
         * @return false     执行失败
         * @author guqingtao (15598887859@163.com)
         * @date 2025-04-01
         */
        virtual bool execGUI() override;
        /**
         * @brief 执行读入操作
         * @return true     执行成功
         * @return false     执行失败
         * @author guqingtao (15598887859@163.com)
         * @date 2025-04-01
         */
        virtual bool execProfession() override;

    private:
        bool createGeometryBox();
        bool createGeometryCylinder();
        bool createGeometrySphere();
        bool createGeometryImprintSolid();
        bool createGeometrySolidSplitter();
    };
}
#endif