#include <QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    app.setApplicationName("SleepGuard");
    app.setApplicationVersion("0.1 MVP");

    SleepGuard::MainWindow window;
    window.resize(1100, 720);
    window.show();

    return app.exec();
}
