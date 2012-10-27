#include <QtGui/QApplication>
#include <QtGui>

#ifndef QT_NO_CONCURRENT

#include "albumviewer.h"
#include "photoscaling.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    /**
    AlbumViewer albumViewer;
    //albumViewer.setStyleSheet("* { background-color:rgb(255,); padding: 7px ; color:rgb(255,255,255)}");
    albumViewer.setCentralWidget(albumViewer.widget);
    albumViewer.show();
    **/

    /**
    QObject::connect(&albumViewer,
            SIGNAL(folderOpened(QStringList)),
            &albumViewer,
            SLOT(displayFiles())
            );
    **/

    Photos photoView;
    photoView.show();

    return a.exec();
}

#else

int main(int argc, char *argv[])
{
    qDebug() << "Qt concurrent is not supported on this platform.";
}

#endif
