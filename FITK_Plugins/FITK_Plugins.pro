TEMPLATE = subdirs

SUBDIRS += \
    PluginAbstract \
    PluginGeoAbstract \
    PluginGridAbstract \
    PluginGeoOCC \
    PluginGmshExec \
    PluginTmshExec

PluginAbstract.subdir = $$PWD/PluginAbstract
PluginGeoAbstract.subdir = $$PWD/PluginGeoAbstract
PluginGridAbstract.subdir = $$PWD/PluginGridAbstract
PluginGeoOCC.subdir = $$PWD/PluginGeoOCC
PluginGmshExec.subdir = $$PWD/PluginGmshExec
PluginTmshExec.subdir = $$PWD/PluginTmshExec

PluginGeoAbstract.depends = PluginAbstract
PluginGridAbstract.depends = PluginAbstract
PluginGeoOCC.depends = PluginGeoAbstract PluginAbstract
PluginGmshExec.depends = PluginGridAbstract PluginAbstract
PluginTmshExec.depends = PluginGridAbstract PluginAbstract

# Optional modules (currently disabled)
# SUBDIRS += PluginFastCAEGrid PluginDFCDGrid
# PluginFastCAEGrid.subdir = $$PWD/PluginFastCAEGrid
# PluginDFCDGrid.subdir = $$PWD/PluginDFCDGrid
