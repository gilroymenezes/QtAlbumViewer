#include <cmath>

#include "photoscaling.h"

#ifndef QT_NO_CONCURRENT

const int imageSize = 100;

QImage scale (const QString &imageFileName)
{
    QImage image(imageFileName);
    return image.scaled(QSize(imageSize, imageSize),
                        Qt::IgnoreAspectRatio,
                        Qt::SmoothTransformation
                        );
}

Photos::Photos(QWidget *parent) :
    QWidget(parent)
{
    setWindowTitle(tr("Image loading and scaling example"));
    resize(800, 600);

    photoScaling = new QFutureWatcher<QImage>(this);
    connect(photoScaling, SIGNAL(resultReadyAt(int)), SLOT(showImage(int)));
    connect(photoScaling, SIGNAL(finished()), SLOT(finished()));

    openButton = new QPushButton(tr("Open Images"));
    connect(openButton, SIGNAL(clicked()), SLOT(open()));

    cancelButton = new QPushButton(tr("Cancel"));
    cancelButton->setEnabled(false);
    connect(cancelButton, SIGNAL(clicked()), photoScaling, SLOT(cancel()));

    pauseButton = new QPushButton(tr("Pause/Resume"));
    pauseButton->setEnabled(false);
    connect(pauseButton, SIGNAL(clicked()), photoScaling, SLOT(togglePaused()));

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(openButton);
    buttonLayout->addWidget(cancelButton);
    buttonLayout->addWidget(pauseButton);
    buttonLayout->addStretch();

    imagesLayout = new QGridLayout();

    mainLayout = new QVBoxLayout();
    mainLayout->addLayout(buttonLayout);
    mainLayout->addLayout(imagesLayout);
    mainLayout->addStretch();
    setLayout(mainLayout);
}

Photos::~Photos()
{
    photoScaling->cancel();
    photoScaling->waitForFinished();
}

void Photos::open()
{
    // Cancel and wait if images are already loading
    if (photoScaling->isRunning())
    {
        photoScaling->cancel();
        photoScaling->waitForFinished();
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

    int dim = sqrt(qreal(files.count())) + 1;
    for (int i = 0; i < dim; ++i)
    {
        for (int j = 0; j < dim; ++j)
        {
            QLabel *imageLabel = new QLabel;
            imageLabel->setFixedSize(imageSize, imageSize);
            imagesLayout->addWidget(imageLabel, i, j);
            labels.append(imageLabel);
        }
    }

    // Use mapped to run the thread safe scale function on the files
    photoScaling->setFuture(QtConcurrent::mapped(files, scale));

    openButton->setEnabled(false);
    cancelButton->setEnabled(true);
    pauseButton->setEnabled(true);
}

void Photos::showImage(int num)
{
    labels[num]->setPixmap(QPixmap::fromImage(photoScaling->resultAt(num)));
}

void Photos::finished()
{
    openButton->setEnabled(true);
    cancelButton->setEnabled(false);
    pauseButton->setEnabled(false);
}

#endif // QT_NO_CONCURRENT
