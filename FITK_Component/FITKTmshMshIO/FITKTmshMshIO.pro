TEMPLATE    =  lib
CONFIG     +=  c++11
CONFIG     +=  qt
TARGET      =  FITKTmshMshIO
QT         +=  core widgets gui
DEFINES    +=  FITKTmshMshIO_API

unix:!mac{ QMAKE_LFLAGS += "-Wl,-rpath,\'\$$ORIGIN/\'" }

include(./FITKTmshMshIO.pri)
include(./vtk.pri)

win32{

    INCLUDEPATH    +=  ./   \
                       ../  \
                       ../../ \

    Release:DESTDIR         = ../../output/bin
    Release:MOC_DIR         = ../../generate/FITKTmshMshIO/release/moc
    Release:RCC_DIR         = ../../generate/FITKTmshMshIO/release/rcc
    Release:UI_DIR          = ../../generate/FITKTmshMshIO/release/qui
    Release:OBJECTS_DIR     = ../../generate/FITKTmshMshIO/release/obj
    Release:LIBS +=  \
        -L../../output/bin  \
        -lFITKAppFramework \
        -lFITKCore \
        -lFITKAdaptor \
        -lFITKInterfaceIO \
        -lFITKInterfaceModel \
        -lFITKInterfaceMesh \

    Debug:CONFIG            +=  console
    Debug:DESTDIR         = ../../output/bin_d
    Debug:MOC_DIR         = ../../generate/FITKTmshMshIO/debug/moc
    Debug:RCC_DIR         = ../../generate/FITKTmshMshIO/debug/rcc
    Debug:UI_DIR          = ../../generate/FITKTmshMshIO/debug/qui
    Debug:OBJECTS_DIR     = ../../generate/FITKTmshMshIO/debug/obj
    Debug:LIBS +=  \
        -L../../output/bin_d \
        -lFITKAppFramework \
        -lFITKCore \
        -lFITKAdaptor \
        -lFITKInterfaceIO \
        -lFITKInterfaceModel \
        -lFITKInterfaceMesh \

    message("Windows FITKTmshMshIO generated")
}

unix{

    INCLUDEPATH    +=   ./  \
                        ../ \
                        ../../ \

    CONFIG          += console
    CONFIG          += plugin
    DESTDIR         = ../../output/bin
    MOC_DIR         = ../../generate/FITKTmshMshIO/release/moc
    UI_DIR          = ../../generate/FITKTmshMshIO/release/qui
    RCC_DIR         = ../../generate/FITKTmshMshIO/release/rcc
    OBJECTS_DIR     = ../../generate/FITKTmshMshIO/release/obj
    LIBS += \
        -L../../output/bin \
        -lFITKAppFramework \
        -lFITKCore \
        -lFITKAdaptor \
        -lFITKInterfaceIO \
        -lFITKInterfaceModel \
        -lFITKInterfaceMesh \
  

    message("Linux FITKTmshMshIO generated")
}

