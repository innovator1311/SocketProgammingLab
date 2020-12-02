/* ui lib*/
#include "mainwindow.h"

using namespace std;

int main(int argc, char *argv[])
{
    // UI code
    QApplication app(argc, argv);
    
    MainWindow mainWindow;
    mainWindow.setWindowTitle("WHO WANTS TO BE A MILLIONAIRE ?");
    mainWindow.show();
    mainWindow.setFixedSize(500,500);
    mainWindow.setStyleSheet("background-color: white; color: black");
    
    return app.exec();
}
