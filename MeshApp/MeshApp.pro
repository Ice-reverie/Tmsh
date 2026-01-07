TEMPLATE    =  app
CONFIG     +=  c++11
CONFIG     +=  qt
TARGET      =  MeshApp
QT         +=  core widgets gui 

unix:!mac{ QMAKE_LFLAGS += "-Wl,-rpath,\'\$$ORIGIN/\'" }

include(./vtk.pri)
include(./MeshApp.pri)

win32{

    INCLUDEPATH    +=  ./   \
                       ../  \
                       ../../ \
                       ../Tools/Win64/SARibbon/include/SARibbon-2.0.1/  \
                       
    Release:CONFIG            +=  console
    Release:DESTDIR         = ../output/bin
    Release:MOC_DIR         = ../generate/MeshApp/release/moc
    Release:RCC_DIR         = ../generate/MeshApp/release/rcc
    Release:UI_DIR          = ../generate/MeshApp/release/qui
    Release:OBJECTS_DIR     = ../generate/MeshApp/release/obj
    Release:LIBS +=  \
        -L../output/bin  \
        -lFITKAppFramework \
        -lFITKCore \
        -lFITKRenderWindowVTK \
        -lFITKInterfaceMesh \
        -lFITKInterfaceGeometry \
        -lFITKWidget \
        -lFITKGeoCompOCC \
        -lFITKCompMessageWidget \
        -lGUIFrame \
        -lModelData \
        -lOperatorsModel \
        -lOperatorsGUI \
        -lGraphDataPick \
        -lHDF5IO \
        -lFITKCGNSIO \
        -lFITKGmshMshIO \
        #-lFITKAbaqusIOINP \

    Debug:CONFIG            +=  console
    Debug:DESTDIR         = ../output/bin_d
    Debug:MOC_DIR         = ../generate/MeshApp/debug/moc
    Debug:RCC_DIR         = ../generate/MeshApp/debug/rcc
    Debug:UI_DIR          = ../generate/MeshApp/debug/qui
    Debug:OBJECTS_DIR     = ../generate/MeshApp/debug/obj
    Debug:LIBS +=  \
        -L../output/bin_d  \
        -lFITKAppFramework \
        -lFITKCore \
        -lFITKRenderWindowVTK \
        -lFITKInterfaceMesh \
        -lFITKInterfaceGeometry \
        -lFITKWidget \
        -lFITKGeoCompOCC \
        -lFITKCompMessageWidget \
        -lGUIFrame \
        -lModelData \
        -lOperatorsModel \
        -lOperatorsGUI \
        -lGraphDataPick \
        -lHDF5IO \
        -lFITKCGNSIO \
        -lFITKGmshMshIO \
        #-lFITKAbaqusIOINP \

Debug:LIBS +=  -L$$PWD/../Tools/Win64/SARibbon/libd/  \ 
               -lSARibbonBard  \

Release:LIBS +=  -L$$PWD/../Tools/Win64/SARibbon/lib/  \
               -lSARibbonBar  \

    message("Windows MeshApp generated")
}

unix{

    INCLUDEPATH    +=   ./  \
                        ../ \
                        ../../ \
                        ../Tools/Linux64/SARibbon/include/SARibbon-2.0.1/  \

    CONFIG          += console
    CONFIG          += plugin
    DESTDIR         = ../output/bin
    MOC_DIR         = ../generate/MeshApp/release/moc
    UI_DIR          = ../generate/MeshApp/release/qui
    RCC_DIR         = ../generate/MeshApp/release/rcc
    OBJECTS_DIR     = ../generate/MeshApp/release/obj
    LIBS += \
        -L../output/bin \ 
        -lFITKAppFramework \
        -lFITKCore \
        -lFITKRenderWindowVTK \
        -lFITKInterfaceMesh \
        -lFITKInterfaceGeometry \
        -lFITKWidget \
        -lFITKGeoCompOCC \
        -lFITKCompMessageWidget \
        -lGUIFrame \
        -lModelData \
        -lOperatorsModel \
        -lOperatorsGUI \
        -lGraphDataPick \
        -lHDF5IO \
        -lFITKCGNSIO \
        -lFITKGmshMshIO \
        #-lFITKAbaqusIOINP \

        
     LIBS += \
        -L$$PWD/../Tools/Linux64/SARibbon/lib/  \
        -lSARibbonBar \
        
    message("Linux MeshApp generated")
}

