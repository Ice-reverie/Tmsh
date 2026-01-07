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


        void setMinLength(double len);
        double getMinLength() const;
        bool hasMinLength() const;
        void clearMinLength();

        void setMaxLength(double len);
        double getMaxLength() const;
        bool hasMaxLength() const;
        void clearMaxLength();

        void setMeshDim(int mes);
        double getMeshDim() const;
        bool hasMeshDim() const;
        void clearMeshDim();

        void setTetgenSwitches(const char* tet);
        double getTetgenSwitches() const;
        bool hasTetgenSwitches() const;
        void clearTetgenSwitches();

        void setNormalTol(double tol);
        double getNormalTol() const;
        bool hasNormalTol() const;
        void clearNormalTol();

        void setMinAngle(double ang);
        double getMinAngle() const;
        bool hasMinAngle() const;
        void clearMinAngle();



        void setUseMultiThreading(bool thr);
        double getUseMultiThreading() const;
        bool hasUseMultiThreading() const;
        void clearUseMultiThreading();

        void setRefineIter(int ite);
        double getRefineIter() const;
        bool hasRefineIter() const;
        void clearRefineIter();

        void setRefineSmoothIter(int ite);
        double getRefineSmoothIter() const;
        bool hasRefineSmoothIter() const;
        void clearRefineSmoothIter();

        void setAdaptIter(int ite);
        double getAdaptIter() const;
        bool hasAdaptIter() const;
        void clearAdaptIter();

        void setAdaptSmoothIter(int ite);
        double getAdaptSmoothIter() const;
        bool hasAdaptSmoothIter() const;
        void clearAdaptSmoothIter();
    };
}

#endif
