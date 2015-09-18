#ifndef MAINDIALOG_H
#define MAINDIALOG_H

#include "ui_maindialog.h"

class MainDialog
    : public QDialog {

    Q_OBJECT

public:

    explicit MainDialog(QWidget* parent = 0);

private:


    Ui::MainDialog ui;
};

#endif // MAINDIALOG_H
