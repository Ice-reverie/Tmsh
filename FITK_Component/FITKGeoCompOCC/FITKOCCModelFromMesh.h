#ifndef __FITK_OCC_MODEL_FROM_MESH_H__
#define __FITK_OCC_MODEL_FROM_MESH_H__


#include "FITKAbstractOCCModel.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoModelFromMesh.h"

namespace OCC
{
    /**
     * @brief OCC模型从网格生成的类
     * @details 此类提供从网格数据生成几何模型的功能
     * @author libaojun
     * @date 2025-05-09
     */
    class FITKGeoCompOCCAPI FITKOCCModelFromMesh : public Interface::FITKAbsGeoModelFromMesh, 
        public OCCShapeAgent
    {
    public:
        /**
         * @brief Construct a new FITKOCCModelExtractCenter object
         * @date 2025-05-09
         * @author libaojun
         */
        explicit FITKOCCModelFromMesh();
        virtual ~FITKOCCModelFromMesh() = default;

        /**
         * @brief 更新函数，重写基类虚函数
         * @return 成功返回 true，失败返回 false
         * @date 2025-05-09
         * @author libaojun
         */
        bool update() override;

    };
    
} // namespace OCC



#endif //
// __FITK_OCC_MODEL_FROM_MESH_H__