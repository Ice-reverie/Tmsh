/*****************************************************************//**
 * @file        FITKHighOrderCellFilter.h
 * @brief       A filter for removing inner lines of high-oreder elements.
 *              ( Legacy )
 * 
 * @author      ChengHaotian (yeguangbaozi@foxmail.com)
 * @date        2024-03-19
 *********************************************************************/

#ifndef FITKHIGHORDERCELLFILTER_H
#define FITKHIGHORDERCELLFILTER_H

#include "vtkPolyDataAlgorithm.h"

#include "vtkSetGet.h"

#include "FITKVTKAlgorithmAPI.h"

 /**
  * @brief      Deal with high order cells' invisible edges.
  * @author     ChengHaotian (yeguangbaozi@foxmail.com)
  * @date       2024-03-25
  */
class FITKVTKALGORITHMAPI FITKHighOrderCellFilter : public vtkPolyDataAlgorithm
{
public:
    vtkTypeMacro(FITKHighOrderCellFilter, vtkPolyDataAlgorithm);
    static FITKHighOrderCellFilter* New();
    void PrintSelf(ostream& os, vtkIndent indent) override;

    ///**
    // * In order to determine which edges existed in the original data, we need an
    // * identifier on each cell determining which face (not cell) it originally
    // * came from.  The ids should be put in a cell data array with this name.  The
    // * existence of this field is also a signal that this wireframe extraction is
    // * necessary.
    // */
    //vtkSetMacro(OrginalCellIdsName, vtkStdString);
    //vtkGetMacro(OrginalCellIdsName, vtkStdString);

protected:
    FITKHighOrderCellFilter();
    ~FITKHighOrderCellFilter() override;

    int RequestData(vtkInformation* request, vtkInformationVector** inputVector,
        vtkInformationVector* outputVector) override;

private:
    FITKHighOrderCellFilter(const FITKHighOrderCellFilter&) = delete;
    void operator=(const FITKHighOrderCellFilter&) = delete;

    // Default: vtkOriginalCellIds
    // vtkStdString OrginalCellIdsName;

};

#endif // FITKHIGHORDERCELLFILTER_H
