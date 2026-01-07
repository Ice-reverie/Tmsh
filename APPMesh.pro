TEMPLATE = subdirs

SUBDIRS += \
    FITK_Kernel \
    FITK_Interface \
    FITK_Component \
    ModelData \
    GraphData \
    GUIWidget \
    GUIFrame \
    GUIDialog \
    HDF5IO \
    OperatorsInterface \
    OperatorsModel \
    OperatorsGUI \
    FITK_Plugins \
    MeshApp

FITK_Kernel.subdir = $$PWD/FITK_Kernel
FITK_Interface.subdir = $$PWD/FITK_Interface
FITK_Component.subdir = $$PWD/FITK_Component
FITK_Plugins.subdir = $$PWD/FITK_Plugins
GraphData.subdir = $$PWD/GraphData
ModelData.subdir = $$PWD/ModelData
GUIWidget.subdir = $$PWD/GUIWidget
GUIFrame.subdir = $$PWD/GUIFrame
GUIDialog.subdir = $$PWD/GUIDialog
HDF5IO.subdir = $$PWD/HDF5IO
OperatorsInterface.subdir = $$PWD/OperatorsInterface
OperatorsModel.subdir = $$PWD/OperatorsModel
OperatorsGUI.subdir = $$PWD/OperatorsGUI
MeshApp.subdir = $$PWD/MeshApp

FITK_Interface.depends = FITK_Kernel
FITK_Component.depends = FITK_Interface
ModelData.depends = FITK_Interface
GraphData.depends = FITK_Component ModelData
GUIWidget.depends = GraphData ModelData FITK_Component
GUIFrame.depends = GUIWidget FITK_Component
GUIDialog.depends = GraphData ModelData
HDF5IO.depends = FITK_Interface
OperatorsInterface.depends = FITK_Kernel
OperatorsModel.depends = OperatorsInterface GUIFrame GUIDialog HDF5IO
OperatorsGUI.depends = OperatorsInterface GUIFrame GraphData ModelData
FITK_Plugins.depends = GUIFrame OperatorsInterface GraphData ModelData FITK_Component
MeshApp.depends = OperatorsModel OperatorsGUI GUIFrame HDF5IO
