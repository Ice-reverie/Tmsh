/**
 * 
 * @file PluginAbstract.h
 * @brief 插件抽象类声明
 * @author LiBaojun (libaojunqd@foxmail.com)
 * @date 2025-03-06
 * 
 */
#ifndef _PLUGIN_AHBSTRACT_H___
#define _PLUGIN_AHBSTRACT_H___
 
#include "PluginAbstractAPI.h"
#include "FITK_Kernel/FITKAppFramework/FITKAbstractPlugin.h"

namespace Core
{
    class FITKAbstractOperator;
}

namespace GUI
{
    class MainWindow;
}

namespace Plugin
{
    class PluginActionOpersManager;
    /**
     * @brief 插件抽象类
     * @author LiBaojun (libaojunqd@foxmail.com)
     * @date 2025-03-06
     */
    class PluginAbstractAPI PluginAbstract : public AppFrame::FITKAbstractPlugin
    {
    public:
        explicit PluginAbstract(QLibrary* lib);
        /**
         * @brief Destroy the Plugin Abstract object
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2025-03-06
         */
        virtual ~PluginAbstract() = 0;
        /**
         * @brief 卸载插件
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2025-03-06
         */
        virtual void unInstall() override;
        /**
         * @brief 插件密钥
         * @return QString
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2025-03-06
         */
        static QString PluginKey();

        /**
         * @brief 获取主页面
         * @return GUI::MainWindow* 
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2025-03-06
         */
         GUI::MainWindow* getMainWindow();

         /**
          * @brief 获取已注册的操作器名称列表
          * @return const QStringList& 操作器名称列表
          * @author libaojun
          * @date 2025-03-31
          */
         const QStringList& getRegisteredOperators() const;

         /**
          * @brief 添加操作器名称和创建函数
          * @param operatorName 操作器名称
          * @param createFun 创建操作器的函数指针
          * @author libaojun
          * @date 2025-03-31
          */
         void addOperator(const QString& operatorName, std::function<Core::FITKAbstractOperator*()> createFun);

         /**
          * @brief 清空操作器名称列表
          * @author libaojun
          * @date 2025-03-31
          */
         void clearOperators();

    protected:
        PluginActionOpersManager* _actOpersMgr{};
        QStringList _registeredOperators;  ///< 已注册的操作器名称列表
    
    };


 
}


#endif
