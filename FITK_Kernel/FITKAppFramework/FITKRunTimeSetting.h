/**
 *
 * @file FITKRunTimeSetting.h
 * @brief 软件运行时参数设定
 * @author libaojun (libaojunqd@Foxmail.com)
 * @date 2024-04-10
 *
 */
#ifndef __FITKRUNTIME__SETTING_H__
#define __FITKRUNTIME__SETTING_H__

#include "FITKAppFrameworkAPI.h"
#include "FITK_Kernel/FITKCore/FITKVarientParams.h"
namespace AppFrame
{
    /**
     * @brief 软件运行时参数设定
     * @author libaojun (libaojunqd@Foxmail.com)
     * @date 2024-04-10
     */ 
    class  FITKAppFrameworkAPI FITKRunTimeSetting : public Core::FITKVarientParams
    {
    public:
        /**
         * @brief Construct a new FITKRunTimeSetting object
         * @author libaojun (libaojunqd@Foxmail.com)
         * @date 2024-04-10
         */
        explicit FITKRunTimeSetting() = default;
        /**
         * @brief Destroy the FITKRunTimeSetting object
         * @author libaojun (libaojunqd@Foxmail.com)
         * @date 2024-04-10
         */
        virtual ~FITKRunTimeSetting() = default;
      

    };

}



#endif
