TEMPLATE = lib
TARGET = DistutilsProjectManager

include(../../qtcreatorplugin.pri)
include(distutilsprojectmanager_dependencies.pri)

INCLUDEPATH += $$PWD/../../libs/utils

HEADERS += distutilsprojectplugin.h \
          distutilsopenprojectwizard.h \
          distutilsmanager.h \
          distutilsprojectfile.h \
          distutilsprojectnode.h \
          distutilsproject.h \
          distutilstarget.h \
          distutilsbuildsettingswidget.h \
          distutilsbuildconfiguration.h \
          distutilsprojectconstants.h \
          buildstep.h \
          setupparserthread.h \
          setupparser.h

SOURCES += distutilsprojectplugin.cpp \
          distutilsopenprojectwizard.cpp \
          distutilsmanager.cpp \
          distutilsprojectfile.cpp \
          distutilsprojectnode.cpp \
          distutilsproject.cpp \
          distutilstarget.cpp \
          distutilsbuildsettingswidget.cpp \
          distutilsbuildconfiguration.cpp \
          buildstep.cpp \
          setupparserthread.cpp \
          setupparser.cpp

RESOURCES += distutilsproject.qrc

OTHER_FILES += DistutilsProjectManager.pluginspec.in \
          DistutilsProject.mimetypes.xml \
          distutils_extract.py
