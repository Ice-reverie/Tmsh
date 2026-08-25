/**********************************************************************
 * @file   MeshQualityColorSettingDialog.h
 * @brief  网格质量着色设置对话框
 * @author IndustrialMesh
 * @date   2026-06-10
 *********************************************************************/
#ifndef _MeshQualityColorSettingDialog_H
#define _MeshQualityColorSettingDialog_H

#include "GUIWidgetAPI.h"
#include <QDialog>
#include <QComboBox>
#include <QCheckBox>
#include <QDoubleSpinBox>
#include <QPushButton>
#include "FITK_Interface/FITKInterfaceModel/FITKAbstractMesh.h"
#include "GraphData/GraphDataAdaptor/MeshQualityColorMapper.h"

class QGroupBox;
class QLabel;

namespace GUI
{
    class MeshQualityColorLegend;

    class GUIWidgetAPI MeshQualityColorSettingDialog : public QDialog
    {
        Q_OBJECT

    public:
        explicit MeshQualityColorSettingDialog(QWidget* parent = nullptr);
        ~MeshQualityColorSettingDialog() = default;

        Interface::QualityMetric getQualityMetric() const;
        Graph::ColorScheme getColorScheme() const;
        double getMinValue() const;
        double getMaxValue() const;
        bool isAutoRange() const;

        void setCurrentValues(Interface::QualityMetric metric,
                             Graph::ColorScheme scheme,
                             double minVal,
                             double maxVal,
                             bool autoRange);

    signals:
        void settingsChanged(Interface::QualityMetric metric,
                            Graph::ColorScheme scheme,
                            double minVal,
                            double maxVal,
                            bool autoRange);

    private slots:
        void onAutoRangeToggled(bool checked);
        void onApplyClicked();
        void onOkClicked();
        void onLegendRefresh();

    private:
        void initUI();
        void updateRangeEnabled();

    private:
        QComboBox* _metricCombo{ nullptr };
        QComboBox* _schemeCombo{ nullptr };
        QCheckBox* _autoRangeCheck{ nullptr };
        QDoubleSpinBox* _minSpin{ nullptr };
        QDoubleSpinBox* _maxSpin{ nullptr };
        QPushButton* _applyBtn{ nullptr };
        QPushButton* _okBtn{ nullptr };
        QPushButton* _cancelBtn{ nullptr };
        MeshQualityColorLegend* _legend{ nullptr };
    };
}

#endif