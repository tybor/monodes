#include <QtGui/QApplication>
#include "canvas.h"
#include "matrix.h"
#include <stdio.h>
#include <iostream>
#include <assert.h>

int main(int argc, char *argv[])
{
    QApplication application(argc, argv);
    Canvas continous_beam;


#if defined(Q_WS_S60)
    continous_beam.showMaximized();
#else
    continous_beam.show();
#endif

    return application.exec();
}
