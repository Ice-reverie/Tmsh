TEMPLATE    =  lib
CONFIG     +=  c++11
CONFIG     +=  qt
TARGET      =  OperatorsModel
QT         +=  core gui widgets
DEFINES    +=  OperatorsModel_API

win32{
    QMAKE_CXXFLAGS += /MP
}

unix:!mac{ QMAKE_LFLAGS += "-Wl,-rpath,\'\$$ORIGIN/\'" }

include(./OperatorsModel.pri)

win32{

    INCLUDEPATH    +=  ./   \
                       ../  \ 
                       ../Tools/Win64/SARibbon/include/SARibbon-2.0.1/  \


    Release:DESTDIR         = ../output/bin
    Release:MOC_DIR         = ../generate/OperatorsModel/release/moc
    Release:RCC_DIR         = ../generate/OperatorsModel/release/rcc
    Release:UI_DIR          = ../generate/OperatorsModel/release/qui
    Release:OBJECTS_DIR     = ../generate/OperatorsModel/release/obj
    Release:LIBS +=  \
        -L../output/bin  \
        -lFITKCore \
        -lFITKAppFramework \
        -lFITKInterfaceGeometry \
        -lFITKWidget \
        -lGUIFrame \ 
        -lGUIWidget \ 
        -lModelData \
        -lOperatorsInterface \
        -lGraphDataPick \
        -lHDF5IO \
        -lGUIDialog \
        #-lFITKAbaqusIOINP \
        #-lFITKAbaqusData \
        -lFITKInterfaceModel \
        #-lFITKInterfaceStructural \
        -lFITKCGNSIO \

    Debug:CONFIG            +=  console
    Debug:DESTDIR         = ../output/bin_d
    Debug:MOC_DIR         = ../generate/OperatorsModel/debug/moc
    Debug:RCC_DIR         = ../generate/OperatorsModel/debug/rcc
    Debug:UI_DIR          = ../generate/OperatorsModel/debug/qui
    Debug:OBJECTS_DIR     = ../generate/OperatorsModel/debug/obj
    Debug:LIBS +=  \
        -L../output/bin_d \
        -lFITKCore \
        -lFITKAppFramework \
        -lFITKInterfaceGeometry \
        -lFITKWidget \
        -lGUIFrame \
        -lGUIWidget \ 
        -lModelData \
        -lOperatorsInterface \
        -lGraphDataPick \
        -lHDF5IO \
        -lGUIDialog \
        #-lFITKAbaqusIOINP \
        #-lFITKAbaqusData \
        -lFITKInterfaceModel \
        #-lFITKInterfaceStructural \
        -lFITKCGNSIO \

    message("Windows OperatorsModel generated")
}

unix{

    INCLUDEPATH    +=   ./  \
                        ../ \ 
                        ../Tools/Linux64/SARibbon/include/SARibbon-2.0.1/  \
 

    CONFIG          += console
    CONFIG          += plugin
    DESTDIR         = ../output/bin
    MOC_DIR         = ../generate/OperatorsModel/release/moc
    UI_DIR          = ../generate/OperatorsModel/release/qui
    RCC_DIR         = ../generate/OperatorsModel/release/rcc
    OBJECTS_DIR     = ../generate/OperatorsModel/release/obj
    LIBS += \
        -L../output/bin \
        -lFITKCore \
        -lFITKAppFramework \
        -lFITKInterfaceGeometry \
        -lFITKWidget \
        -lGUIFrame \ 
        -lGUIWidget \ 
        -lModelData \
        -lOperatorsInterface \
        -lGraphDataPick \
        -lHDF5IO \
        -lGUIDialog \
        #-lFITKAbaqusIOINP \
        #-lFITKAbaqusData \
        -lFITKInterfaceModel \
        #-lFITKInterfaceStructural \
        -lFITKCGNSIO \
        -lGUIPostProcessingDialog \

    message("Linux OperatorsModel generated")
}

