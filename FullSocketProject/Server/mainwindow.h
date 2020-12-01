#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QApplication>

#include <QMainWindow>
#include <QPushButton>
#include <QTextEdit>
#include <QTimer>
#include <QtCore>
#include <QDebug>
#include <QObject>


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <set>
#include <vector>
#include <string>

#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

using namespace std;


namespace Ui {
  class MainWindow;
}
 
class MainWindow : public QMainWindow
{
  Q_OBJECT
public:
  explicit MainWindow(QWidget *parent = nullptr);
  
private slots:
  void handleListen();
  
private:
  bool listenning = false;
  QPushButton *a_button, *b_button, *c_button, *d_button, *listen_button;
  QTextEdit *console_view, *player_number, *question_view, *A_answer, *B_answer, *C_answer, *D_answer, *true_answer;
};


#endif // MAINWINDOW_H


