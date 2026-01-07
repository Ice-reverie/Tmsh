#include "FITKTmshGlobalMeshGenerateAlgorithmInfo.h"

namespace Tmsh
{
    namespace
    {
        constexpr const char* kTangencyToleranceKey = "Tmsh.TangencyTolerance";   //tangent_tol

        constexpr const char* kMinLengthKey = "Tmsh.MinLength";
        constexpr const char* kMaxLengthKey = "Tmsh.MaxLength";
        constexpr const char* kMeshDimKey = "Tmsh.MeshDim";
        constexpr const char* kTetgenSwitchesKey = "Tmsh.TetgenSwitches";
        constexpr const char* kNormalTolKey = "Tmsh.NormalTol";
        constexpr const char* kMinAngleKey = "Tmsh.MinAngle";

        constexpr const char* kUseMultiThreadingKey = "Tmsh.UseMultiThreading";
        constexpr const char* kRefineIterKey = "Tmsh.RefineIter";
        constexpr const char* kRefineSmoothIterKey = "Tmsh.RefineSmoothIter";
        constexpr const char* kAdaptIterKey = "Tmsh.AdaptIter";
        constexpr const char* kAdaptSmoothIterKey = "Tmsh.AdaptSmoothIter";


        bool isSet(const Core::FITKVarientParams& params, const char* key)
        {
            const QVariant v = params.getValue(key);
            return v.isValid() && !v.isNull();
        }
    }

    FITKTmshGlobalMeshGenerateAlgorithmInfo::FITKTmshGlobalMeshGenerateAlgorithmInfo()
        : Interface::FITKGlobalMeshGenerateAlgorithmInfo()
    {

    }

    void FITKTmshGlobalMeshGenerateAlgorithmInfo::setTangencyTolerance(double tol)
    {
        this->setValue(kTangencyToleranceKey, tol);
    }

    double FITKTmshGlobalMeshGenerateAlgorithmInfo::getTangencyTolerance() const
    {
        return this->getValueT<double>(kTangencyToleranceKey);
    }

    bool FITKTmshGlobalMeshGenerateAlgorithmInfo::hasTangencyTolerance() const
    {
        return isSet(*this, kTangencyToleranceKey);
    }

    void FITKTmshGlobalMeshGenerateAlgorithmInfo::clearTangencyTolerance()
    {
        this->removeVarient(kTangencyToleranceKey);
    }



    void FITKTmshGlobalMeshGenerateAlgorithmInfo::setMinLength(double len)
    {
        this->setValue(kMinLengthKey, len);
    }

    double FITKTmshGlobalMeshGenerateAlgorithmInfo::getMinLength() const
    {
        return this->getValueT<double>(kMinLengthKey);
    }

    bool FITKTmshGlobalMeshGenerateAlgorithmInfo::hasMinLength() const
    {
        return isSet(*this, kMinLengthKey);
    }

    void FITKTmshGlobalMeshGenerateAlgorithmInfo::clearMinLength()
    {
        this->removeVarient(kMinLengthKey);
    }


    void FITKTmshGlobalMeshGenerateAlgorithmInfo::setMaxLength(double len)
    {
        this->setValue(kMaxLengthKey, len);
    }

    double FITKTmshGlobalMeshGenerateAlgorithmInfo::getMaxLength() const
    {
        return this->getValueT<double>(kMaxLengthKey);
    }

    bool FITKTmshGlobalMeshGenerateAlgorithmInfo::hasMaxLength() const
    {
        return isSet(*this, kMaxLengthKey);
    }

    void FITKTmshGlobalMeshGenerateAlgorithmInfo::clearMaxLength()
    {
        this->removeVarient(kMaxLengthKey);
    }


    void FITKTmshGlobalMeshGenerateAlgorithmInfo::setMeshDim(int mes)
    {
        this->setValue(kMeshDimKey, mes);
    }

    double FITKTmshGlobalMeshGenerateAlgorithmInfo::getMeshDim() const
    {
        return this->getValueT<int>(kMeshDimKey);
    }

    bool FITKTmshGlobalMeshGenerateAlgorithmInfo::hasMeshDim() const
    {
        return isSet(*this, kMeshDimKey);
    }

    void FITKTmshGlobalMeshGenerateAlgorithmInfo::clearMeshDim()
    {
        this->removeVarient(kMeshDimKey);
    }


    void FITKTmshGlobalMeshGenerateAlgorithmInfo::setTetgenSwitches(const char* tet)
    {
        this->setValue(kTetgenSwitchesKey, tet);
    }

    double FITKTmshGlobalMeshGenerateAlgorithmInfo::getTetgenSwitches() const
    {
        return this->getValueT<char>(kTetgenSwitchesKey);
    }

    bool FITKTmshGlobalMeshGenerateAlgorithmInfo::hasTetgenSwitches() const
    {
        return isSet(*this, kTetgenSwitchesKey);
    }

