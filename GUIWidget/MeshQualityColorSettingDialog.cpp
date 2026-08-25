#include "MeshQualityColorSettingDialog.h"
#include "MeshQualityColorLegend.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QLabel>
#include <QDialogButtonBox>

namespace GUI
{
    MeshQualityColorSettingDialog::MeshQualityColorSettingDialog(QWidget* parent)
        : QDialog(parent)
    {
        initUI();
    }

    void MeshQualityColorSettingDialog::initUI()
    {
        setWindowTitle(QStringLiteral("网格质量着色设置"));
        setMinimumSize(350, 300);

        QVBoxLayout* mainLayout = new QVBoxLayout(this);

        QGroupBox* metricGroup = new QGroupBox(QStringLiteral("质量指标"), this);
        QVBoxLayout* metricLayout = new QVBoxLayout(metricGroup);

        QHBoxLayout* metricRow = new QHBoxLayout();
        metricRow->addWidget(new QLabel(QStringLiteral("指标类型:"), this));
        _metricCombo = new QComboBox(this);
        _metricCombo->addItem(QStringLiteral("长宽比 (AspectRatio)"), static_cast<int>(Interface::QualityMetric::AspectRatio));
        _metricCombo->addItem(QStringLiteral("雅可比 (Jacobian)"), static_cast<int>(Interface::QualityMetric::Jacobian));
        _metricCombo->addItem(QStringLiteral("偏斜度 (Skewness)"), static_cast<int>(Interface::QualityMetric::Skewness));
        _metricCombo->addItem(QStringLiteral("扭曲度 (Warpage)"), static_cast<int>(Interface::QualityMetric::Warpage));
        _metricCombo->addItem(QStringLiteral("缩放雅可比 (Scaled Jacobian)"), static_cast<int>(Interface::QualityMetric::ScaledJacobian));
        _metricCombo->addItem(QStringLiteral("形状因子 (Shape)"), static_cast<int>(Interface::QualityMetric::Shape));
        _metricCombo->addItem(QStringLiteral("条件数 (Condition)"), static_cast<int>(Interface::QualityMetric::Condition));
        metricRow->addWidget(_metricCombo);
        metricLayout->addLayout(metricRow);

        mainLayout->addWidget(metricGroup);

        QGroupBox* colorGroup = new QGroupBox(QStringLiteral("颜色方案"), this);
        QVBoxLayout* colorLayout = new QVBoxLayout(colorGroup);

        QHBoxLayout* schemeRow = new QHBoxLayout();
        schemeRow->addWidget(new QLabel(QStringLiteral("颜色方案:"), this));
        _schemeCombo = new QComboBox(this);
        _schemeCombo->addItem(QStringLiteral("彩虹色 (Rainbow)"), static_cast<int>(Graph::ColorScheme::Rainbow));
        _schemeCombo->addItem(QStringLiteral("交通灯 (Traffic)"), static_cast<int>(Graph::ColorScheme::Traffic));
        _schemeCombo->addItem(QStringLiteral("蓝红渐变 (Blue-Red)"), static_cast<int>(Graph::ColorScheme::BlueRed));
        _schemeCombo->addItem(QStringLiteral("热图 (Heat)"), static_cast<int>(Graph::ColorScheme::Heat));
        _schemeCombo->addItem(QStringLiteral("冷色 (Cool)"), static_cast<int>(Graph::ColorScheme::Cool));
        schemeRow->addWidget(_schemeCombo);
        colorLayout->addLayout(schemeRow);

        _legend = new MeshQualityColorLegend(colorGroup);
        _legend->setMinimumHeight(80);
        colorLayout->addWidget(_legend);

        mainLayout->addWidget(colorGroup);

        QGroupBox* rangeGroup = new QGroupBox(QStringLiteral("数值范围"), this);
        QVBoxLayout* rangeLayout = new QVBoxLayout(rangeGroup);

        _autoRangeCheck = new QCheckBox(QStringLiteral("自动范围"), this);
        _autoRangeCheck->setChecked(true);
        connect(_autoRangeCheck, &QCheckBox::toggled, this, &MeshQualityColorSettingDialog::onAutoRangeToggled);
        rangeLayout->addWidget(_autoRangeCheck);

        QHBoxLayout* minRow = new QHBoxLayout();
        minRow->addWidget(new QLabel(QStringLiteral("最小值:"), this));
        _minSpin = new QDoubleSpinBox(this);
        _minSpin->setRange(-1e10, 1e10);
        _minSpin->setDecimals(6);
        _minSpin->setValue(1.0);
        _minSpin->setEnabled(false);
        minRow->addWidget(_minSpin);
        rangeLayout->addLayout(minRow);

        QHBoxLayout* maxRow = new QHBoxLayout();
        maxRow->addWidget(new QLabel(QStringLiteral("最大值:"), this));
        _maxSpin = new QDoubleSpinBox(this);
        _maxSpin->setRange(-1e10, 1e10);
        _maxSpin->setDecimals(6);
        _maxSpin->setValue(10.0);
        _maxSpin->setEnabled(false);
        maxRow->addWidget(_maxSpin);
        rangeLayout->addLayout(maxRow);

        mainLayout->addWidget(rangeGroup);

        QHBoxLayout* buttonLayout = new QHBoxLayout();
        buttonLayout->addStretch();

        _applyBtn = new QPushButton(QStringLiteral("应用"), this);
        connect(_applyBtn, &QPushButton::clicked, this, &MeshQualityColorSettingDialog::onApplyClicked);
        buttonLayout->addWidget(_applyBtn);

        _okBtn = new QPushButton(QStringLiteral("确定"), this);
        connect(_okBtn, &QPushButton::clicked, this, &MeshQualityColorSettingDialog::onOkClicked);
        buttonLayout->addWidget(_okBtn);

        _cancelBtn = new QPushButton(QStringLiteral("取消"), this);
        connect(_cancelBtn, &QPushButton::clicked, this, &QDialog::reject);
        buttonLayout->addWidget(_cancelBtn);

        mainLayout->addLayout(buttonLayout);

        connect(_schemeCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
                this, &MeshQualityColorSettingDialog::onLegendRefresh);
        connect(_metricCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
                this, &MeshQualityColorSettingDialog::onLegendRefresh);
        connect(_minSpin, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
                this, &MeshQualityColorSettingDialog::onLegendRefresh);
        connect(_maxSpin, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
                this, &MeshQualityColorSettingDialog::onLegendRefresh);
        connect(_autoRangeCheck, &QCheckBox::toggled,
                this, &MeshQualityColorSettingDialog::onLegendRefresh);

        onLegendRefresh();
    }

