#include <QtGui/QApplication>
#include <QtGui>

#ifndef QT_NO_CONCURRENT

#include "albumviewer.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Photos albumViewer;
    albumViewer.show();

    return a.exec();
}

#else

int main(int argc, char *argv[])
{
    qDebug() << "Qt concurrent is not supported on this platform.";
}

#endif
