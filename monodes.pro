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
    trussdialog.cpp \
    licensedialog.cpp \
    sectiondialog.cpp

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
    trussdialog.h \
    licensedialog.h \
    sectiondialog.h

FORMS    += \
    trussdialog.ui \
    nodedialog.ui \
    beamdialog.ui \
    materialdialog.ui \
    licensedialog.ui \
    sectiondialog.ui

CONFIG += mobility
MOBILITY = 

symbian {
    TARGET.UID3 = 0xe0bb5b77
    # TARGET.CAPABILITY += 
    TARGET.EPOCSTACKSIZE = 0x14000
    TARGET.EPOCHEAPSIZE = 0x020000 0x800000
}

RESOURCES += \
    images.qrc \
    texts.qrc

OTHER_FILES += \
    README
