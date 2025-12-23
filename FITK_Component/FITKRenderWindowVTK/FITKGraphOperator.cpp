#include "FITKGraphOperator.h"

namespace Comp
{
    FITKGraphOperator::FITKGraphOperator()
    {

    }

    FITKGraphOperator::~FITKGraphOperator()
    {

    }
   
    void FITKGraphOperator::setGraph3DWindow(FITKGraph3DWindowVTK* w)
    {
        //设置三维渲染窗口
        m_graph3DWindow = w;
    }

    Comp::FITKGraph3DWindowVTK* FITKGraphOperator::getGraph3DWindow()
    {
        //返回渲染窗口
        return m_graph3DWindow;
    }

}