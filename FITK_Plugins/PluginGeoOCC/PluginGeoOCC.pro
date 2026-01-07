TEMPLATE    =  lib
CONFIG     +=  c++11
CONFIG     +=  qt
TARGET      =  PluginGeoOCC
QT         +=  core widgets gui
DEFINES    +=  PluginGeoOCC_API

unix:!mac{ QMAKE_LFLAGS += "-Wl,-rpath,\'\$$ORIGIN/\'" }

include(./PluginGeoOCC.pri)

win32{

    INCLUDEPATH    +=  ./   \
                       ../  \
                       ../../ \
                       ../../Tools/Win64/SARibbon/include/SARibbon-2.0.1/  \

    Release:DESTDIR         = ../../output/bin/Plugins
    Release:MOC_DIR         = ../../generate/PluginGeoOCC/release/moc
    Release:RCC_DIR         = ../../generate/PluginGeoOCC/release/rcc
    Release:UI_DIR          = ../../generate/PluginGeoOCC/release/qui
    Release:OBJECTS_DIR     = ../../generate/PluginGeoOCC/release/obj
    Release:LIBS +=  \
        -L../../output/bin  \
        -lFITKAppFramework \
        -lPluginAbstract \
        -lPluginGeoAbstract \
        -lFITKGeoCompOCC \
        -lFITKInterfaceGeometry \
        -lFITKCore \
        -lGraphDataPick \
        -lFITKWidget \
        -lModelData \
        -lOperatorsInterface \

    Debug:CONFIG            +=  console
    Debug:DESTDIR         = ../../output/bin_d/Plugins
    Debug:MOC_DIR         = ../../generate/PluginGeoOCC/debug/moc
    Debug:RCC_DIR         = ../../generate/PluginGeoOCC/debug/rcc
    Debug:UI_DIR          = ../../generate/PluginGeoOCC/debug/qui
    Debug:OBJECTS_DIR     = ../../generate/PluginGeoOCC/debug/obj
    Debug:LIBS +=  \
        -L../../output/bin_d \
        -lFITKAppFramework \
        -lPluginAbstract \
        -lPluginGeoAbstract \
        -lFITKGeoCompOCC \
        -lFITKInterfaceGeometry \
        -lFITKCore \
        -lGraphDataPick \
        -lFITKWidget \
        -lModelData \
        -lOperatorsInterface \

    message("Windows PluginGeoOCC generated")
}

unix{

    INCLUDEPATH    +=   ./  \
                        ../ \
                        ../../ \
                        ../../Tools/Win64/SARibbon/include/SARibbon-2.0.1/  \

    CONFIG          += console
    CONFIG          += plugin
    DESTDIR         = ../../output/bin/Plugins
    MOC_DIR         = ../../generate/PluginGeoOCC/release/moc
    UI_DIR          = ../../generate/PluginGeoOCC/release/qui
    RCC_DIR         = ../../generate/PluginGeoOCC/release/rcc
    OBJECTS_DIR     = ../../generate/PluginGeoOCC/release/obj
    LIBS += \
        -L../../output/bin \
        -lFITKAppFramework \
        -lPluginAbstract \
        -lPluginGeoAbstract \
        -lFITKGeoCompOCC \
        -lFITKInterfaceGeometry \
        -lFITKCore \
        -lGraphDataPick \
        -lFITKWidget \
        -lModelData \
        -lOperatorsInterface \


    message("Linux PluginGeoOCC generated")
}

