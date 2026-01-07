TEMPLATE    =  lib
CONFIG     +=  c++11
CONFIG     +=  qt
TARGET      =  PluginAbstract
QT         +=  core widgets gui
DEFINES    +=  PluginAbstract_API

unix:!mac{ QMAKE_LFLAGS += "-Wl,-rpath,\'\$$ORIGIN/\'" }

include(./PluginAbstract.pri)

win32{

    INCLUDEPATH    +=  ./   \
                       ../  \
                       ../../ \
                       ../../Tools/Win64/SARibbon/include/SARibbon-2.0.1/  \

    Release:DESTDIR         = ../../output/bin
    Release:MOC_DIR         = ../../generate/PluginAbstract/release/moc
    Release:RCC_DIR         = ../../generate/PluginAbstract/release/rcc
    Release:UI_DIR          = ../../generate/PluginAbstract/release/qui
    Release:OBJECTS_DIR     = ../../generate/PluginAbstract/release/obj
    Release:LIBS +=  \
        -L../../output/bin  \
        -lFITKAppFramework \
        -lFITKCore \
        -lGUIFrame \


    Debug:CONFIG            +=  console
    Debug:DESTDIR         = ../../output/bin_d
    Debug:MOC_DIR         = ../../generate/PluginAbstract/debug/moc
    Debug:RCC_DIR         = ../../generate/PluginAbstract/debug/rcc
    Debug:UI_DIR          = ../../generate/PluginAbstract/debug/qui
    Debug:OBJECTS_DIR     = ../../generate/PluginAbstract/debug/obj
    Debug:LIBS +=  \
        -L../../output/bin_d \
        -lFITKAppFramework \
        -lFITKCore \
        -lGUIFrame \

    message("Windows PluginAbstract generated")
}

unix{

    INCLUDEPATH    +=   ./  \
                        ../ \
                        ../../ \
                         ../../Tools/Linux64/SARibbon/include/SARibbon-2.0.1/  \

    CONFIG          += console
    CONFIG          += plugin
    DESTDIR         = ../../output/bin
    MOC_DIR         = ../../generate/PluginAbstract/release/moc
    UI_DIR          = ../../generate/PluginAbstract/release/qui
    RCC_DIR         = ../../generate/PluginAbstract/release/rcc
    OBJECTS_DIR     = ../../generate/PluginAbstract/release/obj
    LIBS += \
        -L../../output/bin \
        -lFITKAppFramework \
        -lFITKCore \
        -lGUIFrame \
        
    message("Linux PluginAbstract generated")
}

