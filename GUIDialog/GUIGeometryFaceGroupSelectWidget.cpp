#include "GUIGeometryFaceGroupSelectWidget.h"
#include "ui_GUIGeometryFaceGroupSelectWidget.h"

#include "GraphData/GraphDataPick/GraphPickProvider.h"
#include "GraphData/GraphDataPick/GraphDataPickInfo.h"
#include "GraphData/GraphDataPick/GraphPickData.h"

#include <QApplication>
#include <QStyle>

namespace GUI
{
    GUIGeometryFaceGroupSelectWidget::GUIGeometryFaceGroupSelectWidget(QWidget * parent) :
        QWidget(parent)
    {
        _ui = new Ui::GUIGeometryFaceGroupSelectWidget();
        _ui->setupUi(this);

        init();
    }

    GUIGeometryFaceGroupSelectWidget::~GUIGeometryFaceGroupSelectWidget()
    {
        if (_ui)delete _ui;
    }

    void GUIGeometryFaceGroupSelectWidget::init()
    {
        _ui->pushButton_OK->setStyleSheet("background: transparent;");
        _ui->pushButton_OK->setIcon(QApplication::style()->standardIcon(QStyle::SP_DialogApplyButton));
        _ui->pushButton_OK->hide();

        _ui->pushButton_Cancel->setStyleSheet("background: transparent;");
        _ui->pushButton_Cancel->setIcon(QApplication::style()->standardIcon(QStyle::SP_FileDialogBack));
        //_ui->pushButton_Cancel->setIcon(QIcon(":/icons/back.png"));
        _ui->pushButton_Cancel->hide();

        _ui->pushButton_Delete->setStyleSheet("background: transparent;");
        _ui->pushButton_Delete->setIcon(QApplication::style()->standardIcon(QStyle::SP_DialogCancelButton));
    }

    void GUIGeometryFaceGroupSelectWidget::setGroupData(QString groupName, int groupID)
    {
        _groupData.first = groupName;
        _groupData.second = groupID;
    }

    QString GUIGeometryFaceGroupSelectWidget::getGroupName()
    {
        return  _groupData.first;
    }

    int GUIGeometryFaceGroupSelectWidget::getGroupID()
    {
        return  _groupData.second;
    }

    void GUIGeometryFaceGroupSelectWidget::setPickDataIDs(QList<int> ids)
    {
        _pickDataIDs = ids;
    }

    QList<int> GUIGeometryFaceGroupSelectWidget::getPickDataIDs()
    {
        return _pickDataIDs;
    }

    void GUIGeometryFaceGroupSelectWidget::updateWidget()
    {
        //更新组名
        _ui->lineEdit_Name->setText(QString("%1 (%2)").arg(_groupData.first).arg(_pickDataIDs.size()));
    }

    void GUIGeometryFaceGroupSelectWidget::setSelectState(bool state)
    {
        if (state) {
            _ui->pushButton_OK->show();
            _ui->pushButton_Cancel->show();
        }
        else
        {
            _ui->pushButton_OK->hide();
            _ui->pushButton_Cancel->hide();
        }
    }

    void GUIGeometryFaceGroupSelectWidget::on_pushButton_OK_clicked()
    {
        //获取拾取操作器
        Graph::GraphPickProvider* pickD = Graph::GraphPickProvider::getInstance();
        if (pickD == nullptr) return;
        _pickDataIDs.clear();
        //获取拾取的数据
        QList<Graph::GraphPickData*> pickData = pickD->getPickedList();
        for (QList<Graph::GraphPickData*>::iterator iter = pickData.begin(); iter != pickData.end(); ++iter)
        {
            _pickDataIDs.append((*iter)->getPickedIDs());
        }
        //更新窗口
        this->updateWidget();
        emit sigOkClickedFinish();
    }

    void GUIGeometryFaceGroupSelectWidget::on_pushButton_Cancel_clicked()
    {
        emit sigCancelClicked();
    }

    void GUIGeometryFaceGroupSelectWidget::on_pushButton_Delete_clicked()
    {
        emit sigDeleteClicked();
    }

}

