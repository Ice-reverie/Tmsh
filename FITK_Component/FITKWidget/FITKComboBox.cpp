#include "FITKComboBox.h"

#include <QListView>
#include <QMouseEvent>


FITKComboBox::FITKComboBox(QWidget* p)
    : QComboBox(p)
{
    //创建并设置为Listview
    QListView * lv = new QListView(this);
    this->setView(lv);
}

//虚函数
FITKComboBox::~FITKComboBox()
{
}

void FITKComboBox::setWheelDisable(bool isDisable)
{
    _isDisableWheel = isDisable;
}

void FITKComboBox::mousePressEvent(QMouseEvent * event)
{
    if (event->button() == Qt::LeftButton) {
        emit clicked(true);
    }
    QComboBox::mousePressEvent(event);
}

void FITKComboBox::wheelEvent(QWheelEvent * e)
{
    if (_isDisableWheel == true) {
        return;
    }
    QComboBox::wheelEvent(e);
}

