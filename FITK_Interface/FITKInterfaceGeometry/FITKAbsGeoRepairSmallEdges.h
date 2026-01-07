/**
 * @file   FITKAbsGeoRepairSmallEdges.h
 * @brief  修复小边
 * @author Yanzhihui (chanyuantiandao@126.com)
 * @date   2025-03-27
 */
#ifndef FITKABSGEOREPAIRSMALLEDGES_H
#define FITKABSGEOREPAIRSMALLEDGES_H


#include "FITKAbsGeoCommand.h"
#include "FITKAbsGeoDatum.h"
#include "FITKInterfaceGeometryAPI.h"
#include <array>

namespace Interface {
    /**
     * @brief  修复小边.
     * @author YanZhiHui (chanyuantiandao@126.com)
     * @date   2024-09-10
     */
    class FITKInterfaceGeometryAPI FITKAbsGeoRepairSmallEdges :
        public FITKAbsGeoCommand
    {
        FITKCLASS(Interface, FITKAbsGeoRepairSmallEdges);
    public:
        FITKAbsGeoRepairSmallEdges() = default;
        virtual ~FITKAbsGeoRepairSmallEdges() = default;
        FITKGeoEnum::FITKGeometryComType getGeometryCommandType() override;

        QList<VirtualShape> edges() const { return m_Edges; }
        void setEdges(QList<VirtualShape> edges){ m_Edges= edges;}

    protected:
        /**
         * @brief  要修复的边
         * @author Yanzhihui (chanyuantiandao@126.com)
         * @date   2025-03-25
         */
        QList<VirtualShape> m_Edges{};
    };
}
#endif // !FITKABSGEOREPAIRSMALLEDGES_H
