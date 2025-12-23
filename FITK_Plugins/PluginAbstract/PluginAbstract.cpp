#include "PluginAbstract.h"
#include "PluginActionOpersManager.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Kernel/FITKCore/FITKOperatorRepo.h"
#include "FITK_Kernel/FITKCore/FITKAbstractOperator.h"
#include "GUIFrame/MainWindow.h"

namespace Plugin
{

    PluginAbstract::PluginAbstract(QLibrary* lib) : AppFrame::FITKAbstractPlugin(lib)
    { 
        _actOpersMgr = new PluginActionOpersManager(this);
    }

    PluginAbstract::~PluginAbstract()
    {
        if (_actOpersMgr != nullptr) delete _actOpersMgr;
    }


    void PluginAbstract::unInstall()
    {
        if (_actOpersMgr) _actOpersMgr->removeAllPages();
    }

    QString PluginAbstract::PluginKey()
    {
        return QString("FastCAE_Mesh");
    }

     GUI::MainWindow* PluginAbstract::getMainWindow()
    { 
        return FITKAPP->getGlobalData()->getMainWindowT<GUI::MainWindow>();
    }

     const QStringList& PluginAbstract::getRegisteredOperators() const
     {
         return _registeredOperators;
     }

     void PluginAbstract::addOperator(const QString& operatorName, std::function<Core::FITKAbstractOperator*()> createFun)
     {
         if (createFun == nullptr || operatorName.isEmpty() || _registeredOperators.contains(operatorName))
         {
             return;
         }

         _registeredOperators.append(operatorName);

         // 注册到操作器仓库
         Core::FITKOperatorRepo::getInstance()->registerOperatorFunction(operatorName, createFun);
     }

     void PluginAbstract::clearOperators()
     {
         // 从操作器仓库中移除所有注册的操作器
         for (const QString& operatorName : _registeredOperators)
         {
             Core::FITKOperatorRepo::getInstance()->removeOperator(operatorName);
         }

         _registeredOperators.clear();

     }

}



