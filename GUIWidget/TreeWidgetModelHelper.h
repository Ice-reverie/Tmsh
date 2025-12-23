#ifndef TREEWIDGETMODELHELPER_H
#define TREEWIDGETMODELHELPER_H

#include <QTreeWidgetItem>

namespace Interface
{
    class FITKAbsGeoCommand;
    class FITKAbsVirtualTopo;
    class FITKVirtualTopoManager;
}

namespace GUI
{
    class TreeWidgetModel;

    class TreeWidgetModelHelper
    {
    public:
        static void setTopoEnableRecursive(Interface::FITKAbsVirtualTopo* topo, bool enableState);
        static void setAllToposEnable(Interface::FITKVirtualTopoManager* vtMgr, bool enableState);
        static void setSubToposEnable(Interface::FITKAbsVirtualTopo* topo, bool enableState);
        static void appendSolidItems(TreeWidgetModel* treeWidget, QTreeWidgetItem* geoItem, Interface::FITKAbsGeoCommand* geoCmd);
        static void appendShellItems(TreeWidgetModel* treeWidget, QTreeWidgetItem* geoItem, Interface::FITKAbsGeoCommand* geoCmd);

        static constexpr int kCustomColorRole = Qt::UserRole + 5;

    private:
        static bool hasSolidAncestor(Interface::FITKAbsVirtualTopo* topo);
    };
}

#endif // TREEWIDGETMODELHELPER_H
