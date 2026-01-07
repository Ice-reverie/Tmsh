#include "FITKMeshAlgorithmGeneratorTmshExec.h"
#include "FITKTmshGlobalMeshGenerateAlgorithmInfo.h"

namespace Tmsh
{
    Interface::FITKGlobalMeshGenerateAlgorithmInfo* FITKMeshAlgorithmGeneratorTmshExec::generateGlobalMeshAlgorithmInfo()
    {
        // 在实例化后通过 set*() 写入默认参数，供界面初始化显示；也保证命令行拼参可直接读取。
        auto* info = new FITKTmshGlobalMeshGenerateAlgorithmInfo;
        info->setTangencyTolerance(15);

        info->setTangencyTolerance(15);
        info->setTangencyTolerance(15);
        info->setTangencyTolerance(15);
        info->setTangencyTolerance(15);
        info->setTangencyTolerance(15);



        return info;
    }
}
