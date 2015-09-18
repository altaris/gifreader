#include <QCommandLineParser>
#include <QApplication>

#include "src/maindialog.h"

int main(int argc, char* argv[]) {

    QApplication app(argc, argv);
    app.setApplicationName("gifreader");
    app.setApplicationVersion("1");

    QCommandLineParser parser;
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addPositionalArgument("file", "Gif file to open");

    parser.process(app);

    MainDialog dialog;
    dialog.show();

    if (!parser.positionalArguments().isEmpty()) {
        dialog.open(parser.positionalArguments().first());
    }

    return app.exec();

}
