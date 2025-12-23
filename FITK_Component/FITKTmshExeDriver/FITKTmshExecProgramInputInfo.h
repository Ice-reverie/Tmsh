/**********************************************************************
 * @file   FITKTmshExecProgramInputInfo.h
 * @brief  Tmsh 程序输入信息
 * @date   2025-12-22
 *********************************************************************/
#ifndef _FITK_TMSHEXEC_PROGRAMER_INPUTINFO_H___
#define _FITK_TMSHEXEC_PROGRAMER_INPUTINFO_H___

#include "FITKTmshExeDriverAPI.h"
#include "FITK_Kernel/FITKAppFramework/FITKAbstractProgramDriver.h"

namespace TmshExe
{
    class FITKTmshExeDriverAPI FITKTmshExecProgramInputInfo : public AppFrame::FITKProgramInputInfo
    {
    public:
        explicit FITKTmshExecProgramInputInfo();
        virtual ~FITKTmshExecProgramInputInfo();

        void setArgs(const QStringList& args) override;
        void clearArgs();

        virtual QStringList args() override;
        virtual void setMonitorFiles(QStringList& monitorFiles) override;
        virtual void setMonitorPath(QStringList& monitorpath) override;

    private:
        QStringList _args{};
    };
}

#endif
