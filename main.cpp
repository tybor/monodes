#include <QtGui> //#include <QtGui/QApplication>
#include "canvas.h"
#include <stdio.h>
#include <iostream>
#include <assert.h>
#include "nodedialog.h"
int main(int argc, char *argv[])
{
    QApplication application(argc, argv);
    Canvas beam;
    // Show the initial dialog
    beam.dialog.show();

    return application.exec();
}
