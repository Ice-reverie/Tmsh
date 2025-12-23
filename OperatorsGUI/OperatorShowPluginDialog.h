/**********************************************************************
@file   OperatorShowPluginDialog.h
@brief  显示插件对话框操作器
@author ZhouZhaoYi (zhouzhaoyi@diso.cn)
@date   2025-03-07
*********************************************************************/
#ifndef _OPERATORSHOWPLUGINDIALOG_H__
#define _OPERATORSHOWPLUGINDIALOG_H__

#include "FITK_Kernel/FITKCore/FITKActionOperator.h"
#include "FITK_Kernel/FITKCore/FITKOperatorRepo.h"
#include "OperatorsGUIAPI.h"


namespace Core {
    /**
     * @brief  显示插件对话框操作符
     * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
     * @date   2025-03-07
     */
    class OperatorsGUIAPI OperatorShowPluginDialog : public FITKActionOperator
    {
    public:
        /**
         * @brief    构造函数
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2025-03-07
         */
        explicit OperatorShowPluginDialog();
        /**
         * @brief    析构函数
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2025-03-07
         */
        virtual ~OperatorShowPluginDialog();
        /**
         * @brief    执行GUI操作
         * @return   bool         是否成功
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2025-03-07
         */
        bool execGUI() override;

        /**
         * @brief    执行专业操作
         * @return   bool         是否成功
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2025-03-07
         */
        bool execProfession() override;


    };
    // 注册操作类到操作符仓库
    Register2FITKOperatorRepo(actionPlugin, OperatorShowPluginDialog);
} // namespace Core
#endif // SHOWPLUGINDIALOGOPERATOR_H_