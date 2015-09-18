#include <QApplication>

#include "src/maindialog.h"

int main(int argc, char* argv[]) {

    QApplication app(argc, argv);
    app.setApplicationName("gifreader");
    app.setApplicationVersion("1");

    MainDialog dialog;
    dialog.show();

    return app.exec();

}
