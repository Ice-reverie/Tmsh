/**********************************************************************
 * @file   MeshQualityColorMapper.h
 * @brief  Íø¸ñÖÊÁ¿µ½ÑÕÉ«µÄÓ³ÉäÀà
 * @author IndustrialMesh
 * @date   2026-06-10
 *********************************************************************/
#ifndef _MeshQualityColorMapper_H
#define _MeshQualityColorMapper_H

#include "GraphDataAdaptorAPI.h"
#include <QColor>
#include <QMap>
#include "FITK_Interface/FITKInterfaceModel/FITKAbstractMesh.h"

namespace Graph
{
    enum class ColorScheme
    {
        Rainbow = 0,    //²ÊºçÉ«£ººì-³È-»Æ-ÂÌ-Çà-À¶
        Traffic,        //½»Í¨µÆ£ººì-»Æ-ÂÌ
        BlueRed,        //À¶ºì½¥±ä£ºÀ¶-°×-ºì
        Heat,           //ÈÈÍ¼£ººÚ-ºì-»Æ-°×
        Cool            //ÀäÉ«£ºÇà-À¶-×Ï
    };

    class GraphDataAdaptorAPI MeshQualityColorMapper
    {
    public:
        MeshQualityColorMapper();
        ~MeshQualityColorMapper() = default;

        void setColorScheme(ColorScheme scheme);
        ColorScheme getColorScheme() const;

        void setQualityMetric(Interface::QualityMetric metric);
        Interface::QualityMetric getQualityMetric() const;

        void setRange(double minVal, double maxVal);
        double getMinValue() const;
        double getMaxValue() const;

        void setAutoRange(bool autoRange);
        bool isAutoRange() const;

        QColor mapToColor(double qualityValue) const;
        QColor mapToColor(const Interface::FITKElemntQuality& quality) const;

        void computeAutoRange(const QList<Interface::FITKElemntQuality>& qualities);

        QMap<double, QColor> getColorBar(int steps = 10) const;

    private:
        QColor interpolateColor(const QColor& c1, const QColor& c2, double t) const;
        double getQualityValue(const Interface::FITKElemntQuality& quality) const;

    private:
        ColorScheme _colorScheme{ ColorScheme::Traffic };
        Interface::QualityMetric _qualityMetric{ Interface::QualityMetric::AspectRatio };
        double _minValue{ 1.0 };
        double _maxValue{ 10.0 };
        bool _autoRange{ true };
    };
}

#endif