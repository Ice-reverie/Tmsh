TEMPLATE    =  lib
CONFIG     +=  c++11
CONFIG     +=  qt
TARGET      =  GraphDataAdaptor
QT         +=  core widgets gui
DEFINES    +=  GraphDataAdaptor_API

include(./vtk.pri)
include(./GraphDataAdaptor.pri)

win32{

    INCLUDEPATH    +=  ./   \
                       ../  \
                       ../../  \

    Release:DESTDIR         = ../../output/bin
    Release:MOC_DIR         = ../../generate/GraphDataAdaptor/release/moc
    Release:RCC_DIR         = ../../generate/GraphDataAdaptor/release/rcc
    Release:UI_DIR          = ../../generate/GraphDataAdaptor/release/qui
    Release:OBJECTS_DIR     = ../../generate/GraphDataAdaptor/release/obj
    Release:LIBS += \
        -L../../output/bin \
        -lFITKVTKAlgorithm \
        -lFITKCore \
        -lFITKAppFramework \
        -lFITKAdaptor \
        -lFITKInterfaceModel \
        -lFITKInterfaceGeometry \
        -lFITKInterfaceMesh \
        -lFITKRenderWindowVTK \
        -lModelData \
        

    Debug:CONFIG            +=  console
    Debug:DESTDIR         = ../../output/bin_d
    Debug:MOC_DIR         = ../../generate/GraphDataAdaptor/debug/moc
    Debug:RCC_DIR         = ../../generate/GraphDataAdaptor/debug/rcc
    Debug:UI_DIR          = ../../generate/GraphDataAdaptor/debug/qui
    Debug:OBJECTS_DIR     = ../../generate/GraphDataAdaptor/debug/obj
    Debug:LIBS += \
        -L../../output/bin_d \
        -lFITKVTKAlgorithm \
        -lFITKCore \
        -lFITKAppFramework \
        -lFITKAdaptor \
        -lFITKInterfaceModel \
        -lFITKInterfaceGeometry \
        -lFITKInterfaceMesh \
        -lFITKRenderWindowVTK \
        -lModelData \

    message("Windows GraphDataAdaptor generated")
}

unix{

    INCLUDEPATH    +=   ./  \
                        ../ \
                        ../../ \

    CONFIG          += console
    CONFIG          += plugin
    DESTDIR         = ../../output/bin
    MOC_DIR         = ../../generate/GraphDataAdaptor/release/moc
    UI_DIR          = ../../generate/GraphDataAdaptor/release/qui
    RCC_DIR         = ../../generate/GraphDataAdaptor/release/rcc
    OBJECTS_DIR     = ../../generate/GraphDataAdaptor/release/obj
    LIBS += \
        -L../../output/bin \
        -lFITKVTKAlgorithm \
        -lFITKCore \
        -lFITKAppFramework \
        -lFITKAdaptor \
        -lFITKInterfaceModel \
        -lFITKInterfaceGeometry \
        -lFITKInterfaceMesh \
        -lFITKRenderWindowVTK \
        -lModelData \

    message("Linux FITKFluidVTKGraphAdptor generated")
}

