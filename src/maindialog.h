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

#ifndef SRC_MAINDIALOG_H_
#define SRC_MAINDIALOG_H_

#include <QDir>
#include <QFileInfo>
#include <QFileDialog>
#include <QGuiApplication>
#include <QKeySequence>
#include <QMovie>
#include <QPixmap>
#include <QScreen>
#include <QShortcut>
#include <QSize>
#include <QString>
#include <QStringList>
#include <QtDebug>

#include "ui_maindialog.h"

/*!
 * \brief The main (and only) dialog class.
 */
class MainDialog
    : public QDialog {

    Q_OBJECT

public:

    /*!
     * \brief Constructor.
     */
    explicit MainDialog(QWidget* parent = 0);

    /*!
     * \brief Loads a gif file.
     *
     * Checks that the path is valid and that the file extension is gif.
     */
    bool open(const QString& path);

private:

    /*!
     * \brief Opens the file that is at position i relative to the current file.
     *
     * More explicitely, files in the current file's dir are ordered
     * alphabetically, and file at relative index i is opened. For instance,
     * file +1 is the next file, whereas file -1 is the previous file.
     */
    bool jumpToFile(int i);

private slots:

    /*!
     * \brief Changes the frame label infos.
     */
    void frameChanged(int f);

    /*!
     * \brief Called on movie errors.
     */
    void movieError(QImageReader::ImageReaderError err);

    /*!
     * \brief Handles movie state changes.
     */
    void movieStateChanged(QMovie::MovieState state);

    /*!
     * \brief Opens the next file.
     *
     * ... in the current file's dir, alphabetically speaking.
     */
    void openNextFile();

    /*!
     * \brief Opens the previous file.
     *
     * ... in the current file's dir, alphabetically speaking.
     */
    void openPreviousFile();

    /*!
     * \brief Changes the frame.
     */
    void setFrame(int f);

    /*!
     * \brief Shows an open file dialog.
     *
     * The chosen file path is processed by open().
     */
    void showFileOpenDialog();

    /*!
     * \brief Show / hide the controls.
     */
    void toggleControls();

    /*!
     * \brief Pause or play depending on the state.
     */
    void togglePause();

private:

    /*!
     * \brief Current file infos.
     */
    QFileInfo currentFile;

    /*!
     * \brief Movie handler.
     *
     * This actually _plays_ the gifs.
     */
    QMovie* movie;

    /*!
     * \brief Qt generated UI builder.
     */
    Ui::MainDialog ui;

};

#endif // SRC_MAINDIALOG_H_
