TEMPLATE    =  lib
CONFIG     +=  c++11
CONFIG     +=  qt
TARGET      =  PluginGridAbstract
QT         +=  core widgets gui
DEFINES    +=  PluginGridAbstract_API

unix:!mac{ QMAKE_LFLAGS += "-Wl,-rpath,\'\$$ORIGIN/\'" }

include(./PluginGridAbstract.pri)

win32{

    INCLUDEPATH    +=  ./   \
                       ../  \
                       ../../ \
                       ../../Tools/Win64/SARibbon/include/SARibbon-2.0.1/  \

    Release:DESTDIR         = ../../output/bin
    Release:MOC_DIR         = ../../generate/PluginGridAbstract/release/moc
    Release:RCC_DIR         = ../../generate/PluginGridAbstract/release/rcc
    Release:UI_DIR          = ../../generate/PluginGridAbstract/release/qui
    Release:OBJECTS_DIR     = ../../generate/PluginGridAbstract/release/obj
    Release:LIBS +=  \
        -L../../output/bin  \
        -lFITKAppFramework \
        -lFITKCore \
        -lGUIFrame \
        -lPluginAbstract \
        -lModelData \
        -lFITKInterfaceMeshGen \
        -lFITKInterfaceGeometry \
        -lFITKInterfaceModel \
        -lFITKInterfaceMesh \

    Debug:CONFIG            +=  console
    Debug:DESTDIR         = ../../output/bin_d
    Debug:MOC_DIR         = ../../generate/PluginGridAbstract/debug/moc
    Debug:RCC_DIR         = ../../generate/PluginGridAbstract/debug/rcc
    Debug:UI_DIR          = ../../generate/PluginGridAbstract/debug/qui
    Debug:OBJECTS_DIR     = ../../generate/PluginGridAbstract/debug/obj
    Debug:LIBS +=  \
        -L../../output/bin_d \
        -lFITKAppFramework \
        -lFITKCore \
        -lGUIFrame \
        -lPluginAbstract \
        -lModelData \
        -lFITKInterfaceMeshGen \
        -lFITKInterfaceGeometry \
        -lFITKInterfaceModel \
        -lFITKInterfaceMesh \

    message("Windows PluginGridAbstract generated")
}

unix{

    INCLUDEPATH    +=   ./  \
                        ../ \
                        ../../ \
                         ../../Tools/Linux64/SARibbon/include/SARibbon-2.0.1/  \

    CONFIG          += console
    CONFIG          += plugin
    DESTDIR         = ../../output/bin
    MOC_DIR         = ../../generate/PluginGridAbstract/release/moc
    UI_DIR          = ../../generate/PluginGridAbstract/release/qui
    RCC_DIR         = ../../generate/PluginGridAbstract/release/rcc
    OBJECTS_DIR     = ../../generate/PluginGridAbstract/release/obj
    LIBS += \
        -L../../output/bin \
        -lFITKAppFramework \
        -lFITKCore \
        -lGUIFrame \
        -lPluginAbstract \
        -lModelData \
        -lFITKInterfaceMeshGen \
        -lFITKInterfaceGeometry \
        -lFITKInterfaceModel \
        -lFITKInterfaceMesh \

    message("Linux PluginGridAbstract generated")
}



