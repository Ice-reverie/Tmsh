#include "GraphObjectPickPreview.h"

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
    GraphObjectPickPreview::GraphObjectPickPreview()
        : GraphObjectBase(nullptr)
    {
        init();

        m_hasFixedBounds = false;
    }

    GraphObjectPickPreview::~GraphObjectPickPreview()
    {
        // Delete pointers.
        deleteVTKObj(m_selectNode);
        deleteVTKObj(m_section);
        deleteVTKObj(m_extractSelection);
        deleteVTKObj(m_idArray);
        deleteVTKObj(m_surfaceFilter);
        deleteVTKObj(m_highOrderFilter);
    }

    void GraphObjectPickPreview::init()
    {
        m_selectNode = vtkSelectionNode::New();
        m_section = vtkSelection::New();
        m_extractSelection = vtkExtractSelection::New();
        m_selectNode->SetContentType(vtkSelectionNode::INDICES);
        m_selectNode->SetFieldType(vtkSelectionNode::SelectionField::CELL);
        m_section->AddNode(m_selectNode);
        m_extractSelection->SetInputData(1, m_section);

        m_idArray = vtkIdTypeArray::New();
        m_selectNode->SetSelectionList(m_idArray);

        m_surfaceFilter = vtkDataSetSurfaceFilter::New();
        // m_surfaceFilter->SetPassThroughCellIds(true);
        m_surfaceFilter->SetNonlinearSubdivisionLevel(1);
        m_surfaceFilter->SetInputConnection(m_extractSelection->GetOutputPort());

        m_highOrderFilter = FITKHighOrderCellFilter::New();
        m_highOrderFilter->SetInputConnection(m_surfaceFilter->GetOutputPort());

        m_fActor = FITKGraphActor::New();
        m_fActor->setGraphObject(this);
        m_fActor->SetPickable(false);
        m_fActor->setColor(Qt::blue);
        m_fActor->setInputConnection(m_highOrderFilter->GetOutputPort());
        addActor(m_fActor);

        _renderLayer = 2;
    }

    void GraphObjectPickPreview::setPickedData(vtkActor* actor, int type, int index)
    {
        if (!actor || !m_idArray || !m_selectNode || !m_extractSelection || !m_fActor)
        {
            return;
        }

        // Update the extract index array and selection type.
        m_idArray->SetNumberOfValues(0);
        m_idArray->InsertNextValue(index);

        vtkDataSet* dataSet = actor->GetMapper()->GetInputAsDataSet();
        m_extractSelection->SetInputData(dataSet);

        // Update.
        m_extractSelection->Modified();

        // Set the actor properties.
        initActorProperties(m_fActor, type);

        m_fActor->update();
    }

    void GraphObjectPickPreview::setPickedData(vtkDataSet* grid)
    {
        if (!m_fActor || !grid)
        {
            return;
        }

        // Set the full appended picked dataset.
        m_fActor->setInputDataObject(grid);
        initActorProperties(m_fActor, 0);
    }

    void GraphObjectPickPreview::setPickedData(vtkDataSet* dataSet, int type, int index)
    {
        if (!dataSet || !m_idArray || !m_selectNode || !m_extractSelection || !m_fActor)
        {
            return;
        }

        if (type != 0 && type != 1)
        {
            return;
        }

        // Update the extract index array and selection type.
        m_idArray->SetNumberOfValues(0);
        m_idArray->InsertNextValue(index);
        m_extractSelection->SetInputData(dataSet);

        m_selectNode->SetFieldType(vtkSelectionNode::SelectionField(type));

        // Update.
        m_extractSelection->Modified();

        // Set the actor properties.
        initActorProperties(m_fActor, type);

        m_fActor->update();
    }

    void GraphObjectPickPreview::setPickedType(int type)
    {
        initActorProperties(m_fActor, type);
    }

    void GraphObjectPickPreview::initActorProperties(vtkProp* actor, QVariant details)
    {
        if (!actor)
        {
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

        fActor->GetProperty()->SetColor(1., 0.5, 0.);

        if (details.toInt() == 1)
        {
            // Points.
            fActor->GetProperty()->SetRepresentation(0);
            fActor->GetProperty()->SetVertexVisibility(true);
            fActor->GetProperty()->SetEdgeVisibility(false);
            fActor->GetProperty()->SetOpacity(1.);
        }
        else if (details.toInt() == 2)
        {
            // Wireframe.
            fActor->GetProperty()->SetRepresentation(1);
            fActor->GetProperty()->SetVertexVisibility(false);
            fActor->GetProperty()->SetEdgeVisibility(true);
            fActor->GetProperty()->SetOpacity(1.);
        }
        else if (details.toInt() == 3)
        {
            // Surface and wireframe.
            fActor->GetProperty()->SetRepresentation(2);
            fActor->GetProperty()->SetVertexVisibility(false);
            fActor->GetProperty()->SetEdgeVisibility(false);
            fActor->GetProperty()->SetOpacity(0.4);
        }

        // Points. ( Should be smaller than pick size )
        fActor->GetProperty()->SetPointSize(9);

        // Edge. ( Should be smaller than pick size )
        fActor->GetProperty()->SetLineWidth(2);
    }
}   // namespace Exchange