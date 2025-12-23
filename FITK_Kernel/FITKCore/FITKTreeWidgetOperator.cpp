#include "FITKTreeWidgetOperator.h"

namespace Core
{
    FITKTreeWidgetOperator::~FITKTreeWidgetOperator()
    {
    }

    void FITKTreeWidgetOperator::on_itemClicked(QTreeWidget* w, QTreeWidgetItem* item, int col /*= 0*/)
    {
         //虚函数，不做任何操作
    }

    void FITKTreeWidgetOperator::on_itemDoubleClicked(QTreeWidget* w, QTreeWidgetItem* item, int col /*= 0*/)
    {
       //虚函数，不做任何操作
    }

}
