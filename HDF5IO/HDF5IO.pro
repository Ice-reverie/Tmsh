TEMPLATE    =  lib
CONFIG     +=  c++11
CONFIG     +=  qt
TARGET      =  HDF5IO
QT         +=  core widgets gui
DEFINES    +=  HDF5IO_API

unix:!mac{ QMAKE_LFLAGS += "-Wl,-rpath,\'\$$ORIGIN/\'" }

include(./HDF5IO.pri)
include(./hdf5.pri)

win32{

    INCLUDEPATH    +=  ./   \
                                          ../  \

    Release:DESTDIR         = ../output/bin
    Release:MOC_DIR         = ../generate/HDF5IO/release/moc
    Release:RCC_DIR         = ../generate/HDF5IO/release/rcc
    Release:UI_DIR          = ../generate/HDF5IO/release/qui
    Release:OBJECTS_DIR     = ../generate/HDF5IO/release/obj
    Release:LIBS +=  \
        -L../output/bin  \
        -lFITKCore \
        -lFITKAppFramework \
        -lFITKInterfaceIO \
        -lFITKAdaptor \


    Debug:CONFIG            +=  console
    Debug:DESTDIR         = ../output/bin_d
    Debug:MOC_DIR         = ../generate/HDF5IO/debug/moc
    Debug:RCC_DIR         = ../generate/HDF5IO/debug/rcc
    Debug:UI_DIR          = ../generate/HDF5IO/debug/qui
    Debug:OBJECTS_DIR     = ../generate/HDF5IO/debug/obj
    Debug:LIBS +=  \
        -L../output/bin_d \
        -lFITKCore \
        -lFITKAppFramework \
        -lFITKInterfaceIO \
        -lFITKAdaptor \


    message("Windows HDF5IO generated")
}

unix{

    INCLUDEPATH    +=   ./  \
                                           ../ \

    CONFIG          += console
    CONFIG          += plugin
    DESTDIR         = ../output/bin
    MOC_DIR         = ../generate/HDF5IO/release/moc
    UI_DIR          = ../generate/HDF5IO/release/qui
    RCC_DIR         = ../generate/HDF5IO/release/rcc
    OBJECTS_DIR     = ../generate/HDF5IO/release/obj
    LIBS += \
        -lFITKCore \
        -lFITKAppFramework \
        -lFITKInterfaceIO \
        -lFITKAdaptor \

    message("Linux HDF5IO generated")
}

