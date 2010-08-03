#include <QtGui/QApplication>
#include "canvas.h"
#include "trussdialog.h"
#include "matrix.h"
#include <stdio.h>
#include <iostream>
#include <assert.h>

int main(int argc, char *argv[])
{
    QApplication application(argc, argv);
    //Canvas continous_beam;
    TrussDialog dialog;
#if defined(Q_WS_S60)
    dialog.showMaximized();    //continous_beam.showMaximized();
#else
    dialog.show(); //    continous_beam.show();
#endif

    return application.exec();
}
