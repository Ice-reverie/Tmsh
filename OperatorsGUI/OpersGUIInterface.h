#ifndef _OPERGUI_INTERFACE__H___
#define _OPERGUI_INTERFACE__H___

#include "FITK_Kernel/FITKAppFramework/FITKComponentInterface.h"
#include "OperatorsGUIAPI.h"

namespace GUIOper
{
    /* @brief 模型相关操作器注册，主要用于加载dll
     * @author LiBaojun (libaojunqd@foxmail.com)
     * @date 2024-04-14
     */
    class OperatorsGUIAPI OperGUIInterface :
        public AppFrame::FITKComponentInterface
    {
    public:
        /**
         * @brief Construct a new Oper G U I Interface object
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-09-24
         */
        explicit OperGUIInterface() = default;
        virtual ~OperGUIInterface() = default;
        /**
         * @brief 获取部件名称，不能重复
         * @return QString
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-04
         */
        virtual QString getComponentName();

    };
}


#endif
