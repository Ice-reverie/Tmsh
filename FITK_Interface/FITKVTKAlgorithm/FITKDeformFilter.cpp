#include "FITKDeformFilter.h"

// VTK
#include <vtkDemandDrivenPipeline.h>
#include <vtkStreamingDemandDrivenPipeline.h>
#include <vtkInformation.h>
#include <vtkInformationVector.h>
#include <vtkUnstructuredGrid.h>
#include <vtkDataArray.h>
#include <vtkPointData.h>
#include <vtkPoints.h>
#include <vtkCellData.h>

// For VTK 7
//@{
#include <vtkSmartPointer.h>
#include <vtkCell.h>
//@}

FITKDeformFilter* FITKDeformFilter::New()
{
    FITKDeformFilter* filter = new FITKDeformFilter;
    filter->InitializeObjectBase();
    return filter;
}

void FITKDeformFilter::PrintSelf(ostream& os, vtkIndent indent)
{
    this->Superclass::PrintSelf(os, indent);
}

int FITKDeformFilter::ProcessRequest(vtkInformation* request, vtkInformationVector** inputVector, vtkInformationVector* outputVector)
{
    if (request->Has(vtkDemandDrivenPipeline::REQUEST_INFORMATION()))
    {
        return this->RequestInformation(request, inputVector, outputVector);
    }
    if (request->Has(vtkStreamingDemandDrivenPipeline::REQUEST_UPDATE_EXTENT()))
    {
        return this->RequestUpdateExtent(request, inputVector, outputVector);
    }
    if (request->Has(vtkDemandDrivenPipeline::REQUEST_DATA()))
    {
        return this->RequestData(request, inputVector, outputVector);
    }
    return this->Superclass::ProcessRequest(request, inputVector, outputVector);
}

int FITKDeformFilter::RequestData(vtkInformation* vtkNotUsed(request), vtkInformationVector** inputVector, vtkInformationVector* outputVector)
{
    // Add the array to data set.
    vtkDataSet* inputData = vtkDataSet::GetData(inputVector[0]);
    vtkUnstructuredGrid* outputData = vtkUnstructuredGrid::GetData(outputVector);
    if (inputData == nullptr)
    {
        vtkErrorMacro("Input data object is invalid!");
        return 0;
    }

    // Get the point data of the input data.
    auto pointData = inputData->GetPointData();
    if (pointData == nullptr)
    {
        vtkErrorMacro("The point data of data set is invalid!");
        return 0;
    }

    // Check the array name and component indice of U, V, W.
    bool hasValidArray = true;
    vtkDataArray* deformArray = pointData->GetArray(this->DeformArrayName);
    if (deformArray == nullptr)
    {
        hasValidArray = false;
    }

    // Check component indice.
    if (this->UComponentIndex < 0 || this->VComponentIndex < 0 || this->WComponentIndex < 0)
    {
        hasValidArray = false;
    }

    // Check component indice with array.
    if (hasValidArray)
    {
        int nComp = deformArray->GetNumberOfComponents();
        if (nComp <= this->UComponentIndex || nComp <= this->VComponentIndex || nComp <= this->WComponentIndex)
        {
            hasValidArray = false;
        }
    }

    // Reset the scale factor value to zero if the factor is below zero.
    //if (ScaleFactor < 0)
    //{
    //    ScaleFactor = 0.;
    //}

    // Get the number of input points.
    int nPts = inputData->GetNumberOfPoints();

    // Copy a new data for output.
    vtkSmartPointer<vtkUnstructuredGrid> tempData = vtkSmartPointer<vtkUnstructuredGrid>::New();
    vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();

    if (Deform && hasValidArray)
    {
        for (int i = 0; i < nPts; i++)
        {
            double coor[3]{ 0., 0., 0. };
            inputData->GetPoint(i, coor);

            double* tp = deformArray->GetTuple(i);
            double du = tp[this->UComponentIndex];
            double dv = tp[this->VComponentIndex];
            double dw = tp[this->WComponentIndex];

            // Add the displacement and the original point coordinate.
            double coorNew[3]{ 0., 0., 0. };
            coorNew[0] = coor[0] + du * this->ScaleFactor;
            coorNew[1] = coor[1] + dv * this->ScaleFactor;
            coorNew[2] = coor[2] + dw * this->ScaleFactor;

            points->InsertPoint(i, coorNew);
        }
    }
    else
    {
        for (int i = 0; i < nPts; i++)
        {
            double coor[3]{ 0., 0., 0. };
            inputData->GetPoint(i, coor);
            points->InsertPoint(i, coor);
        }
    }

    tempData->SetPoints(points);

    // Copy cells.
    const int nCell = inputData->GetNumberOfCells();
    for (int i = 0; i < nCell; i++)
    {
        vtkCell* cell = inputData->GetCell(i);

        tempData->InsertNextCell(cell->GetCellType(), cell->GetPointIds());
    }

    // Copy structures.
    outputData->CopyStructure(tempData);

    // Copy arrays.
    outputData->GetPointData()->PassData(inputData->GetPointData());
    outputData->GetCellData()->PassData(inputData->GetCellData());

    return 1;
}

FITKDeformFilter::FITKDeformFilter()
{
    // Initialize the algorithm parameters.
    this->ScaleFactor = 1;
    this->DeformArrayName = NULL;
    this->Deform = 1;

    this->UComponentIndex = 0;
    this->VComponentIndex = 1;
    this->WComponentIndex = 2;
}

FITKDeformFilter::~FITKDeformFilter()
{
    // Nothing to do here.
}

int FITKDeformFilter::FillInputPortInformation(int vtkNotUsed(port), vtkInformation* info)
{
    info->Set(vtkAlgorithm::INPUT_REQUIRED_DATA_TYPE(), "vtkDataSet");
    return 1;
}

int FITKDeformFilter::FillOutputPortInformation(int vtkNotUsed(port), vtkInformation* info)
{
    info->Set(vtkDataObject::DATA_TYPE_NAME(), "vtkUnstructuredGrid");
    return 1;
}
