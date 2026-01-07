/**********************************************************************
 * @file   FITKMeshGenTmshExecInterface.h
 * @brief  Tmsh exe网格生成
 * @date   2025-12-22
 *********************************************************************/
#ifndef _FITKMESHGENTMSHEXECINTERFACE_H_
#define _FITKMESHGENTMSHEXECINTERFACE_H_

#include "FITKTmshExeDriverAPI.h"
#include "FITK_Kernel/FITKAppFramework/FITKComponentInterface.h"

namespace Tmsh
{
    class FITKTmshExeDriverAPI FITKMeshGenTmshExecInterface :
        public AppFrame::FITKComponentInterface
    {
    public:
        explicit FITKMeshGenTmshExecInterface();
        virtual ~FITKMeshGenTmshExecInterface() = default;

        virtual QString getComponentName() override;
        virtual bool exec(const int indexPort) override;
        virtual QList<int> getUnResetDataObj() override;
    };
}

#endif

