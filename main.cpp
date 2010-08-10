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

#if defined(Q_WS_S60)
    beam.showMaximized();
#else
    beam.show();
#endif

    return application.exec();
}
