TEMPLATE    =  lib
CONFIG     +=  c++11
CONFIG     +=  qt
TARGET      =  GUIDialog
QT         +=  core gui widgets
DEFINES    +=  GUIDialog_API

win32{
    QMAKE_CXXFLAGS += /MP
}

unix:!mac{ QMAKE_LFLAGS += "-Wl,-rpath,\'\$$ORIGIN/\'" }

include(./GUIDialog.pri)

win32{

    INCLUDEPATH    +=  ./   \
                       ../  \ 

    Release:DESTDIR         = ../output/bin
    Release:MOC_DIR         = ../generate/GUIDialog/release/moc
    Release:RCC_DIR         = ../generate/GUIDialog/release/rcc
    Release:UI_DIR          = ../generate/GUIDialog/release/qui
    Release:OBJECTS_DIR     = ../generate/GUIDialog/release/obj
    Release:LIBS +=  \
        -L../output/bin  \
        -lFITKCore \
        -lFITKInterfaceGeometry \
        -lFITKAppFramework \
        -lModelData \
        -lFITKInterfaceModel \
        -lGraphDataPick \

    Debug:CONFIG            +=  console
    Debug:DESTDIR         = ../output/bin_d
    Debug:MOC_DIR         = ../generate/GUIDialog/debug/moc
    Debug:RCC_DIR         = ../generate/GUIDialog/debug/rcc
    Debug:UI_DIR          = ../generate/GUIDialog/debug/qui
    Debug:OBJECTS_DIR     = ../generate/GUIDialog/debug/obj
    Debug:LIBS +=  \
        -L../output/bin_d \
        -lFITKCore \
        -lFITKInterfaceGeometry \
        -lFITKAppFramework \
        -lModelData \
        -lFITKInterfaceModel \
        -lGraphDataPick \

    message("Windows GUIDialog generated")
}

unix{

    INCLUDEPATH    +=   ./  \
                        ../ \ 

    CONFIG          += console
    CONFIG          += plugin
    DESTDIR         = ../output/bin
    MOC_DIR         = ../generate/GUIDialog/release/moc
    UI_DIR          = ../generate/GUIDialog/release/qui
    RCC_DIR         = ../generate/GUIDialog/release/rcc
    OBJECTS_DIR     = ../generate/GUIDialog/release/obj
    LIBS += \
        -L../output/bin \
        -lFITKCore \
        -lFITKInterfaceGeometry \
        -lFITKAppFramework \
        -lModelData \
        -lFITKInterfaceModel \
        -lGraphDataPick \

    message("Linux GUIDialog generated")
}

