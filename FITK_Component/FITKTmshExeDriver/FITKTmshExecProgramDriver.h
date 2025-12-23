/**********************************************************************
 * @file   FITKTmshExecProgramDriver.h
 * @brief  Tmsh可执行程序驱动器
 * @date   2025-12-22
 *********************************************************************/
#ifndef _FITK_TMSHEXEC_PROGRAMER_DRIVER_H___
#define _FITK_TMSHEXEC_PROGRAMER_DRIVER_H___

#include "FITKTmshExeDriverAPI.h"
#include "FITK_Kernel/FITKAppFramework/FITKExecProgramDriver.h"
#include "FITK_Kernel/FITKAppFramework/FITKProgramDriverFactory.h"

namespace TmshExe
{
    class FITKTmshExeDriverAPI FITKTmshExecProgramDriver : public AppFrame::FITKExecProgramDriver
    {
        Q_OBJECT
    public:
        explicit FITKTmshExecProgramDriver();
        virtual ~FITKTmshExecProgramDriver();

        virtual int getProgramType() override;
        virtual QString getProgramName() override;

        bool isExistExeProgram();
    };

    Register2FITKProgramDriverFactory(1, FITKTmshExecProgramDriver, FITKTmshExecProgramDriver);
}

#endif

