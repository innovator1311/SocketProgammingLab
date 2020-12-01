#include "mainwindow.h"
#include <QApplication>
 
int main(int argc, char *argv[]) 
{
  QApplication app(argc, argv);
  MainWindow mainWindow;
  mainWindow.showMaximized();
  mainWindow.setFixedSize(500,500);
  mainWindow.setStyleSheet("background-color: white;");
  return app.exec();
}
