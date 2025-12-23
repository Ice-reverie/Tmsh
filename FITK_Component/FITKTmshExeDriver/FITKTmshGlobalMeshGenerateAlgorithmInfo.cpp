#include "FITKTmshGlobalMeshGenerateAlgorithmInfo.h"

namespace Tmsh
{
    namespace
    {
        constexpr const char* kTangencyToleranceKey = "Tmsh.TangencyTolerance";

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
}
