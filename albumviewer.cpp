#include <cmath>

#include "albumviewer.h"

#ifndef QT_NO_CONCURRENT

const int imageSize = 100;

QImage scale (const QString &imageFileName)
{
    QImage image(imageFileName);

    return (image.height() >= image.width()) ?
                image.scaledToHeight(imageSize, Qt::SmoothTransformation) :
                image.scaledToWidth(imageSize, Qt::SmoothTransformation);
}

Photos::Photos(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle(tr("Image loading and scaling example"));
    resize(800, 600);

    albumViewer = new QFutureWatcher<QImage>(this);
    connect(albumViewer, SIGNAL(resultReadyAt(int)), SLOT(showImage(int)));
    connect(albumViewer, SIGNAL(finished()), SLOT(finished()));

    openButton = new QPushButton(tr("Open Images"));
    connect(openButton, SIGNAL(clicked()), SLOT(open()));

    cancelButton = new QPushButton(tr("Cancel"));
    cancelButton->setEnabled(false);
    connect(cancelButton, SIGNAL(clicked()), albumViewer, SLOT(cancel()));

    pauseButton = new QPushButton(tr("Pause/Resume"));
    pauseButton->setEnabled(false);
    connect(pauseButton, SIGNAL(clicked()), albumViewer, SLOT(togglePaused()));

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(openButton);
    buttonLayout->addWidget(cancelButton);
    buttonLayout->addWidget(pauseButton);
    buttonLayout->addStretch();

    scrollArea = new QScrollArea;

    imagesLayout = new QGridLayout();

    mainLayout = new QVBoxLayout(scrollArea);
    mainLayout->addLayout(buttonLayout);
    mainLayout->addLayout(imagesLayout);
    mainLayout->addStretch();

    QWidget *container = new QWidget(imagesLayout->widget());
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    scrollArea->setWidgetResizable(false);
    scrollArea->setWidget(container);
    setCentralWidget(scrollArea);
}


Photos::~Photos()
{
    albumViewer->cancel();
    albumViewer->waitForFinished();
}

void Photos::open()
{
    // Cancel and wait if images are already loading
    if (albumViewer->isRunning())
    {
        albumViewer->cancel();
        albumViewer->waitForFinished();
    }

    // Show a file open dialog at My Pictures
    QStringList files = QFileDialog::getOpenFileNames(
                this,
                tr("Select photos"),
                QDir::currentPath(), /* QDesktopServices::storageLocation(QDesktopServices::PicturesLocation),*/
                "*.jpg *.png"
                );
    if (files.count() == 0)
    {
        qDebug() << "No photos found!";
        return;
    }

    // Do a simple layout
    qDeleteAll(labels);
    labels.clear();

    //int dim = sqrt(qreal(files.count())) + 1;
    for (int i = 0; i < files.count() / 8; ++i)
    {
        for (int j = 0; j < 8; ++j)
        {
            QLabel *imageLabel = new QLabel;
            imageLabel->setFixedSize(imageSize, imageSize);
            imageLabel->setAlignment(Qt::AlignLeft | Qt::AlignTop);
            imagesLayout->addWidget(imageLabel, i, j);
            labels.append(imageLabel);
        }
    }

    // Use mapped to run the thread safe scale function on the files
    albumViewer->setFuture(QtConcurrent::mapped(files, scale));

    openButton->setEnabled(false);
    cancelButton->setEnabled(true);
    pauseButton->setEnabled(true);
}

void Photos::showImage(int num)
{
    labels[num]->setPixmap(QPixmap::fromImage(albumViewer->resultAt(num)));
}

void Photos::finished()
{
    openButton->setEnabled(true);
    cancelButton->setEnabled(false);
    pauseButton->setEnabled(false);
}

#endif // QT_NO_CONCURRENT
