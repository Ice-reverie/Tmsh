TEMPLATE    =  lib
CONFIG     +=  c++11
CONFIG     +=  qt
TARGET      =  ModelData
QT         +=  core gui widgets
DEFINES    +=  ModelData_API

win32{
    QMAKE_CXXFLAGS += /MP
}

unix:!mac{ QMAKE_LFLAGS += "-Wl,-rpath,\'\$$ORIGIN/\'" }

include(./ModelData.pri)

win32{

    INCLUDEPATH    +=  ./   \
                       ../  \ 

    Release:DESTDIR         = ../output/bin
    Release:MOC_DIR         = ../generate/ModelData/release/moc
    Release:RCC_DIR         = ../generate/ModelData/release/rcc
    Release:UI_DIR          = ../generate/ModelData/release/qui
    Release:OBJECTS_DIR     = ../generate/ModelData/release/obj
    Release:LIBS +=  \
        -L../output/bin  \
        -lFITKCore \
        -lFITKAppFramework \
        -lFITKInterfaceModel \
        -lFITKInterfaceGeometry \
        #-lFITKAbaqusData \
        -lFITKInterfaceMesh \
        #-lFITKInterfaceStructural \



    Debug:CONFIG            +=  console
    Debug:DESTDIR         = ../output/bin_d
    Debug:MOC_DIR         = ../generate/ModelData/debug/moc
    Debug:RCC_DIR         = ../generate/ModelData/debug/rcc
    Debug:UI_DIR          = ../generate/ModelData/debug/qui
    Debug:OBJECTS_DIR     = ../generate/ModelData/debug/obj
    Debug:LIBS +=  \
        -L../output/bin_d \
        -lFITKCore \
        -lFITKAppFramework \
        -lFITKInterfaceModel \
        -lFITKInterfaceGeometry \
        #-lFITKAbaqusData \
        -lFITKInterfaceMesh \
        #-lFITKInterfaceStructural \


    message("Windows ModelData generated")
}

unix{

    INCLUDEPATH    +=   ./  \
                        ../ \ 

    CONFIG          += console
    CONFIG          += plugin
    DESTDIR         = ../output/bin
    MOC_DIR         = ../generate/ModelData/release/moc
    UI_DIR          = ../generate/ModelData/release/qui
    RCC_DIR         = ../generate/ModelData/release/rcc
    OBJECTS_DIR     = ../generate/ModelData/release/obj
    LIBS += \
        -L../output/bin \
        -lFITKCore \
        -lFITKAppFramework \
        -lFITKInterfaceModel \
        -lFITKInterfaceGeometry \
        #-lFITKAbaqusData \
        -lFITKInterfaceMesh \
        #-lFITKInterfaceStructural \

    message("Linux ModelData generated")
}

