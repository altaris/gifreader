#ifndef MAINDIALOG_H
#define MAINDIALOG_H

#include <QCoreApplication>
#include <QDir>
#include <QFileInfo>
#include <QFileDialog>
#include <QKeySequence>
#include <QMovie>
#include <QShortcut>
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
     * \param path If not null, the given file is opened.
     *
     * The argument path is processed by open().
     */
    explicit MainDialog(const QString& path = QString(), QWidget* parent = 0);

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
     * \brief Pause or unpause depending on the state.
     */
    void togglePause();

private:

    /*!
     * \brief Current dir.
     */
    QDir currentDir;

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

#endif // MAINDIALOG_H