    void FITKTmshGlobalMeshGenerateAlgorithmInfo::clearTetgenSwitches()
    {
        this->removeVarient(kTetgenSwitchesKey);
    }


    void FITKTmshGlobalMeshGenerateAlgorithmInfo::setNormalTol(double tol)
    {
        this->setValue(kNormalTolKey, tol);
    }

    double FITKTmshGlobalMeshGenerateAlgorithmInfo::getNormalTol() const
    {
        return this->getValueT<double>(kNormalTolKey);
    }

    bool FITKTmshGlobalMeshGenerateAlgorithmInfo::hasNormalTol() const
    {
        return isSet(*this, kNormalTolKey);
    }

    void FITKTmshGlobalMeshGenerateAlgorithmInfo::clearNormalTol()
    {
        this->removeVarient(kNormalTolKey);
    }


    void FITKTmshGlobalMeshGenerateAlgorithmInfo::setMinAngle(double ang)
    {
        this->setValue(kMinAngleKey, ang);
    }

    double FITKTmshGlobalMeshGenerateAlgorithmInfo::getMinAngle() const
    {
        return this->getValueT<double>(kMinAngleKey);
    }

    bool FITKTmshGlobalMeshGenerateAlgorithmInfo::hasMinAngle() const
    {
        return isSet(*this, kMinAngleKey);
    }

    void FITKTmshGlobalMeshGenerateAlgorithmInfo::clearMinAngle()
    {
        this->removeVarient(kMinAngleKey);
    }



    void FITKTmshGlobalMeshGenerateAlgorithmInfo::setUseMultiThreading(bool thr)
    {
        this->setValue(kUseMultiThreadingKey, thr);
    }

    double FITKTmshGlobalMeshGenerateAlgorithmInfo::getUseMultiThreading() const
    {
        return this->getValueT<bool>(kUseMultiThreadingKey);
    }

    bool FITKTmshGlobalMeshGenerateAlgorithmInfo::hasUseMultiThreading() const
    {
        return isSet(*this, kUseMultiThreadingKey);
    }

    void FITKTmshGlobalMeshGenerateAlgorithmInfo::clearUseMultiThreading()
    {
        this->removeVarient(kUseMultiThreadingKey);
    }



    void FITKTmshGlobalMeshGenerateAlgorithmInfo::setRefineIter(int ite)
    {
        this->setValue(kRefineIterKey, ite);
    }

    double FITKTmshGlobalMeshGenerateAlgorithmInfo::getRefineIter() const
    {
        return this->getValueT<int>(kRefineIterKey);
    }

    bool FITKTmshGlobalMeshGenerateAlgorithmInfo::hasRefineIter() const
    {
        return isSet(*this, kRefineIterKey);
    }

    void FITKTmshGlobalMeshGenerateAlgorithmInfo::clearRefineIter()
    {
        this->removeVarient(kRefineIterKey);
    }


    void FITKTmshGlobalMeshGenerateAlgorithmInfo::setRefineSmoothIter(int ite)
    {
        this->setValue(kRefineSmoothIterKey, ite);
    }

    double FITKTmshGlobalMeshGenerateAlgorithmInfo::getRefineSmoothIter() const
    {
        return this->getValueT<int>(kRefineSmoothIterKey);
    }

    bool FITKTmshGlobalMeshGenerateAlgorithmInfo::hasRefineSmoothIter() const
    {
        return isSet(*this, kRefineSmoothIterKey);
    }

    void FITKTmshGlobalMeshGenerateAlgorithmInfo::clearRefineSmoothIter()
    {
        this->removeVarient(kRefineSmoothIterKey);
    }


    void FITKTmshGlobalMeshGenerateAlgorithmInfo::setAdaptIter(int ite)
    {
        this->setValue(kAdaptIterKey, ite);
    }

    double FITKTmshGlobalMeshGenerateAlgorithmInfo::getAdaptIter() const
    {
        return this->getValueT<int>(kAdaptIterKey);
    }

    bool FITKTmshGlobalMeshGenerateAlgorithmInfo::hasAdaptIter() const
    {
        return isSet(*this, kAdaptIterKey);
    }

    void FITKTmshGlobalMeshGenerateAlgorithmInfo::clearAdaptIter()
    {
        this->removeVarient(kAdaptIterKey);
    }


    void FITKTmshGlobalMeshGenerateAlgorithmInfo::setAdaptSmoothIter(int ite)
    {
        this->setValue(kAdaptSmoothIterKey, ite);
    }

    double FITKTmshGlobalMeshGenerateAlgorithmInfo::getAdaptSmoothIter() const
    {
        return this->getValueT<int>(kAdaptSmoothIterKey);
    }

    bool FITKTmshGlobalMeshGenerateAlgorithmInfo::hasAdaptSmoothIter() const
    {
        return isSet(*this, kAdaptSmoothIterKey);
    }

    void FITKTmshGlobalMeshGenerateAlgorithmInfo::clearAdaptSmoothIter()
    {
        this->removeVarient(kAdaptSmoothIterKey);
    }

}
