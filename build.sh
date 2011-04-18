#!/bin/bash
export PATH=/home/paolo/gnupoc/bin:/home/paolo/gnupoc/qt-4.6.3-try2/bin:$PATH
export EPOCROOT=/home/paolo/gnupoc/sdk_5.0/ # trailing "/" is required!
unset  QMAKESPEC    # make sure there is no QMAKESPEC variable set

#I wasn't able to make "qmake gnupoc" produce correct Makefiles for Symbian.
qmake -spec symbian-abld
#For now just use qmake from QtCreator
##make distclean 

make release-gcce 
make sis QT_SIS_CERTIFICATE=OVI_06-28-2010.crt QT_SIS_KEY=OVI_06-28-2010.key
make installer_sis QT_SIS_CERTIFICATE=OVI_06-28-2010.crt QT_SIS_KEY=OVI_06-28-2010.key

