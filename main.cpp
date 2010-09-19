#include <QtGui> //#include <QtGui/QApplication>
#include "canvas.h"
#include <stdio.h>
#include <iostream>
#include <assert.h>
#include "nodedialog.h"

int main(int argc, char *argv[])
{
//    TODO: makes the application "matrix-centrix", i.e. still using the
//    old-fashioned matrix approach, masking all the issues under Node and Beam
//    objects, making them "proxy" object.

    QApplication application(argc, argv);
    Canvas beam;
    // Show the initial dialog
    beam.dialog.show();

    return application.exec();
}
