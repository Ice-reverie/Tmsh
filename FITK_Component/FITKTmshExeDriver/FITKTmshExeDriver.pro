TEMPLATE    =  lib
CONFIG     +=  c++11
CONFIG     +=  qt
TARGET      =  FITKTmshExeDriver
QT         +=  core widgets gui
DEFINES    +=  FITKTmshExeDriver_API

unix:!mac{ QMAKE_LFLAGS += "-Wl,-rpath,\'\$$ORIGIN/\'" }

include(./FITKTmshExeDriver.pri)

win32{

    INCLUDEPATH    +=  ./   \
                       ../  \
                       ../../ \

    Release:DESTDIR         = ../../output/bin
    Release:MOC_DIR         = ../../generate/FITKTmshExeDriver/release/moc
    Release:RCC_DIR         = ../../generate/FITKTmshExeDriver/release/rcc
    Release:UI_DIR          = ../../generate/FITKTmshExeDriver/release/qui
    Release:OBJECTS_DIR     = ../../generate/FITKTmshExeDriver/release/obj
    Release:LIBS +=  \
        -L../../output/bin  \
        -lFITKCore \
        -lFITKAppFramework \
        -lFITKInterfaceMeshGen \
        -lFITKInterfaceModel \
        -lFITKInterfaceIO \
        -lFITKTmshMshIO \
        -lFITKWidget \
        -lFITKInterfaceGeometry \

    Debug:CONFIG            +=  console
    Debug:DESTDIR         = ../../output/bin_d
    Debug:MOC_DIR         = ../../generate/FITKTmshExeDriver/debug/moc
    Debug:RCC_DIR         = ../../generate/FITKTmshExeDriver/debug/rcc
    Debug:UI_DIR          = ../../generate/FITKTmshExeDriver/debug/qui
    Debug:OBJECTS_DIR     = ../../generate/FITKTmshExeDriver/debug/obj
    Debug:LIBS +=  \
        -L../../output/bin_d \
        -lFITKCore \
        -lFITKAppFramework \
        -lFITKInterfaceMeshGen \
        -lFITKInterfaceModel \
        -lFITKInterfaceIO \
        -lFITKTmshMshIO \
        -lFITKWidget \
        -lFITKInterfaceGeometry \

    message("Windows FITKTmshExeDriver generated")
}

unix{

    INCLUDEPATH    +=   ./  \
                        ../ \
                        ../../ \

    CONFIG          += console
    CONFIG          += plugin
    DESTDIR         = ../../output/bin
    MOC_DIR         = ../../generate/FITKTmshExeDriver/release/moc
    UI_DIR          = ../../generate/FITKTmshExeDriver/release/qui
    RCC_DIR         = ../../generate/FITKTmshExeDriver/release/rcc
    OBJECTS_DIR     = ../../generate/FITKTmshExeDriver/release/obj
    LIBS += \
        -L../../output/bin \
        -lFITKCore \
        -lFITKAppFramework \
        -lFITKInterfaceMeshGen \
        -lFITKInterfaceModel \
        -lFITKInterfaceIO \
        -lFITKTmshMshIO \
        -lFITKWidget \
        -lFITKInterfaceGeometry \

    message("Linux FITKTmshExeDriver generated")
}

FORMS += \
    GUIDetailedParameters.ui

HEADERS += \
    GUIDetailedParameters.h

SOURCES += \
    GUIDetailedParameters.cpp

