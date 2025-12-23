#include "RenderWidget.h"

#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKComponents.h"
#include "FITK_Kernel/FITKAppFramework/FITKComponentInterface.h"
#include "FITK_Component/FITKRenderWindowVTK/FITKGraph3DWindowVTK.h"

#include <QGridLayout>

namespace GUI
{
    RenderWidget::RenderWidget(QWidget* parent /*= nullptr*/)
    {
        //布局
        QGridLayout* lay = new QGridLayout(this);
        lay->setMargin(0);
        this->setLayout(lay);
        //组件中创建窗口
        AppFrame::FITKComponentInterface *graph3DCompBase =
            FITKAPP->getComponents()->getComponentByName("Graph3DWindowVTK");
        if (graph3DCompBase)
        {
            _renderWin = graph3DCompBase->getWidget(1);
            lay->addWidget(_renderWin);

            Comp::FITKGraph3DWindowVTK* renderWin = dynamic_cast<Comp::FITKGraph3DWindowVTK*>(_renderWin);
            if (renderWin) {
                renderWin->setIsShowActions(true);
                renderWin->setIsShowClipAction(true);
                renderWin->SetGlobalWarningDisplay(false);
            }
        }
    }
    RenderWidget::~RenderWidget()
    {
        if (_renderWin) {
            delete _renderWin;
            _renderWin = nullptr;
        }
    }

    QWidget* RenderWidget::getRenderWin()
    {
        return _renderWin;
    }
}






