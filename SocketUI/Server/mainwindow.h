#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QApplication>

#include <QMainWindow>
#include <QPushButton>
#include <QTextEdit>

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
  /*void handleNumberOfPlayer();
  void handleAButton();
  void handleBButton();
  void handleCButton();
  void handleDButton();*/
  void handleListen();
  
private:
  
  QPushButton *a_button, *b_button, *c_button, *d_button, *listen_button;
  QTextEdit *console_view, *player_number, *question, *A_answer, *B_answer, *C_answer, *D_answer, *true_answer;
};
#endif // MAINWINDOW_H
