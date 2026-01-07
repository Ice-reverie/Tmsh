#include "GraphObjectPick.h"

#include "FITK_Interface/FITKVTKAlgorithm/FITKGraphActor.h"
#include "FITK_Interface/FITKVTKAlgorithm/FITKHighOrderCellFilter.h"

#include <vtkDataSet.h>
#include <vtkMapper.h>
#include <vtkProperty.h>
#include <vtkSelectionNode.h>
#include <vtkSelection.h>
#include <vtkExtractSelection.h>
#include <vtkIdTypeArray.h>
#include <vtkDataSetSurfaceFilter.h>

namespace Graph
{
    GraphObjectPick::GraphObjectPick()
        : GraphObjectBase(nullptr)
    {
        init();
        m_hasFixedBounds = false;
    }

    GraphObjectPick::~GraphObjectPick()
    {
        // Delete pointers.
        deleteVTKObj(m_surfaceFilter);
        deleteVTKObj(m_highOrderFilter);
    }

    void GraphObjectPick::init()
    {
        m_surfaceFilter = vtkDataSetSurfaceFilter::New();
        // m_surfaceFilter->SetPassThroughCellIds(true);
        m_surfaceFilter->SetNonlinearSubdivisionLevel(1);

        m_highOrderFilter = FITKHighOrderCellFilter::New();
        m_highOrderFilter->SetInputConnection(m_surfaceFilter->GetOutputPort());

        m_fActor = FITKGraphActor::New();
        m_fActor->setGraphObject(this);
        m_fActor->SetPickable(false);
        m_fActor->setColor(Qt::red);
        m_fActor->setInputConnection(m_highOrderFilter->GetOutputPort());
        addActor(m_fActor);

        _renderLayer = 2;
    }

    void GraphObjectPick::setPickedData(vtkDataSet* grid)
    {
        if (!m_surfaceFilter || !m_fActor || !grid)
        {
            return;
        }

        // Set the full appended picked dataset.
        //m_fActor->setInputDataObject(grid);
        m_surfaceFilter->SetInputData(grid);
        initActorProperties(m_fActor);
    }

    void GraphObjectPick::setPickedType(int type)
    {
        initActorProperties(m_fActor, type);
    }

    void GraphObjectPick::initActorProperties(vtkProp* actor, QVariant details)
    {
        if (!actor){
            return;
        }

        // Set model actor properties.
        // Default show face and edge.
        FITKGraphActor* fActor = FITKGraphActor::SafeDownCast(actor);
        if (!fActor)
        {
            return;
        }

        // Red wireframe or point.
        fActor->setScalarVisibility(false);
        fActor->SetPickable(false);

        fActor->GetProperty()->SetColor(1., 0., 0.);

        if (details.toInt() == 1)
        {
            // Points.
            fActor->GetProperty()->SetRepresentation(0);
            fActor->GetProperty()->SetVertexVisibility(true);
            fActor->GetProperty()->SetEdgeVisibility(false);
            //fActor->GetProperty()->SetOpacity(1.);
        }
        else if (details.toInt() == 2)
        {
            // Wireframe.
            fActor->GetProperty()->SetRepresentation(1);
            fActor->GetProperty()->SetVertexVisibility(false);
            fActor->GetProperty()->SetEdgeVisibility(true);
            //fActor->GetProperty()->SetOpacity(1.);
        }
        else if (details.toInt() == 3)
        {
            // Surface and wireframe.
            fActor->GetProperty()->SetRepresentation(2);
            fActor->GetProperty()->SetVertexVisibility(false);
            fActor->GetProperty()->SetEdgeVisibility(false);
            //fActor->GetProperty()->SetOpacity(0.4);
        }

        // Points. ( Should be larger than pick preview size )
        fActor->GetProperty()->SetPointSize(9);

        // Edge. ( Should be bigger than pick preview size )
        fActor->GetProperty()->SetLineWidth(2);
    }
}   // namespace Exchange