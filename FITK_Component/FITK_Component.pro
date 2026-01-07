TEMPLATE = subdirs

SUBDIRS += \
    FITKGeoCompOCC \
    FITKRenderWindowVTK \
    FITKWidget \
    FITKCompMessageWidget \
    FITKCGNSIO \
    FITKGmshMshIO \
    FITKGmshExeDriver \
    FITKTmshExeDriver

FITKGeoCompOCC.subdir = $$PWD/FITKGeoCompOCC
FITKRenderWindowVTK.subdir = $$PWD/FITKRenderWindowVTK
FITKWidget.subdir = $$PWD/FITKWidget
FITKCompMessageWidget.subdir = $$PWD/FITKCompMessageWidget
FITKCGNSIO.subdir = $$PWD/FITKCGNSIO
FITKGmshExeDriver.subdir = $$PWD/FITKGmshExeDriver
FITKGmshMshIO.subdir = $$PWD/FITKGmshMshIO
FITKTmshExeDriver.subdir = $$PWD/FITKTmshExeDriver

FITKGmshExeDriver.depends = FITKGmshMshIO FITKWidget
FITKTmshExeDriver.depends = FITKGmshMshIO FITKWidget

# Optional modules (currently disabled)
# SUBDIRS += FITKMeshGenFastCAEGrid FITKAbaqusData FITKAbaqusIOINP
# FITKMeshGenFastCAEGrid.subdir = $$PWD/FITKMeshGenFastCAEGrid
# FITKAbaqusData.subdir = $$PWD/FITKAbaqusData
# FITKAbaqusIOINP.subdir = $$PWD/FITKAbaqusIOINP
