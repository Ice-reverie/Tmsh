TEMPLATE = subdirs

SUBDIRS += \
    GraphDataAdaptor \
    GraphDataPick \
    GraphDataProvider

GraphDataAdaptor.subdir = $$PWD/GraphDataAdaptor
GraphDataPick.subdir = $$PWD/GraphDataPick
GraphDataProvider.subdir = $$PWD/GraphDataProvider

GraphDataPick.depends = GraphDataAdaptor
GraphDataProvider.depends = GraphDataAdaptor
