TEMPLATE = lib
TARGET = GenericProjectManager
include(../../qworkbenchplugin.pri)
include(genericprojectmanager_dependencies.pri)
HEADERS = genericproject.h \
    genericprojectplugin.h \
    genericprojectmanager.h \
    genericprojectconstants.h \
    genericprojectnodes.h \
    genericprojectwizard.h \
    genericprojectfileseditor.h \
    pkgconfigtool.h \
    genericmakestep.h
SOURCES = genericproject.cpp \
    genericprojectplugin.cpp \
    genericprojectmanager.cpp \
    genericprojectnodes.cpp \
    genericprojectwizard.cpp \
    genericprojectfileseditor.cpp \
    pkgconfigtool.cpp \
    genericmakestep.cpp
RESOURCES += genericproject.qrc
