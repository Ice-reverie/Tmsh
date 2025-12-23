/**********************************************************************
 * @file   FITKTmshGlobalMeshGenerateAlgorithmInfo.h
 * @brief  Tmsh网格生成算法/参数信息（扩展FITK全局算法信息）
 * @date   2025-12-22
 *********************************************************************/
#ifndef _FITK_TMSH_GLOBAL_MESH_GENERATE_ALGORITHM_INFO_H_
#define _FITK_TMSH_GLOBAL_MESH_GENERATE_ALGORITHM_INFO_H_

#include "FITKTmshExeDriverAPI.h"
#include "FITK_Interface/FITKInterfaceMeshGen/FITKGlobalMeshGenerateAlgorithmInfo.h"

namespace Tmsh
{
    /**
     * @brief Tmsh 全局网格生成算法/参数信息
     * @details 通过 FITKVarientParams 存储 Tmsh 专用参数。
     *
     * 约定键名（可按 Tmsh 实际需求扩展）：
     * - "Tmsh.TangencyTolerance" : double
     */
    class FITKTmshExeDriverAPI FITKTmshGlobalMeshGenerateAlgorithmInfo
        : public Interface::FITKGlobalMeshGenerateAlgorithmInfo
    {
    public:
        explicit FITKTmshGlobalMeshGenerateAlgorithmInfo();
        virtual ~FITKTmshGlobalMeshGenerateAlgorithmInfo() = default;

        void setTangencyTolerance(double tol);
        double getTangencyTolerance() const;
        bool hasTangencyTolerance() const;
        void clearTangencyTolerance();
    };
}

#endif
