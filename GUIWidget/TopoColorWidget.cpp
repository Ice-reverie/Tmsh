#include "TopoColorWidget.h"

#include <QCheckBox>
#include <QLabel>
#include <QPushButton>
#include <QHBoxLayout>
#include <QColor>
#include <QSizePolicy>

namespace GUI
{
    TopoColorWidget::TopoColorWidget(const QString& text, bool checked, const QColor& color, QWidget* parent)
        : QWidget(parent)
    {
        // Expand to the full cell width so we can align the color button column.
        this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

        auto* layout = new QHBoxLayout(this);
        // Add a small right margin so the color button isn't flush against the tree border.
        layout->setContentsMargins(0, 0, 8, 0);
        layout->setSpacing(6);

        _check = new QCheckBox(this);
        _check->setChecked(checked);
        layout->addWidget(_check, 0, Qt::AlignLeft);

        // Separate label so clicking text不会触发勾选，只能点方框切换。
        auto* label = new QLabel(text, this);
        layout->addWidget(label, 0, Qt::AlignLeft);

        _colorBtn = new QPushButton(this);
        _colorBtn->setFixedSize(16, 16);
        _colorBtn->setObjectName("colorBtn");
        setColor(color);
        _colorBtn->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

        // Stretch between the checkbox text and the color square to right-align the color column.
        layout->addStretch();
        layout->addWidget(_colorBtn, 0, Qt::AlignRight);

        connect(_check, &QCheckBox::toggled, this, &TopoColorWidget::toggled);
        connect(_colorBtn, &QPushButton::clicked, this, &TopoColorWidget::colorClicked);
    }

    void TopoColorWidget::setColor(const QColor& c)
    {
        if (!c.isValid()) return;
        _colorBtn->setStyleSheet(QString("border: 1px solid #666; background-color: rgb(%1,%2,%3);")
            .arg(c.red()).arg(c.green()).arg(c.blue()));
    }
}

#include "moc_TopoColorWidget.cpp"
