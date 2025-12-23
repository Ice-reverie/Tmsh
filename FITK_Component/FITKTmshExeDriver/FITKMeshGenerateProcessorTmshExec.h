/**********************************************************************
 * @file   FITKMeshGenerateProcessorTmshExec.h
 * @brief  Tmsh网格划分处理器（读取 .msh）
 * @date   2025-12-22
 *********************************************************************/
#ifndef _FITKMESHGENERATEPROCESSORTMSHEXEC_H_
#define _FITKMESHGENERATEPROCESSORTMSHEXEC_H_

#include "FITK_Interface/FITKInterfaceMeshGen/FITKAbstractMeshProcessor.h"

namespace Tmsh
{
    class FITKMeshGenerateProcessorTmshExec : public Interface::FITKAbstractMeshProcessor
    {
    public:
        explicit FITKMeshGenerateProcessorTmshExec();
        virtual ~FITKMeshGenerateProcessorTmshExec();

        virtual void start(QStringList info = QStringList()) override;
    };
}

#endif

