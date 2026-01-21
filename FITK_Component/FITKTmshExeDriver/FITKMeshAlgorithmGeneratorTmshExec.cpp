#include "FITKMeshAlgorithmGeneratorTmshExec.h"
#include "FITKTmshGlobalMeshGenerateAlgorithmInfo.h"

namespace Tmsh
{
    Interface::FITKGlobalMeshGenerateAlgorithmInfo* FITKMeshAlgorithmGeneratorTmshExec::generateGlobalMeshAlgorithmInfo()
    {
        // 在实例化后通过 set*() 写入默认参数，供界面初始化显示；也保证命令行拼参可直接读取。
        auto* info = new FITKTmshGlobalMeshGenerateAlgorithmInfo;
        info->setTangencyTolerance(15);

        info->setMinLength(0);
        info->setMaxLength(0);
        info->setMeshDim(2);
        info->setTetgenSwitches("");
        info->setNormalTol(20);
        info->setMinAngle(20);
        info->setUseMultiThreading(1);
        info->setRefineIter(1);
        info->setRefineSmoothIter(300);
        info->setAdaptIter(3);
        info->setAdaptSmoothIter(30);

        //info->setTetgenSwitches("./output/Tmsh/geometryFile.mesh");

        info->setOption("");

        return info;
    }
}
