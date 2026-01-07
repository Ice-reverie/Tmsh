TEMPLATE    =   lib
CONFIG      +=  c++11
CONFIG      +=  qt
TARGET      =   OperatorsGUI
QT          +=  core widgets gui
DEFINES     +=  OperatorsGUI_API
DEFINES += _USE_MATH_DEFINES

unix:!mac{ QMAKE_LFLAGS += "-Wl,-rpath,\'\$$ORIGIN/\'" }

include(./vtk.pri)
include(./OperatorsGUI.pri)

win32{

    INCLUDEPATH    +=  ./   \
                       ../  \
                       ../../ \
                       ../Tools/Win64/SARibbon/include/SARibbon-2.0.1/  \

    Release:DESTDIR         = ../output/bin
    Release:MOC_DIR         = ../generate/OperatorsGUI/release/moc
    Release:RCC_DIR         = ../generate/OperatorsGUI/release/rcc
    Release:UI_DIR          = ../generate/OperatorsGUI/release/qui
    Release:OBJECTS_DIR     = ../generate/OperatorsGUI/release/obj
    Release:LIBS +=  \
        -L../output/bin  \
        -lFITKAppFramework \
        -lFITKWidget \
        -lFITKCore \
        -lFITKRenderWindowVTK \
        -lFITKInterfaceGeometry \
        -lGUIFrame \
        -lOperatorsInterface \
        -lGraphDataAdaptor \
        -lGraphDataProvider \
        -lGraphDataPick \
        -lModelData \
        

    Debug:CONFIG            +=  console
    Debug:DESTDIR         = ../output/bin_d
    Debug:MOC_DIR         = ../generate/OperatorsGUI/debug/moc
    Debug:RCC_DIR         = ../generate/OperatorsGUI/debug/rcc
    Debug:UI_DIR          = ../generate/OperatorsGUI/debug/qui
    Debug:OBJECTS_DIR     = ../generate/OperatorsGUI/debug/obj
    Debug:LIBS +=  \
        -L../output/bin_d \
        -lFITKAppFramework \
        -lFITKWidget \
        -lFITKCore \
        -lFITKRenderWindowVTK \
        -lFITKInterfaceGeometry \
        -lGUIFrame \
        -lOperatorsInterface \
        -lGraphDataAdaptor \
        -lGraphDataProvider \
        -lGraphDataPick \
        -lModelData \

    message("Windows OperatorsGUI generated")
}

unix{

    INCLUDEPATH    +=   ./  \
                        ../ \
                        ../Tools/Linux64/SARibbon/include/SARibbon-2.0.1/  \

    CONFIG          += console
    CONFIG          += plugin
    DESTDIR         = ../output/bin
    MOC_DIR         = ../generate/OperatorsGUI/release/moc
    UI_DIR          = ../generate/OperatorsGUI/release/qui
    RCC_DIR         = ../generate/OperatorsGUI/release/rcc
    OBJECTS_DIR     = ../generate/OperatorsGUI/release/obj
    LIBS += \
        -L../output/bin \
        -lFITKAppFramework \
        -lFITKWidget \
        -lFITKCore \
        -lFITKRenderWindowVTK \
        -lFITKInterfaceGeometry \
        -lGUIFrame \
        -lOperatorsInterface \
        -lGraphDataAdaptor \
        -lGraphDataProvider \
        -lGraphDataPick \
        -lModelData \
        
    message("Linux OperatorsGUI generated")
}
