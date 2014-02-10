#-------------------------------------------------
#
# Project created by QtCreator 2013-03-29T09:57:39
#
#-------------------------------------------------

CONFIG += plugin

QT       -= gui

TARGET = rasteroperations
TEMPLATE = lib

include(global.pri)

DESTDIR = $$PWD/../libraries/$$PLATFORM$$CONF/$$TARGET
DLLDESTDIR = $$PWD/../output/$$PLATFORM$$CONF/bin/extensions/$$TARGET

DEFINES += RASTEROPERATIONS_LIBRARY

SOURCES += \ 
    rasteroperations/rasteroperationsmodule.cpp \
    rasteroperations/aggregateraster.cpp \
    rasteroperations/areanumbering.cpp \
    rasteroperations/crossrasters.cpp \
    rasteroperations/rasterstretchoperation.cpp \
    rasteroperations/linearrasterfilter.cpp


HEADERS += \
    rasteroperations/rasteroperationsmodule.h \
    rasteroperations/aggregateraster.h \
    rasteroperations/areanumbering.h \
    rasteroperations/crossrasters.h \
    rasteroperations/rasterstrechoperation.h \
    rasteroperations/linearrasterfilter.h


OTHER_FILES += \ 
    rasteroperations/rasteroperations.json

win32:CONFIG(release, debug|release): {
    LIBS += -L$$PWD/../libraries/$$PLATFORM$$CONF/core/ -lilwiscore
    LIBS += -L$$PWD/../libraries/win32release/ -llibgeos
    QMAKE_CXXFLAGS_RELEASE += -O2
}
else:win32:CONFIG(debug, debug|release): {
    LIBS += -L$$PWD/../libraries/$$PLATFORM$$CONF/core/ -lilwiscore
    LIBS += -L$$PWD/../libraries/win32debug/ -llibgeos
}

INCLUDEPATH += $$PWD/../external/geos
DEPENDPATH += $$PWD/../external/geos

INCLUDEPATH += $$PWD/core
DEPENDPATH += $$PWD/core
