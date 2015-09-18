#include "src/maindialog.h"

MainDialog::MainDialog(const QString& path, QWidget *parent)
    : QDialog(parent),
      currendDir(QDir::home()),
      movie(new QMovie(this)) {

    ui.setupUi(this);

    ui.label->setMovie(movie);

    connect(movie, SIGNAL(stateChanged(QMovie::MovieState)),
            this, SLOT(movieStateChanged(QMovie::MovieState)));
    connect(movie, SIGNAL(error(QImageReader::ImageReaderError)),
            this, SLOT(movieError(QImageReader::ImageReaderError)));

    QShortcut* shortcut_o = new QShortcut(QKeySequence(Qt::Key_O), this);
    connect(shortcut_o, SIGNAL(activated()), this, SLOT(showFileOpenDialog()));

    QShortcut* shortcut_space =
            new QShortcut(QKeySequence(Qt::Key_Space), this);
    connect(shortcut_space, SIGNAL(activated()), this, SLOT(togglePause()));

    if (!path.isNull()) {
        open(path);
    }

}

bool MainDialog::open(const QString& path) {

    if (path.isNull()) {
        return false;
    }

    movie->stop();
    movie->setFileName(path);

    if (movie->isValid()) {

        setWindowTitle(path);
        ui.horizontalSlider->setMinimum(1);
        ui.horizontalSlider->setMaximum(movie->frameCount());

        currendDir = QFileInfo(path).dir();

        movie->start();

        qDebug() << "Loaded" << path;

        return true;

    } else {

        qWarning() << "Error loading" << path;

        setWindowTitle("ERROR");
        ui.horizontalSlider->setMinimum(0);
        ui.horizontalSlider->setMaximum(0);

        return false;

    }

}

void MainDialog::movieError(QImageReader::ImageReaderError err) {

    switch(err) {

    case QImageReader::FileNotFoundError:
        qWarning() << "Error: QImageReader::FileNotFoundError";
        break;

    case QImageReader::DeviceError:
        qWarning() << "Error: QImageReader::DeviceError";
        break;

    case QImageReader::UnsupportedFormatError:
        qWarning() << "Error: QImageReader::UnsupportedFormatError";
        break;

    case QImageReader::InvalidDataError:
        qWarning() << "Error: QImageReader::InvalidDataError";
        break;

    default:
    case QImageReader::UnknownError:
        qWarning() << "Error: QImageReader::UnknownError";
        break;

    }

}

void MainDialog::movieStateChanged(QMovie::MovieState state) {

    switch (state) {

    default:
    case QMovie::NotRunning:

        qDebug() << "State changed to 'NotRunning'";

        ui.horizontalSlider->setEnabled(false);

        break;

    case QMovie::Paused:

        qDebug() << "State changed to 'Paused'";

        disconnect(movie, SIGNAL(frameChanged(int)),
                   ui.horizontalSlider, SLOT(setValue(int)));
        connect(ui.horizontalSlider, SIGNAL(valueChanged(int)),
                this, SLOT(setFrame(int)));

        ui.horizontalSlider->setEnabled(true);

        break;

    case QMovie::Running:

        qDebug() << "State changed to 'Running'";

        disconnect(ui.horizontalSlider, SIGNAL(valueChanged(int)),
                   this, SLOT(setFrame(int)));
        connect(movie, SIGNAL(frameChanged(int)),
                ui.horizontalSlider, SLOT(setValue(int)));

        ui.horizontalSlider->setEnabled(false);

        break;

    }

}

void MainDialog::setFrame(int f) {

    movie->jumpToFrame(f);

}

void MainDialog::showFileOpenDialog() {

    QString path = QFileDialog::getOpenFileName(this,
                                                "Open gif",
                                                currendDir.path(),
                                                "Gif images (*.gif)");

    if (!path.isNull()) {
        open(path);
    }

}

void MainDialog::togglePause() {

    switch (movie->state()) {

    default:
    case QMovie::NotRunning:
        break;

    case QMovie::Paused:
        movie->setPaused(false);
        break;

    case QMovie::Running:
        movie->setPaused(true);
        break;

    }

}
