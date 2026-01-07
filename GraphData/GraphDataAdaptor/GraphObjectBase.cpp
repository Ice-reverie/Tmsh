#include "GraphObjectBase.h"

#include "FITK_Kernel/FITKCore/FITKAbstractDataObject.h"

#include <vtkProp.h>


namespace Graph
{
    GraphObjectBase::GraphObjectBase(Core::FITKAbstractDataObject * dataObj) :
        Comp::FITKGraphObjectVTK(dataObj)
    {

    }

    GraphObjectBase::~GraphObjectBase()
    {

    }

    void GraphObjectBase::update(bool forceUpdate)
    {
        Q_UNUSED(forceUpdate);
    }

    bool GraphObjectBase::getVisibility()
    {
        if (_dataObj == nullptr) {
            return false;
        }
        return _dataObj->isEnable();
    }

    void GraphObjectBase::updateVisibility()
    {

    }

    void GraphObjectBase::updateSubColor(QColor color, QVariant otherPar)
    {
        Q_UNUSED(otherPar);
        Q_UNUSED(color);
    }

    void GraphObjectBase::updateModelColor(QColor color)
    {
        Q_UNUSED(color);
    }

    int GraphObjectBase::getRenderLayer()
    {
        return _renderLayer;
    }

    int GraphObjectBase::getDataId()
    {
        if (_dataObj == nullptr) {
            return -1;
        }
        return _dataObj->getDataObjectID();
    }

    bool GraphObjectBase::contains(vtkProp * actor)
    {
        return m_actorList.contains(actor);
    }

    vtkDataSet * GraphObjectBase::getMesh(ShapeType type)
    {
        Q_UNUSED(type);
        return nullptr;
    }

    int GraphObjectBase::getShapeIdByVTKCellId(int vtkCellId, ShapeAbsEnum topAbsShapeType)
    {
        Q_UNUSED(vtkCellId);
        Q_UNUSED(topAbsShapeType);
        return -1;
    }

    const QVector<int> GraphObjectBase::getVTKCellIdsByShapeId(int shapeId, ShapeAbsEnum topAbsShapeType)
    {
        Q_UNUSED(shapeId);
        Q_UNUSED(topAbsShapeType);
        return QVector<int>();
    }

    void GraphObjectBase::setVisible(bool isVis)
    {
        for (vtkProp* actor : m_actorList) {
            if (actor == nullptr)continue;
            actor->SetVisibility(isVis);
        }
    }

    void GraphObjectBase::setPickModel(bool isPick, ShapeType type)
    {
        Q_UNUSED(isPick);
        Q_UNUSED(type);
    }

    void GraphObjectBase::setRender(Comp::FITKGraphRender* render)
    {
        _render = render;
    }

    void GraphObjectBase::highlight(QVariant otherValue)
    {
        Q_UNUSED(otherValue);
    }

    void GraphObjectBase::disHighlight(QVariant otherValue)
    {
        Q_UNUSED(otherValue);
    }
}