/*****************************************************************//**
 * @file        FITKVertexGlyphFilter.h
 * @brief       Points to vertice filter for vtkDataSet.
 *
 * @author      ChengHaotian (yeguangbaozi@foxmail.com)
 * @date        2025-08-04
 *********************************************************************/

#ifndef FITKVertexGlyphFilter_h
#define FITKVertexGlyphFilter_h

#include "FITKVTKAlgorithmAPI.h"
#include "vtkUnstructuredGridAlgorithm.h"

class FITKVTKALGORITHMAPI FITKVertexGlyphFilter : public vtkUnstructuredGridAlgorithm
{
public:
    vtkTypeMacro(FITKVertexGlyphFilter, vtkUnstructuredGridAlgorithm);
    void PrintSelf(ostream& os, vtkIndent indent) override;
    static FITKVertexGlyphFilter* New();

protected:
    FITKVertexGlyphFilter();
    ~FITKVertexGlyphFilter() override;

    int RequestData(vtkInformation*, vtkInformationVector**, vtkInformationVector*) override;
    int FillInputPortInformation(int, vtkInformation*) override;
    //int FillOutputPortInformation(int, vtkInformation* info) override;
    //int ProcessRequest(vtkInformation* request, vtkInformationVector** inputVector, 
    //    vtkInformationVector* outputVector) override;

private:
    FITKVertexGlyphFilter(const FITKVertexGlyphFilter&) = delete;
    void operator=(const FITKVertexGlyphFilter&) = delete;
};

#endif //_FITKVertexGlyphFilter_h
