#ifndef ALBUMVIEWER_H
#define ALBUMVIEWER_H

#include <QWidget>
#include <QtGui>

#ifndef QT_NO_CONCURRENT

class Photos : public QWidget
{
    Q_OBJECT

public:
    explicit Photos(QWidget *parent = 0);
    ~Photos();

public Q_SLOTS:
    void open();
    void showImage(int num);
    void finished();

private:
    QPushButton *openButton;
    QPushButton *cancelButton;
    QPushButton *pauseButton;
    QVBoxLayout *mainLayout;
    QList<QLabel *> labels;
    QGridLayout *imagesLayout;
    QFutureWatcher<QImage> *albumViewer;
    
};

#else

// Dummy class for QT_NO_CONCURRENT not set when moc is run.
class Photos : public QWidget
{
    Q_OBJECT

public Q_SLOTS:
    void open() {}
    void showImage(int) {}
    void finished() {}
};

#endif // QT_NO_CONCURRENT

#endif // ALBUMVIEWER_H
