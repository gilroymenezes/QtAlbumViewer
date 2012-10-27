#include <QApplication>
#include <QMessageBox>
#include <QDirIterator>
#include <QHBoxLayout>
#include <QMenu>
#include <QtGui>

#include "albumviewer.h"

AlbumViewer::AlbumViewer()
{
    createActions();
    createMenus();

    setWindowTitle(tr("Album Viewer"));
    resize(640, 480);
}

QStringList AlbumViewer::folderValues()
{
    return filenames;
}

/**
Use this function as an example for the SIGNAL/SLOT communication
that is commented out from the main.cpp source.
This is only useful for illustration of event firing mechanism in QT
**/
void AlbumViewer::displayFiles()
{
    widget->setViewMode(QListView::IconMode);
    widget->setIconSize(QSize(100,100));
    widget->setFlow(QListView::LeftToRight);
    widget->setSpacing(4);

    for(QStringList::Iterator it = filenames.begin();
        it != filenames.end();
        ++it)
    {
        QString current = *it;
        qDebug() << current;
        QPixmap icon(current);
        QListWidgetItem *item = new QListWidgetItem(current);
        item->setIcon(icon);
        QFont font;
        font.setPixelSize(1);
        item->setFont(font);
        item->setData(Qt::UserRole, current);
        widget->addItem(item);
    }

    QHBoxLayout *layout = new QHBoxLayout();
    layout->addWidget(widget);
}


void AlbumViewer::open()
{
    widget->setViewMode(QListView::IconMode);
    widget->setIconSize(QSize(100,100));
    widget->setFlow(QListView::LeftToRight);
    widget->setSpacing(4);

    QHBoxLayout *layout = new QHBoxLayout();
    layout->addWidget(widget);

    QString dirPath = QFileDialog::getExistingDirectory(
                this,
                tr("Select pictures folder"),
                QDir::currentPath()
                );
    QDirIterator dirWalker(
                dirPath,
                QDir::Files | QDir::NoSymLinks | QDir::NoDotAndDotDot,
                QDirIterator::NoIteratorFlags
                );

    while(dirWalker.hasNext())
    {
        dirWalker.next();
        QStringList strList = dirWalker.fileName().split(".", QString::KeepEmptyParts);
        if (!strList[strList.length() - 1].compare("jpg", Qt::CaseInsensitive))
        {
            qDebug() << dirWalker.fileName();
            //filenames.append(dirWalker.fileName());
            filenames.append(dirWalker.filePath());
            QPixmap icon(dirWalker.filePath());
            QListWidgetItem *item = new QListWidgetItem(dirWalker.filePath());
            item->setIcon(icon);
            QFont font;
            font.setPixelSize(1);
            item->setFont(font);
            item->setData(Qt::UserRole, dirWalker.fileName());
            widget->addItem(item);
            layout->update();
        }
        qApp->processEvents(QEventLoop::AllEvents);
    }
    //emit folderOpened(folderValues());
}

void AlbumViewer::about()
{
    QMessageBox::about(
                this,
                tr("About Album Viewer"),
                tr("Example <b>Album Viewer</b> application")
                );
}

void AlbumViewer::createActions()
{
    widget = new QListWidget();

    openAction = new QAction(tr("&Open.."), this);
    openAction->setShortcut(tr("Ctrl+O"));
    connect(openAction, SIGNAL(triggered()), this, SLOT(open()));

    aboutAction = new QAction(tr("&About"), this);
    connect(aboutAction, SIGNAL(triggered()), this, SLOT(about()));
}

void AlbumViewer::createMenus()
{
    fileMenu = new QMenu(tr("&File"), this);
    fileMenu->addAction(openAction);

    helpMenu = new QMenu(tr("&About"), this);
    helpMenu->addAction(aboutAction);

    menuBar()->addMenu(fileMenu);
    menuBar()->addMenu(helpMenu);
}