    Interface::QualityMetric MeshQualityColorSettingDialog::getQualityMetric() const
    {
        return static_cast<Interface::QualityMetric>(_metricCombo->currentData().toInt());
    }

    Graph::ColorScheme MeshQualityColorSettingDialog::getColorScheme() const
    {
        return static_cast<Graph::ColorScheme>(_schemeCombo->currentData().toInt());
    }

    double MeshQualityColorSettingDialog::getMinValue() const
    {
        return _minSpin->value();
    }

    double MeshQualityColorSettingDialog::getMaxValue() const
    {
        return _maxSpin->value();
    }

    bool MeshQualityColorSettingDialog::isAutoRange() const
    {
        return _autoRangeCheck->isChecked();
    }

    void MeshQualityColorSettingDialog::setCurrentValues(Interface::QualityMetric metric,
                                                         Graph::ColorScheme scheme,
                                                         double minVal,
                                                         double maxVal,
                                                         bool autoRange)
    {
        int metricIndex = _metricCombo->findData(static_cast<int>(metric));
        if (metricIndex >= 0) {
            _metricCombo->setCurrentIndex(metricIndex);
        }

        int schemeIndex = _schemeCombo->findData(static_cast<int>(scheme));
        if (schemeIndex >= 0) {
            _schemeCombo->setCurrentIndex(schemeIndex);
        }

        _minSpin->setValue(minVal);
        _maxSpin->setValue(maxVal);
        _autoRangeCheck->setChecked(autoRange);
    }

    void MeshQualityColorSettingDialog::onAutoRangeToggled(bool checked)
    {
        updateRangeEnabled();
    }

    void MeshQualityColorSettingDialog::onApplyClicked()
    {
        emit settingsChanged(getQualityMetric(), getColorScheme(),
                           getMinValue(), getMaxValue(), isAutoRange());
    }

    void MeshQualityColorSettingDialog::onOkClicked()
    {
        emit settingsChanged(getQualityMetric(), getColorScheme(),
                           getMinValue(), getMaxValue(), isAutoRange());
        accept();
    }

    void MeshQualityColorSettingDialog::updateRangeEnabled()
    {
        bool enabled = !_autoRangeCheck->isChecked();
        _minSpin->setEnabled(enabled);
        _maxSpin->setEnabled(enabled);
    }

    void MeshQualityColorSettingDialog::onLegendRefresh()
    {
        if (_legend == nullptr) {
            return;
        }
        _legend->setDirectionVisible(true);
        _legend->setLegendData(getColorScheme(), getQualityMetric(),
                               getMinValue(), getMaxValue());
    }
}