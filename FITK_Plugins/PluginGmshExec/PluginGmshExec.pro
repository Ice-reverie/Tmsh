TEMPLATE    =  lib
CONFIG     +=  c++11
CONFIG     +=  qt
TARGET      =  PluginGmshExec
QT         +=  core widgets gui
DEFINES    +=  PluginGmshExec_API

unix:!mac{ QMAKE_LFLAGS += "-Wl,-rpath,\'\$$ORIGIN/\'" }

include(./PluginGmshExec.pri)

win32{

    INCLUDEPATH    +=  ./   \
                       ../  \
                       ../../ \
                       ../../Tools/Win64/SARibbon/include/SARibbon-2.0.1/  \

    Release:DESTDIR         = ../../output/bin/Plugins
    Release:MOC_DIR         = ../../generate/PluginGmshExec/release/moc
    Release:RCC_DIR         = ../../generate/PluginGmshExec/release/rcc
    Release:UI_DIR          = ../../generate/PluginGmshExec/release/qui
    Release:OBJECTS_DIR     = ../../generate/PluginGmshExec/release/obj
    Release:LIBS +=  \
        -L../../output/bin  \
        -lFITKAppFramework \
        -lPluginAbstract \
        -lPluginGridAbstract \
        -lFITKGmshExeDriver \
        -lModelData \
        -lFITKInterfaceMeshGen \
        -lFITKCore \
        -lGUIFrame \
        -lFITKInterfaceGeometry \
        -lFITKCGNSIO \
        -lOperatorsInterface \
        -lFITKInterfaceMesh \
        -lFITKInterfaceModel \
        -lGraphDataPick \
        -lFITKWidget \


    Debug:CONFIG            +=  console
    Debug:DESTDIR         = ../../output/bin_d/Plugins
    Debug:MOC_DIR         = ../../generate/PluginGmshExec/debug/moc
    Debug:RCC_DIR         = ../../generate/PluginGmshExec/debug/rcc
    Debug:UI_DIR          = ../../generate/PluginGmshExec/debug/qui
    Debug:OBJECTS_DIR     = ../../generate/PluginGmshExec/debug/obj
    Debug:LIBS +=  \
        -L../../output/bin_d \
        -lFITKAppFramework \
        -lPluginAbstract \
        -lPluginGridAbstract \
        -lFITKGmshExeDriver \
        -lModelData \
        -lFITKInterfaceMeshGen \
        -lFITKCore \
        -lGUIFrame \
        -lFITKInterfaceGeometry \
        -lFITKCGNSIO \
        -lOperatorsInterface \
        -lFITKInterfaceMesh \
        -lFITKInterfaceModel \
        -lGraphDataPick \
        -lFITKWidget \

    message("Windows PluginGmshExec generated")
}

unix{

    INCLUDEPATH    +=   ./  \
                        ../ \
                        ../../ \
                        ../../Tools/Win64/SARibbon/include/SARibbon-2.0.1/  \

    CONFIG          += console
    CONFIG          += plugin
    DESTDIR         = ../../output/bin/Plugins
    MOC_DIR         = ../../generate/PluginGmshExec/release/moc
    UI_DIR          = ../../generate/PluginGmshExec/release/qui
    RCC_DIR         = ../../generate/PluginGmshExec/release/rcc
    OBJECTS_DIR     = ../../generate/PluginGmshExec/release/obj
    LIBS += \
        -L../../output/bin \
        -lFITKAppFramework \
        -lPluginAbstract \
        -lPluginGridAbstract \
        -lFITKGmshExeDriver \
        -lModelData \
        -lFITKInterfaceMeshGen \
        -lFITKCore \
        -lGUIFrame \
        -lFITKInterfaceGeometry \
        -lFITKCGNSIO \
        -lOperatorsInterface \
        -lFITKInterfaceMesh \
        -lFITKInterfaceModel \
        -lGraphDataPick \
        -lFITKWidget \

    message("Linux PluginGmshExec generated")
}

