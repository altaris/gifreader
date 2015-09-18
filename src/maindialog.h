#ifndef MAINDIALOG_H
#define MAINDIALOG_H

#include <QCoreApplication>
#include <QDir>
#include <QFile>
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

private slots:

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
     * \brief Qt generated UI builder.
     */
    Ui::MainDialog ui;

    /*!
     * \brief Movie handler.
     *
     * This actually _plays_ the gifs.
     */
    QMovie* movie;

};

#endif // MAINDIALOG_H
