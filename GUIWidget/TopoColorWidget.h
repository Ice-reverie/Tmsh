#ifndef TOPOCOLORWIDGET_H
#define TOPOCOLORWIDGET_H

#include <QWidget>

class QCheckBox;
class QPushButton;
class QColor;

namespace GUI
{
    /**
     * @brief 实体/壳体行内的小控件：复选框 + 颜色块
     */
    class TopoColorWidget : public QWidget
    {
        Q_OBJECT
    public:
        explicit TopoColorWidget(const QString& text, bool checked, const QColor& color, QWidget* parent = nullptr);

        void setColor(const QColor& c);
        QCheckBox* checkBox() const { return _check; }

    signals:
        void toggled(bool);
        void colorClicked();

    private:
        QCheckBox* _check{};
        QPushButton* _colorBtn{};
    };
}

#endif // TOPOCOLORWIDGET_H
