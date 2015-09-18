#include "src/maindialog.h"

MainDialog::MainDialog(const QString& path, QWidget *parent)
    : QDialog(parent),
      movie(new QMovie(this)) {

    ui.setupUi(this);

    ui.label->setMovie(movie);

    connect(movie, SIGNAL(frameChanged(int)),
            ui.horizontalSlider, SLOT(setValue(int)));

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

        qDebug() << "Loaded" << path;

        setWindowTitle(path);
        ui.horizontalSlider->setMinimum(1);
        ui.horizontalSlider->setMaximum(movie->frameCount());

        movie->start();

        return true;

    } else {

        qWarning() << "Error loading" << path;

        setWindowTitle("ERROR");
        ui.horizontalSlider->setMinimum(0);
        ui.horizontalSlider->setMaximum(0);

        return false;

    }

}

void MainDialog::showFileOpenDialog() {

    QString path = QFileDialog::getOpenFileName(this,
                                                "Open gif",
                                                QDir::homePath(),
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
