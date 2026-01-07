TEMPLATE    =  lib
CONFIG     +=  c++11
CONFIG     +=  qt
TARGET      =  PluginTmshExec
QT         +=  core widgets gui
DEFINES    +=  PluginTmshExec_API

unix:!mac{ QMAKE_LFLAGS += "-Wl,-rpath,\'\$$ORIGIN/\'" }

include(./PluginTmshExec.pri)

win32{

    INCLUDEPATH    +=  ./   \
                       ../  \
                       ../../ \
                       ../../Tools/Win64/SARibbon/include/SARibbon-2.0.1/  \

    Release:DESTDIR         = ../../output/bin/Plugins
    Release:MOC_DIR         = ../../generate/PluginTmshExec/release/moc
    Release:RCC_DIR         = ../../generate/PluginTmshExec/release/rcc
    Release:UI_DIR          = ../../generate/PluginTmshExec/release/qui
    Release:OBJECTS_DIR     = ../../generate/PluginTmshExec/release/obj
    Release:LIBS +=  \
        -L../../output/bin  \
        -lFITKAppFramework \
        -lPluginAbstract \
        -lPluginGridAbstract \
        -lFITKTmshExeDriver \
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
	-lFITKTmshMshIO \

    Debug:CONFIG            +=  console
    Debug:DESTDIR         = ../../output/bin_d/Plugins
    Debug:MOC_DIR         = ../../generate/PluginTmshExec/debug/moc
    Debug:RCC_DIR         = ../../generate/PluginTmshExec/debug/rcc
    Debug:UI_DIR          = ../../generate/PluginTmshExec/debug/qui
    Debug:OBJECTS_DIR     = ../../generate/PluginTmshExec/debug/obj
    Debug:LIBS +=  \
        -L../../output/bin_d \
        -lFITKAppFramework \
        -lPluginAbstract \
        -lPluginGridAbstract \
        -lFITKTmshExeDriver \
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
	-lFITKTmshMshIO \

    message("Windows PluginTmshExec generated")
}

unix{

    INCLUDEPATH    +=   ./  \
                        ../ \
                        ../../ \
                        ../../Tools/Win64/SARibbon/include/SARibbon-2.0.1/  \

    CONFIG          += console
    CONFIG          += plugin
    DESTDIR         = ../../output/bin/Plugins
    MOC_DIR         = ../../generate/PluginTmshExec/release/moc
    UI_DIR          = ../../generate/PluginTmshExec/release/qui
    RCC_DIR         = ../../generate/PluginTmshExec/release/rcc
    OBJECTS_DIR     = ../../generate/PluginTmshExec/release/obj
    LIBS += \
        -L../../output/bin \
        -lFITKAppFramework \
        -lPluginAbstract \
        -lPluginGridAbstract \
        -lFITKTmshExeDriver \
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
	-lFITKTmshMshIO \

    message("Linux PluginTmshExec generated")
}

