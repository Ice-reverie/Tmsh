#include "GraphInteractionOperator.h"

namespace EventOper
{
    void GraphInteractionOperator::picked(Comp::FITKGraph3DWindowVTK* graphWindow, vtkActor2D* actor)
    {
        // 根据不同子类重写。
        Q_UNUSED(graphWindow);
        Q_UNUSED(actor);
    }

    void GraphInteractionOperator::picked(Comp::FITKGraph3DWindowVTK* graphWindow, vtkImageActor* actor)
    {
        // 根据不同子类重写。
        Q_UNUSED(graphWindow);
        Q_UNUSED(actor);
    }

    void GraphInteractionOperator::picked(Comp::FITKGraph3DWindowVTK* graphWindow, vtkActor* actor, int index, double* pickedWorldPos)
    {
        // 根据不同子类重写。
        Q_UNUSED(graphWindow);
        Q_UNUSED(actor);
        Q_UNUSED(index);
        Q_UNUSED(pickedWorldPos);
    }

    void GraphInteractionOperator::picked(Comp::FITKGraph3DWindowVTK* graphWindow, QList<vtkActor*> actors, vtkPlanes* pickPlane)
    {
        // 根据不同子类重写。
        Q_UNUSED(graphWindow);
        Q_UNUSED(actors);
        Q_UNUSED(pickPlane);
    }

    void GraphInteractionOperator::doublePicked(vtkActor* actor, int index)
    {
        // 根据不同子类重写。
        Q_UNUSED(actor);
        Q_UNUSED(index);
    }

    void GraphInteractionOperator::doublePicked(vtkActor2D* actor)
    {
        // 根据不同子类重写。
        Q_UNUSED(actor);
    }

    void GraphInteractionOperator::doublePicked(vtkImageActor* actor)
    {
        // 根据不同子类重写。
        Q_UNUSED(actor);
    }

    void GraphInteractionOperator::clear(Comp::FITKGraph3DWindowVTK* graphWindow)
    {
        // 根据不同子类重写。
        Q_UNUSED(graphWindow);
    }

    void GraphInteractionOperator::setPickedColor(QColor color)
    {
        // 根据不同子类重写。
        Q_UNUSED(color);
    }

    void GraphInteractionOperator::setActorStateByPickInfo(int pickObjType, int pickMethod, int dataObjId)
    {
        // 根据不同子类重写。
        Q_UNUSED(pickObjType);
        Q_UNUSED(pickMethod);
        Q_UNUSED(dataObjId);
    }
}  // namespace EventOper
