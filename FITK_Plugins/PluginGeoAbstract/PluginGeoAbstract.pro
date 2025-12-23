TEMPLATE    =  lib
CONFIG     +=  c++11
CONFIG     +=  qt
TARGET      =  PluginGeoAbstract
QT         +=  core widgets gui
DEFINES    +=  PluginGeoAbstract_API

unix:!mac{ QMAKE_LFLAGS += "-Wl,-rpath,\'\$$ORIGIN/\'" }

include(./PluginGeoAbstract.pri)

win32{

    INCLUDEPATH    +=  ./   \
                       ../  \
                       ../../ \
                       ../../Tools/Win64/SARibbon/include/SARibbon-2.0.1/  \

    Release:DESTDIR         = ../../output/bin
    Release:MOC_DIR         = ../../generate/PluginGeoAbstract/release/moc
    Release:RCC_DIR         = ../../generate/PluginGeoAbstract/release/rcc
    Release:UI_DIR          = ../../generate/PluginGeoAbstract/release/qui
    Release:OBJECTS_DIR     = ../../generate/PluginGeoAbstract/release/obj
    Release:LIBS +=  \
        -L../../output/bin  \
        -lFITKAppFramework \
        -lFITKCore \
        -lGUIFrame \
        -lPluginAbstract \
        -lFITKInterfaceGeometry \
        -lGraphDataPick \
        -lFITKWidget \
        -lModelData \
        -lOperatorsInterface \

    Debug:CONFIG            +=  console
    Debug:DESTDIR         = ../../output/bin_d
    Debug:MOC_DIR         = ../../generate/PluginGeoAbstract/debug/moc
    Debug:RCC_DIR         = ../../generate/PluginGeoAbstract/debug/rcc
    Debug:UI_DIR          = ../../generate/PluginGeoAbstract/debug/qui
    Debug:OBJECTS_DIR     = ../../generate/PluginGeoAbstract/debug/obj
    Debug:LIBS +=  \
        -L../../output/bin_d \
        -lFITKAppFramework \
        -lFITKCore \
        -lGUIFrame \
        -lPluginAbstract \
        -lFITKInterfaceGeometry \
        -lGraphDataPick \
        -lFITKWidget \
        -lModelData \
        -lOperatorsInterface \

    message("Windows PluginGeoAbstract generated")
}

unix{

    INCLUDEPATH    +=   ./  \
                        ../ \
                        ../../ \
                         ../../Tools/Linux64/SARibbon/include/SARibbon-2.0.1/  \

    CONFIG          += console
    CONFIG          += plugin
    DESTDIR         = ../../output/bin
    MOC_DIR         = ../../generate/PluginGeoAbstract/release/moc
    UI_DIR          = ../../generate/PluginGeoAbstract/release/qui
    RCC_DIR         = ../../generate/PluginGeoAbstract/release/rcc
    OBJECTS_DIR     = ../../generate/PluginGeoAbstract/release/obj
    LIBS += \
        -L../../output/bin \
        -lFITKAppFramework \
        -lFITKCore \
        -lGUIFrame \
        -lPluginAbstract \
        -lFITKInterfaceGeometry \
        -lGraphDataPick \
        -lFITKWidget \
        -lModelData \
        -lOperatorsInterface \

    message("Linux PluginGeoAbstract generated")
}

