#-------------------------------------------------
#
# Project created by QtCreator 2010-07-03T21:33:11
#
#-------------------------------------------------

QT       += core gui

TARGET = monodes
#TEMPLATE = app subdirs

#SUBDIRS = Eigen

SOURCES = main.cpp\
    node.cpp \
    canvas.cpp \
    beam.cpp \
    linearsystem.cpp \
    material.cpp \
    section.cpp \
    nodedialog.cpp \
    beamdialog.cpp \
    materialdialog.cpp \
    trussdialog.cpp \
    licensedialog.cpp \
    sectiondialog.cpp \
    truss.cpp \
    load.cpp \
    reactions.cpp

HEADERS  = \
    node.h \
    canvas.h \
    beam.h \
    linearsystem.h \
    material.h \
    section.h \
    nodedialog.h \
    beamdialog.h \
    materialdialog.h \
    trussdialog.h \
    licensedialog.h \
    sectiondialog.h \
    truss.h \
    load.h \
    reactions.h \
    commonmath.h

## Eigen2 matrix library takes the form of some headers
INCLUDEPATH += Eigen

FORMS = \
    trussdialog.ui \
    nodedialog.ui \
    beamdialog.ui \
    materialdialog.ui \
    licensedialog.ui \
    sectiondialog.ui


#CONFIG += link_pkgconfig
#PKGCONFIG += eigen2

#CONFIG += static

RESOURCES +=

OTHER_FILES += \
    README \
    TODO \
    LICENSE \
    android/src/eu/licentia/necessitas/ministro/IMinistro.aidl \
    android/src/eu/licentia/necessitas/ministro/IMinistroCallback.aidl \
    android/src/eu/licentia/necessitas/industrius/QtActivity.java \
    android/src/eu/licentia/necessitas/industrius/QtSurface.java \
    android/src/eu/licentia/necessitas/industrius/QtApplication.java \
    android/res/drawable-mdpi/icon.png \
    android/res/drawable-hdpi/icon.png \
    android/res/values/libs.xml \
    android/res/values/strings.xml \
    android/res/drawable-ldpi/icon.png \
    android/AndroidManifest.xml

symbian {
vendorinfo = \
    "; Localised Vendor name" \
    "%{\"Tybor, Paolo Redaelli\"}" \
    " " \
    "; Unique Vendor name" \
    ":\"Tybor, Paolo Redaelli\"" \
    " "

 my_deployment.pkg_prerules = vendorinfo

 supported_platforms = \
     "; This program only supports S60 5.0 and later" \
     "[0x1028315F],0,0,0,{\"S60 5th Edition Symbian1\"}" \
     "[0x20022E6D],0,0,0,{\"Symbian3\"}" \
     "[0x200267C2], 4, 6, 3, {\"Qt WebKit\"}" \
     "[0x2001E61C], 4, 6, 3, {\"Qt\"}"

 default_deployment.pkg_prerules -= pkg_platform_dependencies
 my_deployment.pkg_prerules += supported_platforms
 DEPLOYMENT += my_deployment

#    TARGET.UID3 = 0x2003086D
#    # TARGET.CAPABILITY +=
#    TARGET.EPOCSTACKSIZE = 0x14000
#    TARGET.EPOCHEAPSIZE = 0x020000 0x800000
 ICON = icon.svg
}
