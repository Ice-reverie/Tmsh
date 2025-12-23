/*****************************************************************//**
 * @file    FITKOCCModelNearestPointLine.h
 * @brief   OCC Nearest Point Line Command Class
 * @date    2025-03-27
 * @author  libaojun
 *********************************************************************/

#ifndef __FITKOCCMODELNEARESTPOINTLINE_H__
#define __FITKOCCMODELNEARESTPOINTLINE_H__

#include "FITKAbstractOCCModel.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoModelNearestPointLine.h"

namespace OCC
{
    /**
    * @brief OCC Nearest Point Line Command Class
    * @date 2025-03-27
    * @author libaojun
    */
    class FITKGeoCompOCCAPI FITKOCCModelNearestPointLine :
        public Interface::FITKAbsGeoModelNearestPointLine, public OCCShapeAgent
    {
    public:
        /**
         * @brief Construct a new FITKOCCModelNearestPointLine object
         * @date 2025-03-27
         * @author libaojun
         */
        explicit FITKOCCModelNearestPointLine();
        virtual ~FITKOCCModelNearestPointLine() = default;

        /**
         * @brief 更新函数，重写基类虚函数
         * @return 成功返回 true，失败返回 false
         * @date 2025-03-27
         * @author libaojun
         */
        bool update() override;

    private:
        /**
        * @brief 处理曲线情况
        * @return 成功返回 true，失败返回 false
        * @date 2025-03-27
        * @author libaojun
        */
        bool handleCurve();

        /**
         * @brief 处理曲面情况
         * @return 成功返回 true，失败返回 false
         * @date 2025-03-27
         * @author libaojun
         */
        bool handleSurface();
    };
}

#endif