#include "mainwindow.h"

using namespace std;

int main(int argc, char *argv[])
{
    QApplication::setSetuidAllowed(true);
    QApplication app(argc, argv);
    MainWindow mainWindow;
    mainWindow.setWindowTitle("WHO WANTS TO BE A MILLIONAIRE ?");
    //mainWindow.showMaximized();
    mainWindow.show();
    mainWindow.setFixedSize(500,500);
    mainWindow.setStyleSheet("background-color: white;");
    return app.exec();
}   