/**
 *
 * @file FITKAbstractViewAdaptor.h
 * @brief 显示对象适配器抽象类
 * @author LiBaojun (libaojunqd@foxmail.com)
 * @date 2024-03-07
 *
 */
#ifndef __FITKABSTRACT_VIEW_ADAPTOR_H___
#define __FITKABSTRACT_VIEW_ADAPTOR_H___

#include "FITKAbstractAdaptor.h"


namespace Adaptor
{
    /**
     * @brief 显示对象适配器抽象类
     * @author LiBaojun (libaojunqd@foxmail.com)
     * @date 2024-03-07
     */
    class FITKAdaptorAPI FITKAbstractViewAdaptor : public FITKAbstractAdaptor
    {
        FITKCLASS(Adaptor, FITKAbstractViewAdaptor);
    public:
        /**
         * @brief Construct a new FITKAbstractViewAdaptor object
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-07
         */
        explicit FITKAbstractViewAdaptor() = default;
        /**
         * @brief Destroy the FITKAbstractViewAdaptor object
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-07
         */
        virtual ~FITKAbstractViewAdaptor() = 0;
        /**
         * @brief 获取数据类名称
         * @return QString
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-07
         */
        virtual QString getAdaptorClass() override;
        /**
         * @brief 获取适配器类型
         * @return FITKAdaptorType
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-07
         */
        virtual FITKAdaptorType getAdaptorType() override;

    };
}

#endif
