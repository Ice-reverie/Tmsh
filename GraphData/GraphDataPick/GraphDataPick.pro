TEMPLATE    =  lib
CONFIG     +=  c++11
CONFIG     +=  qt
TARGET      =  GraphDataPick
QT         +=  core widgets gui
DEFINES    +=  GraphDataPick_API

include(./vtk.pri)
include(./GraphDataPick.pri)

win32{

    INCLUDEPATH    +=  ./   \
                       ../  \
                       ../../  \

    Release:DESTDIR         = ../../output/bin
    Release:MOC_DIR         = ../../generate/GraphDataPick/release/moc
    Release:RCC_DIR         = ../../generate/GraphDataPick/release/rcc
    Release:UI_DIR          = ../../generate/GraphDataPick/release/qui
    Release:OBJECTS_DIR     = ../../generate/GraphDataPick/release/obj
    Release:LIBS += \
        -L../../output/bin \
        -lFITKAppFramework \
        -lFITKVTKAlgorithm \
        -lFITKCore \
        -lFITKAdaptor \
        -lFITKInterfaceModel \
        -lFITKInterfaceGeometry \
        -lFITKInterfaceMesh \
        -lFITKRenderWindowVTK \
        -lGraphDataAdaptor \
        

    Debug:CONFIG            +=  console
    Debug:DESTDIR         = ../../output/bin_d
    Debug:MOC_DIR         = ../../generate/GraphDataPick/debug/moc
    Debug:RCC_DIR         = ../../generate/GraphDataPick/debug/rcc
    Debug:UI_DIR          = ../../generate/GraphDataPick/debug/qui
    Debug:OBJECTS_DIR     = ../../generate/GraphDataPick/debug/obj
    Debug:LIBS += \
        -L../../output/bin_d \
        -lFITKAppFramework \
        -lFITKVTKAlgorithm \
        -lFITKCore \
        -lFITKAdaptor \
        -lFITKInterfaceModel \
        -lFITKInterfaceGeometry \
        -lFITKInterfaceMesh \
        -lFITKRenderWindowVTK \
        -lGraphDataAdaptor \

    message("Windows GraphDataPick generated")
}

unix{

    INCLUDEPATH    +=   ./  \
                        ../ \
                        ../../ \

    CONFIG          += console
    CONFIG          += plugin
    DESTDIR         = ../../output/bin
    MOC_DIR         = ../../generate/GraphDataPick/release/moc
    UI_DIR          = ../../generate/GraphDataPick/release/qui
    RCC_DIR         = ../../generate/GraphDataPick/release/rcc
    OBJECTS_DIR     = ../../generate/GraphDataPick/release/obj
    LIBS += \
        -L../../output/bin \
        -lFITKAppFramework \
        -lFITKVTKAlgorithm \
        -lFITKCore \
        -lFITKAdaptor \
        -lFITKInterfaceModel \
        -lFITKInterfaceGeometry \
        -lFITKInterfaceMesh \
        -lFITKRenderWindowVTK \
        -lGraphDataAdaptor \

    message("Linux FITKFluidVTKGraphAdptor generated")
}

