/**********************************************************************
 * @file   MeshQualityColorLegend.h
 * @brief  网格质量着色颜色图例控件
 * @author IndustrialMesh
 * @date   2026-06-10
 *********************************************************************/
#ifndef _MeshQualityColorLegend_H
#define _MeshQualityColorLegend_H

#include "GUIWidgetAPI.h"
#include <QWidget>
#include "FITK_Interface/FITKInterfaceModel/FITKAbstractMesh.h"
#include "GraphData/GraphDataAdaptor/MeshQualityColorMapper.h"

namespace GUI
{
    class GUIWidgetAPI MeshQualityColorLegend : public QWidget
    {
        Q_OBJECT

    public:
        explicit MeshQualityColorLegend(QWidget* parent = nullptr);
        ~MeshQualityColorLegend() = default;

        void setLegendData(Graph::ColorScheme scheme,
                           Interface::QualityMetric metric,
                           double minValue,
                           double maxValue);

        void setDirectionVisible(bool visible);
        bool isDirectionVisible() const;

    protected:
        void paintEvent(QPaintEvent* event) override;

    private:
        Graph::ColorScheme _scheme{ Graph::ColorScheme::Traffic };
        Interface::QualityMetric _metric{ Interface::QualityMetric::AspectRatio };
        double _minValue{ 1.0 };
        double _maxValue{ 10.0 };
        bool _directionVisible{ true };
    };
}

#endif
