#ifndef _FITKOCCMODEL_IMPORT_H___
#define _FITKOCCMODEL_IMPORT_H___

#include "FITKAbstractOCCModel.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoModelImport.h"


namespace OCC
{
    class FITKGeoCompOCCAPI FITKOCCModelImport :public Interface::FITKAbsGeoModelImport,
        public OCCShapeAgent
    {
    public:
        explicit FITKOCCModelImport();
        virtual ~FITKOCCModelImport() = default;

        /**
         * @brief 更新模型
         * @return true 更新成功
         * @return false 更新失败
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-06-05
         */
        virtual bool update() override;

    };
}


#endif
