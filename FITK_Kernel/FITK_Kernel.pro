TEMPLATE = subdirs

SUBDIRS += \
    FITKCore \
    FITKAppFramework \
    FITKAdaptor \
    FITKPython

FITKCore.subdir = $$PWD/FITKCore
FITKAppFramework.subdir = $$PWD/FITKAppFramework
FITKAdaptor.subdir = $$PWD/FITKAdaptor
FITKPython.subdir = $$PWD/FITKPython

FITKAppFramework.depends = FITKCore
FITKAdaptor.depends = FITKCore
FITKPython.depends = FITKAppFramework FITKCore
