#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    // Creates the QApplication and shows the UI window
    // Quits on close of the window
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
