/*
 * gifreader : A simple gif reader
 * Copyright (C) 2015 Altaris
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "src/maindialog.h"

MainDialog::MainDialog(QWidget *parent)
    : QDialog(parent),
      movie(new QMovie(this)) {

    ui.setupUi(this);
    ui.widget_controls->setVisible(false);

    setWindowTitle(QGuiApplication::applicationName() + " v" +
                   QGuiApplication::applicationVersion());

    movie->setCacheMode(QMovie::CacheAll);
    ui.label->setMovie(movie);

    connect(movie, SIGNAL(stateChanged(QMovie::MovieState)),
            this, SLOT(movieStateChanged(QMovie::MovieState)));
    connect(movie, SIGNAL(error(QImageReader::ImageReaderError)),
            this, SLOT(movieError(QImageReader::ImageReaderError)));
    connect(movie, SIGNAL(frameChanged(int)), this, SLOT(frameChanged(int)));

    QShortcut* shortcut_o = new QShortcut(QKeySequence(Qt::Key_O), this);
    connect(shortcut_o, SIGNAL(activated()), this, SLOT(showFileOpenDialog()));

    QShortcut* shortcut_space =
            new QShortcut(QKeySequence(Qt::Key_Space), this);
    connect(shortcut_space, SIGNAL(activated()), this, SLOT(togglePause()));

    QShortcut* shortcut_left =
            new QShortcut(QKeySequence(Qt::Key_Left), this);
    connect(shortcut_left, SIGNAL(activated()), this, SLOT(openPreviousFile()));

    QShortcut* shortcut_right =
            new QShortcut(QKeySequence(Qt::Key_Right), this);
    connect(shortcut_right, SIGNAL(activated()), this, SLOT(openNextFile()));

    QShortcut* shortcut_tab = new QShortcut(QKeySequence(Qt::Key_Tab), this);
    connect(shortcut_tab, SIGNAL(activated()), this, SLOT(toggleControls()));

}

bool MainDialog::jumpToFile(int i) {

    QStringList paths = currentFile.dir().entryList(QStringList() << "*.gif",
                                             QDir::Files | QDir::Readable,
                                             QDir::Name);

    if (paths.isEmpty()) {
        return false;
    }

    int c = paths.indexOf(currentFile.fileName());
    if (c < 0) {
        c = 0;
    }

    int j = c + i;
    if (j < 0) {
        j = paths.size() - 1;
    } else if (j >= paths.size()) {
        j = 0;
    }

    return open(currentFile.dir().absolutePath() +
                QDir::separator() + paths[j]);

}

bool MainDialog::open(const QString& path) {

    if (path.isNull()) {
        return false;
    }

    movie->stop();
    movie->setFileName(path);

    if (movie->isValid()) {

        currentFile = QFileInfo(path);

        setWindowTitle(currentFile.absoluteFilePath());
        ui.horizontalSlider->setMinimum(1);
        ui.horizontalSlider->setMaximum(movie->frameCount());

        ui.label->setToolTip(QString("Path: %1\nSize %2\nFrame count: %3")
                             .arg(currentFile.absoluteFilePath(),
                                  QString::number(currentFile.size()),
                                  QString::number(movie->frameCount())));

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

void MainDialog::openNextFile() {
    jumpToFile(1);
}

void MainDialog::openPreviousFile() {
    jumpToFile(-1);
}

void MainDialog::movieError(QImageReader::ImageReaderError err) {

    switch (err) {

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

void MainDialog::frameChanged(int f) {

    if (!isFullScreen() && !isMaximized()) {
        resize(movie->frameRect().size());
    }

    QString max = QString::number(movie->frameCount());
    QString curr = QString::number(f);
    ui.label_frame->setText("Frame: " + curr.rightJustified(max.length(), '0') +
                            "/" + max);

}

void MainDialog::showFileOpenDialog() {

    QDir dir = currentFile.filePath().isEmpty() ?
                QDir::home() : currentFile.dir();
    QString path =
            QFileDialog::getOpenFileName(this, "Open gif", dir.absolutePath(),
                                         "Gif images (*.gif)", 0,
                                         QFileDialog::ReadOnly);

    if (!path.isNull()) {
        open(path);
    }

}

void MainDialog::toggleControls() {

    ui.widget_controls->setVisible(!ui.widget_controls->isVisible());

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
