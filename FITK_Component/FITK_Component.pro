TEMPLATE = subdirs

SUBDIRS += \
    FITKGeoCompOCC \
    FITKRenderWindowVTK \
    FITKWidget \
    FITKCompMessageWidget \
    FITKCGNSIO \
    FITKGmshMshIO \
    FITKTmshMshIO \
    FITKGmshExeDriver \
    FITKTmshExeDriver

FITKGeoCompOCC.subdir = $$PWD/FITKGeoCompOCC
FITKRenderWindowVTK.subdir = $$PWD/FITKRenderWindowVTK
FITKWidget.subdir = $$PWD/FITKWidget
FITKCompMessageWidget.subdir = $$PWD/FITKCompMessageWidget
FITKCGNSIO.subdir = $$PWD/FITKCGNSIO
FITKGmshExeDriver.subdir = $$PWD/FITKGmshExeDriver
FITKGmshMshIO.subdir = $$PWD/FITKGmshMshIO
FITKTmshMshIO.subdir = $$PWD/FITKTmshMshIO
FITKTmshExeDriver.subdir = $$PWD/FITKTmshExeDriver

FITKGmshExeDriver.depends = FITKGmshMshIO FITKWidget
FITKTmshExeDriver.depends = FITKTmshMshIO FITKWidget

# Optional modules (currently disabled)
# SUBDIRS += FITKMeshGenFastCAEGrid FITKAbaqusData FITKAbaqusIOINP
# FITKMeshGenFastCAEGrid.subdir = $$PWD/FITKMeshGenFastCAEGrid
# FITKAbaqusData.subdir = $$PWD/FITKAbaqusData
# FITKAbaqusIOINP.subdir = $$PWD/FITKAbaqusIOINP
