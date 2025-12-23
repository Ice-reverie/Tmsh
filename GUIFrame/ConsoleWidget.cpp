#include "ConsoleWidget.h"
#include "ui_ConsoleWidget.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKComponents.h"
#include "FITK_Kernel/FITKAppFramework/FITKComponentInterface.h"

namespace GUI
{
    ConsoleWidget::ConsoleWidget(QWidget* parent /*= nullptr*/)
        :QTabWidget(parent)
    {
        //创建UI
        _ui = new Ui::ConsoleWidget;
        _ui->setupUi(this);
        //创建组件UI
        auto conWidgetComp = FITKAPP->getComponents()->getComponentByName("MessageConsole");
        if (conWidgetComp != nullptr)
        {
            _ui->MessageLayout->addWidget(conWidgetComp->getWidget(0));
        }
        

    }
    ConsoleWidget::~ConsoleWidget()
    {

    }
}


 

