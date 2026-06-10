#include "MeshQualityColorMapper.h"
#include <QVector>
#include <cmath>

namespace Graph
{
    MeshQualityColorMapper::MeshQualityColorMapper()
    {
    }

    void MeshQualityColorMapper::setColorScheme(ColorScheme scheme)
    {
        _colorScheme = scheme;
    }

    ColorScheme MeshQualityColorMapper::getColorScheme() const
    {
        return _colorScheme;
    }

    void MeshQualityColorMapper::setQualityMetric(Interface::QualityMetric metric)
    {
        _qualityMetric = metric;
    }

    Interface::QualityMetric MeshQualityColorMapper::getQualityMetric() const
    {
        return _qualityMetric;
    }

    void MeshQualityColorMapper::setRange(double minVal, double maxVal)
    {
        _minValue = minVal;
        _maxValue = maxVal;
        if (_maxValue <= _minValue) {
            _maxValue = _minValue + 1.0;
        }
    }

    double MeshQualityColorMapper::getMinValue() const
    {
        return _minValue;
    }

    double MeshQualityColorMapper::getMaxValue() const
    {
        return _maxValue;
    }

    void MeshQualityColorMapper::setAutoRange(bool autoRange)
    {
        _autoRange = autoRange;
    }

    bool MeshQualityColorMapper::isAutoRange() const
    {
        return _autoRange;
    }

    QColor MeshQualityColorMapper::mapToColor(double qualityValue) const
    {
        double normalized = (qualityValue - _minValue) / (_maxValue - _minValue);
        normalized = qBound(0.0, normalized, 1.0);

        QVector<QColor> colors;
        switch (_colorScheme) {
        case ColorScheme::Rainbow:
            colors = { QColor(255, 0, 0), QColor(255, 127, 0), QColor(255, 255, 0),
                      QColor(0, 255, 0), QColor(0, 255, 255), QColor(0, 0, 255) };
            break;
        case ColorScheme::Traffic:
            colors = { QColor(255, 0, 0), QColor(255, 255, 0), QColor(0, 255, 0) };
            break;
        case ColorScheme::BlueRed:
            colors = { QColor(0, 0, 255), QColor(255, 255, 255), QColor(255, 0, 0) };
            break;
        case ColorScheme::Heat:
            colors = { QColor(0, 0, 0), QColor(255, 0, 0), QColor(255, 255, 0), QColor(255, 255, 255) };
            break;
        case ColorScheme::Cool:
            colors = { QColor(0, 255, 255), QColor(0, 0, 255), QColor(128, 0, 128) };
            break;
        }

        if (colors.size() < 2) {
            return QColor(255, 255, 255);
        }

        double segmentCount = colors.size() - 1;
        double segment = normalized * segmentCount;
        int segmentIndex = qMin(static_cast<int>(segment), static_cast<int>(segmentCount) - 1);
        double t = segment - segmentIndex;

        return interpolateColor(colors[segmentIndex], colors[segmentIndex + 1], t);
    }

    QColor MeshQualityColorMapper::mapToColor(const Interface::FITKElemntQuality& quality) const
    {
        double value = getQualityValue(quality);
        return mapToColor(value);
    }

    void MeshQualityColorMapper::computeAutoRange(const QList<Interface::FITKElemntQuality>& qualities)
    {
        if (qualities.isEmpty()) {
            return;
        }

        double minVal = std::numeric_limits<double>::max();
        double maxVal = std::numeric_limits<double>::lowest();

        for (const auto& q : qualities) {
            double val = getQualityValue(q);
            if (std::isfinite(val)) {
                minVal = qMin(minVal, val);
                maxVal = qMax(maxVal, val);
            }
        }

        if (minVal >= maxVal) {
            maxVal = minVal + 1.0;
        }

        _minValue = minVal;
        _maxValue = maxVal;
    }

    QMap<double, QColor> MeshQualityColorMapper::getColorBar(int steps) const
    {
        QMap<double, QColor> colorBar;

        if (steps < 2) {
            steps = 2;
        }

        double step = (_maxValue - _minValue) / (steps - 1);
        for (int i = 0; i < steps; ++i) {
            double value = _minValue + i * step;
            colorBar[value] = mapToColor(value);
        }

        return colorBar;
    }

    QColor MeshQualityColorMapper::interpolateColor(const QColor& c1, const QColor& c2, double t) const
    {
        t = qBound(0.0, t, 1.0);

        int r = static_cast<int>(c1.red() + t * (c2.red() - c1.red()));
        int g = static_cast<int>(c1.green() + t * (c2.green() - c1.green()));
        int b = static_cast<int>(c1.blue() + t * (c2.blue() - c1.blue()));

        return QColor(r, g, b);
    }

    double MeshQualityColorMapper::getQualityValue(const Interface::FITKElemntQuality& quality) const
    {
        switch (_qualityMetric) {
        case Interface::QualityMetric::AspectRatio:
            return quality._aspectRatio;
        case Interface::QualityMetric::Jacobian:
            return quality._jacobian;
        case Interface::QualityMetric::Skewness:
            return quality._skewness;
        case Interface::QualityMetric::Warpage:
            return quality._warpage;
        case Interface::QualityMetric::ScaledJacobian:
            return quality._scaledJacobian;
        case Interface::QualityMetric::Shape:
            return quality._shape;
        case Interface::QualityMetric::Condition:
            return quality._condition;
        default:
            return quality._aspectRatio;
        }
    }
}