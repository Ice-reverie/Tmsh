#include "MeshQualityColorLegend.h"

#include <QPainter>
#include <QLinearGradient>
#include <QPaintEvent>

#include "FITK_Kernel/FITKAppFramework/FITKMessage.h"

namespace GUI
{
    namespace
    {
        QString colorName(const QColor& c)
        {
            if (c == QColor(255, 0, 0)) return QStringLiteral("红");
            if (c == QColor(0, 255, 0)) return QStringLiteral("绿");
            if (c == QColor(0, 0, 255)) return QStringLiteral("蓝");
            if (c == QColor(255, 255, 255)) return QStringLiteral("白");
            if (c == QColor(0, 0, 0)) return QStringLiteral("黑");
            if (c == QColor(128, 0, 128)) return QStringLiteral("紫");
            if (c == QColor(0, 255, 255)) return QStringLiteral("青");
            return c.name();
        }
    }

    MeshQualityColorLegend::MeshQualityColorLegend(QWidget* parent)
        : QWidget(parent)
    {
        setMinimumHeight(80);
    }

    void MeshQualityColorLegend::setLegendData(Graph::ColorScheme scheme,
                                               Interface::QualityMetric metric,
                                               double minValue,
                                               double maxValue)
    {
        _scheme = scheme;
        _metric = metric;
        _minValue = minValue;
        _maxValue = maxValue;
        update();
    }

    void MeshQualityColorLegend::setDirectionVisible(bool visible)
    {
        _directionVisible = visible;
        update();
    }

    bool MeshQualityColorLegend::isDirectionVisible() const
    {
        return _directionVisible;
    }

    void MeshQualityColorLegend::paintEvent(QPaintEvent* event)
    {
        Q_UNUSED(event);

        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing, true);
        painter.fillRect(rect(), QColor(250, 250, 250));

        if (_minValue >= _maxValue) {
            painter.setPen(Qt::darkGray);
            painter.drawText(rect(), Qt::AlignCenter, QStringLiteral("暂无有效质量范围"));
            return;
        }

        Graph::MeshQualityColorMapper mapper;
        mapper.setColorScheme(_scheme);
        mapper.setQualityMetric(_metric);
        mapper.setRange(_minValue, _maxValue);

        QMap<double, QColor> colorBar = mapper.getColorBar(64);

        Graph::ColorScheme effectiveScheme = _scheme;
        if (colorBar.size() < 2) {
            AppFrame::FITKMessageWarning(QStringLiteral("颜色方案不可用，已切换到默认方案"));
            effectiveScheme = Graph::ColorScheme::Traffic;
            mapper.setColorScheme(effectiveScheme);
            colorBar = mapper.getColorBar(64);
        }

        const int w = width();
        const int barLeft = 6;
        const int barRight = w - 6;
        const int barTop = 8;
        const int barHeight = 20;

        QLinearGradient gradient(barLeft, 0, barRight, 0);
        int idx = 0;
        const int count = colorBar.size();
        for (QMap<double, QColor>::const_iterator it = colorBar.constBegin(); it != colorBar.constEnd(); ++it, ++idx) {
            double pos = (count <= 1) ? 0.0 : static_cast<double>(idx) / (count - 1);
            gradient.setColorAt(pos, it.value());
        }

        painter.setPen(Qt::NoPen);
        painter.setBrush(gradient);
        painter.drawRect(barLeft, barTop, barRight - barLeft, barHeight);

        painter.setBrush(Qt::NoBrush);
        painter.setPen(QColor(120, 120, 120));
        painter.drawRect(barLeft, barTop, barRight - barLeft, barHeight);

        painter.setPen(Qt::black);
        QFont valueFont = painter.font();
        valueFont.setPointSize(8);
        painter.setFont(valueFont);
        painter.drawText(QRect(barLeft, barTop + barHeight + 2, 120, 16),
                         Qt::AlignLeft | Qt::AlignVCenter,
                         QString::number(_minValue, 'g', 4));
        painter.drawText(QRect(barRight - 120, barTop + barHeight + 2, 120, 16),
                         Qt::AlignRight | Qt::AlignVCenter,
                         QString::number(_maxValue, 'g', 4));

        Graph::QualityDirection direction = Graph::qualityMetricDirection(_metric);

        if (_directionVisible && direction == Graph::QualityDirection::Undefined) {
            AppFrame::FITKMessageWarning(QStringLiteral("该指标优劣方向未定义"));
        }

        if (_directionVisible && direction != Graph::QualityDirection::Undefined) {
            bool goodAtMin = (direction == Graph::QualityDirection::LowerIsBetter);
            QString minLabel = goodAtMin ? QStringLiteral("质量好") : QStringLiteral("质量差");
            QString maxLabel = goodAtMin ? QStringLiteral("质量差") : QStringLiteral("质量好");

            QFont labelFont = painter.font();
            labelFont.setPointSize(9);
            labelFont.setBold(true);
            painter.setFont(labelFont);

            painter.setPen(QColor(0, 128, 0));
            painter.drawText(QRect(barLeft, barTop + barHeight + 20, 140, 20),
                             Qt::AlignLeft | Qt::AlignVCenter, minLabel);
            painter.setPen(QColor(180, 0, 0));
            painter.drawText(QRect(barRight - 140, barTop + barHeight + 20, 140, 20),
                             Qt::AlignRight | Qt::AlignVCenter, maxLabel);
        }

        Graph::ColorSchemeSemantics sem = Graph::colorSchemeSemantics(effectiveScheme);
        QString hint = QStringLiteral("本方案参考：%1=质量好，%2=质量差")
                       .arg(colorName(sem.badColor), colorName(sem.goodColor));

        QFont hintFont = painter.font();
        hintFont.setPointSize(8);
        painter.setFont(hintFont);
        painter.setPen(QColor(80, 80, 80));
        painter.drawText(QRect(barLeft, height() - 22, w - barLeft * 2, 18),
                         Qt::AlignLeft | Qt::AlignVCenter, hint);
    }
}
