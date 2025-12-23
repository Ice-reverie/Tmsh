TEMPLATE = subdirs

SUBDIRS += \
    FITKInterfaceModel \
    FITKVTKAlgorithm \
    FITKInterfaceIO \
    FITKInterfaceGeometry \
    FITKInterfaceMeshGen \
    FITKInterfaceMesh \
    FITKInterfacePhysics

FITKInterfaceModel.subdir = $$PWD/FITKInterfaceModel
FITKVTKAlgorithm.subdir = $$PWD/FITKVTKAlgorithm
FITKInterfaceIO.subdir = $$PWD/FITKInterfaceIO
FITKInterfaceMesh.subdir = $$PWD/FITKInterfaceMesh
FITKInterfaceMeshGen.subdir = $$PWD/FITKInterfaceMeshGen
FITKInterfaceGeometry.subdir = $$PWD/FITKInterfaceGeometry
FITKInterfacePhysics.subdir = $$PWD/FITKInterfacePhysics

FITKInterfaceGeometry.depends = FITKInterfaceModel
FITKInterfaceMeshGen.depends = FITKInterfaceModel FITKInterfaceGeometry
FITKInterfaceMesh.depends = FITKInterfaceModel FITKVTKAlgorithm
FITKInterfacePhysics.depends = FITKInterfaceModel

# Optional modules (currently disabled)
# SUBDIRS += FITKInterfaceStructural FITKInterfaceStructuralPost
# FITKInterfaceStructural.subdir = $$PWD/FITKInterfaceStructural
# FITKInterfaceStructuralPost.subdir = $$PWD/FITKInterfaceStructuralPost
