#include "FITKGraph3DWidgetPlaneCallBack.h"

#include <vtkImplicitPlaneWidget2.h>
#include <vtkImplicitPlaneRepresentation.h>

namespace Comp
{
    void FITKGraph3DWidgetPlaneCallBack::Execute(vtkObject * caller, unsigned long eventId, void * callData)
    {
        Q_UNUSED(eventId);
        Q_UNUSED(callData);

        vtkImplicitPlaneWidget2* widget = vtkImplicitPlaneWidget2::SafeDownCast(caller);
        if (widget == nullptr)return;
        vtkImplicitPlaneRepresentation* representation = widget->GetImplicitPlaneRepresentation();
        if (representation == nullptr)return;
        double origin[3] = { 0,0,0 };
        double normal[3] = { 0,0,0 };
        representation->GetOrigin(origin);
        representation->GetNormal(normal);
        emit sigValueChange(origin, normal);
    }
}

