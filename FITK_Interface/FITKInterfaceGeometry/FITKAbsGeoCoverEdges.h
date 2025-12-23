/**
 * @file   FITKAbsGeoCoverEdges.h
 * @brief  覆盖面
 * @author Yanzhihui (chanyuantiandao@126.com)
 * @date   2025-03-26
 */
#ifndef FITKABSGEOCOVEREDGES_H
#define FITKABSGEOCOVEREDGES_H


#include "FITKAbsGeoCommand.h"
#include "FITKAbsGeoDatum.h"
#include "FITKInterfaceGeometryAPI.h"
#include <array>

namespace Interface {
    /**
     * @brief  覆盖面.
     * @author YanZhiHui (chanyuantiandao@126.com)
     * @date   2024-09-10
     */
    class FITKInterfaceGeometryAPI FITKAbsGeoCoverEdges :
        public FITKAbsGeoCommand
    {
        FITKCLASS(Interface, FITKAbsGeoCoverEdges);
    public:
        FITKAbsGeoCoverEdges() = default;
        virtual ~FITKAbsGeoCoverEdges() = default;
        FITKGeoEnum::FITKGeometryComType getGeometryCommandType() override;

        QList<VirtualShape> edges() const { return m_Edges; }
        void setEdges(QList<VirtualShape> edges){ m_Edges= edges;}

    protected:
        /**
         * @brief  拼接使用的边
         * @author Yanzhihui (chanyuantiandao@126.com)
         * @date   2025-03-25
         */
        QList<VirtualShape> m_Edges{};
    };
}
#endif // !FITKABSGEOCOVEREDGES_H
