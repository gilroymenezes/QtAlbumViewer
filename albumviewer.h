#ifndef ALBUMVIEWER_H
#define ALBUMVIEWER_H

#include <QMainWindow>
#include <QLabel>
#include <QAction>
#include <QMenu>
#include <QStringList>
#include <QListWidget>

class AlbumViewer : public QMainWindow
{
    Q_OBJECT

public:
    AlbumViewer();
    QStringList folderValues();

    QListWidget *widget;

signals:
    void folderOpened(QStringList list);

public slots:
    void displayFiles();

private slots:
    void open();
    void about();

private:
    void createActions();
    void createMenus();

    QAction *openAction;
    QAction *aboutAction;

    QMenu *fileMenu;
    QMenu *helpMenu;

    QStringList filenames;
};

#endif // ALBUMVIEWER_H
