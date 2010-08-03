#-------------------------------------------------
#
# Project created by QtCreator 2010-07-03T21:33:11
#
#-------------------------------------------------

QT       += core gui

TARGET = monodes
TEMPLATE = app


SOURCES += main.cpp\
    node.cpp \
    canvas.cpp \
    beam.cpp \
    linearsystem.cpp \
    matrix.cpp \
    material.cpp \
    section.cpp \
    nodedialog.cpp \
    beamdialog.cpp \
    materialdialog.cpp \
    trussdialog.cpp

HEADERS  += \
    node.h \
    canvas.h \
    beam.h \
    linearsystem.h \
    matrix.h \
    material.h \
    section.h \
    nodedialog.h \
    beamdialog.h \
    materialdialog.h \
    trussdialog.h

FORMS    += \
    trussdialog.ui

CONFIG += mobility
MOBILITY = 

symbian {
    TARGET.UID3 = 0xe0bb5b77
    # TARGET.CAPABILITY += 
    TARGET.EPOCSTACKSIZE = 0x14000
    TARGET.EPOCHEAPSIZE = 0x020000 0x800000
}

RESOURCES += \
    images.qrc

OTHER_FILES += \
    README
