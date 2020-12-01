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
#include <unistd.h>
#include <string.h>
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

using namespace std;


namespace Ui {
  class MainWindow;
}
 
class MainWindow : public QMainWindow
{
  Q_OBJECT
public:
  explicit MainWindow(QWidget *parent = nullptr);
  //void handleListen();
  
private slots:
  //void handleNumberOfPlayer();
  void handleAButton();
  void handleBButton();
  void handleCButton();
  void handleDButton();
  void handleSkipButton();
  void handleListen();
  

private:
  bool outOfSkipTurn = false;
  bool pressed = false;
  char answer = 'S';
  QPushButton *a_button, *b_button, *c_button, *d_button, *listen_button, *skip_answer;
  QTextEdit *console_view, *player_name, *player_number, *question_view, *A_answer, *B_answer, *C_answer, *D_answer, *timer_view;
  QTextEdit *name_title, *time_title, *question_title;
};


#endif // MAINWINDOW_H


