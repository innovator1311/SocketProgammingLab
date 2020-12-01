#include "mainwindow.h"

void error(const char *msg)
{
    perror(msg);
    exit(0);
}

bool isResponse(const char *responseServer){
    if(strcmp(responseServer, "correct")==0){
        return true;
    }
    else if(strcmp(responseServer, "skip")==0){
        return true;
    }
    else if(strcmp(responseServer, "winner")==0){
        return true;
    }
    else if(strcmp(responseServer, "incorrect")==0){
        return true;
    }
    return false;
}

string convertMes(const char *messageSer){
    if(strcmp(messageSer, "correct")==0){
        return "correct";
    }
    else if(strcmp(messageSer, "skip")==0){
        return "skip";
    }
    else if(strcmp(messageSer, "winner")==0){
        return "winner";
    }
    else if(strcmp(messageSer, "incorrect")==0){
        return "incorrect";
    }
    else if(strcmp(messageSer, "your turn")==0){
        return "your turn";
    }
    else if(strcmp(messageSer, "start game")==0){
        return "start game";
    }
    else if(strcmp(messageSer, "end game")==0){
        return "end game";
    }
    return "question";
}
// bool isNotification(const char *turnString){
//     if(strcmp(turnString, "your turn") ==0)
//         return true;
//     return false;
// }
// bool isQuestion(const char *responseServer){
//     if(strcmp())
// }



MainWindow::MainWindow(QWidget *parent)
  : QMainWindow(parent)
{

  console_view = new QTextEdit("",this);
  console_view->setGeometry(QRect(QPoint(20, 20), QSize(450, 100)));
  console_view->setDisabled(true);

  name_title = new QTextEdit("Enter your name: ",this);
  name_title->setGeometry(QRect(QPoint(20, 130), QSize(180, 20)));
  name_title->setDisabled(true);
  name_title->setFrameStyle(QFrame::NoFrame);

  player_name = new QTextEdit("",this);
  player_name->setGeometry(QRect(QPoint(150, 130), QSize(70, 20)));

  time_title = new QTextEdit("Remaining time",this);
  time_title->setGeometry(QRect(QPoint(200, 280), QSize(200, 30)));
  time_title->setDisabled(true);
  time_title->setFrameStyle(QFrame::NoFrame);


  timer_view = new QTextEdit("",this);
  timer_view->setGeometry(QRect(QPoint(200, 315), QSize(100, 30)));
  timer_view->setDisabled(true);

  skip_answer = new QPushButton("Skip",this);
  skip_answer->setGeometry(QRect(QPoint(200, 400), QSize(100, 30)));

  question_title = new QTextEdit("Question:",this); 
  question_title->setGeometry(QRect(QPoint(20, 150), QSize(180, 20)));
  question_title->setDisabled(true);
  question_title->setFrameStyle(QFrame::NoFrame);

  question_view = new QTextEdit("",this);
  question_view->setGeometry(QRect(QPoint(20, 180), QSize(450, 50)));
  question_view->setDisabled(true);

  A_answer = new QTextEdit("",this);
  A_answer->setGeometry(QRect(QPoint(20, 260), QSize(120, 40)));
  A_answer->setDisabled(true);

  a_button = new QPushButton("A", this);
  a_button->setGeometry(QRect(QPoint(20, 300), QSize(40, 40)));

  B_answer = new QTextEdit("",this);
  B_answer->setGeometry(QRect(QPoint(350, 260), QSize(120, 40)));
  B_answer->setDisabled(true);

  b_button = new QPushButton("B", this);
  b_button->setGeometry(QRect(QPoint(350, 300), QSize(40, 40)));

  C_answer = new QTextEdit("",this);
  C_answer->setGeometry(QRect(QPoint(20, 360), QSize(120, 40)));
  C_answer->setDisabled(true);

  c_button = new QPushButton("C", this);
  c_button->setGeometry(QRect(QPoint(20, 400), QSize(40, 40)));

  D_answer = new QTextEdit("",this);
  D_answer->setGeometry(QRect(QPoint(350, 360), QSize(120, 40)));
  D_answer->setDisabled(true);

  d_button = new QPushButton("D", this);
  d_button->setGeometry(QRect(QPoint(350, 400), QSize(40, 40)));

  listen_button = new QPushButton("Connect", this);
  listen_button->setGeometry(QRect(QPoint(350, 450), QSize(80, 20)));

  connect(listen_button, &QPushButton::released, this, &MainWindow::handleListen);
  connect(a_button, &QPushButton::released, this, &MainWindow::handleAButton);
  connect(b_button, &QPushButton::released, this, &MainWindow::handleBButton);
  connect(c_button, &QPushButton::released, this, &MainWindow::handleCButton);
  connect(d_button, &QPushButton::released, this, &MainWindow::handleDButton);
  connect(skip_answer, &QPushButton::released, this, &MainWindow::handleSkipButton);
}
 
void MainWindow::handleListen() {

    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;

    char buffer[256];
    
    portno = 15;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
        error("ERROR opening socket");
    server = gethostbyname("127.0.0.1");
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, 
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    serv_addr.sin_port = htons(portno);

    if (::connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) 
        return;
        //error("ERROR connecting");
    
    bzero(buffer,256);

    QString text = player_name->toPlainText();
    string text1 = text.toStdString();
    strcpy(buffer, text1.c_str());

    qApp->processEvents();  
    qApp->processEvents();  

    n = write(sockfd, buffer, strlen(buffer));
    if (n < 0) 
        error("ERROR writing from socket");

    qApp->processEvents();  
    qApp->processEvents();  

    bzero(buffer,256);

    n = read(sockfd, buffer, 256);
    if (n < 0) {
        error("ERROR reading from socket");
    }

    char * tmp = (char*) malloc(strlen(buffer)+1);
    strcpy(tmp, buffer);


    if (strcmp(tmp, "fail")==0){

        qApp->processEvents();  
        qApp->processEvents();

        // received zero bytes means to be failed 
        console_view->setText("Your nickname is not valid or has exist, re-enter your nickname pls\n");
        console_view->setText("Enter your nickname again: ");

        ::close(sockfd);
        return;
    }

    qApp->processEvents();  
    qApp->processEvents();  

    cout<<"Welcome player "<<buffer<<" to join the game"<<endl;
    cout<<"Please wait for other players"<<endl;
    char message[256];
    char answers[4][256];
    string realMessage;

    qApp->processEvents();  
    qApp->processEvents();  

    //-----------------------
    // waiting for confirmation of server to start game
    /*while(true){

        qApp->processEvents();  
        qApp->processEvents();  

        n = read(sockfd, message, 256);
        realMessage = convertMes(message);
        if(realMessage == "start game")
            break;

        qApp->processEvents();  
        qApp->processEvents();  
    }*/
    //-----------------------

    //After receive, send verification back
    //write(sockfd, "ready", 1);

    // enter the game, init game properties from player
    //char answer; // answer from player
    bool isValidAnswer=true; // condition for a valid answer from player's input
    char * responseServer; // response from server upon player's answer
    //bool outOfSkipTurn = false;
    int isWinning=1;


    /// To ensure the client receive the correct package
    /// it must check the incoming package before doing next action
    /// Below are variables which is used to do that
    bool isResponse; // whether the received package is the response of client's answer
    bool isQuestion; // whether the received package is the question sent from server
    bool isConfirmation; // whether the .... ....... is used for other action

    cout<<"Game started"<<endl; 
    while (true) {

        // waiting for the question
        qApp->processEvents();  
        qApp->processEvents();

        console_view->setText("Waiting for your turn!");

        qApp->processEvents();  
        qApp->processEvents();

        // ---------------------
        while(true){
            bzero(message, 256);
            n = read(sockfd, message, 256);
            cout<<"Message receive"<<message<<endl;
            realMessage = convertMes(message);
            if(realMessage == "question" || realMessage == "end game") {
                
                if (realMessage == "question") {
                    for (int i = 0; i < 4; i++) {
                        bzero(answers[i], 256);
                        write(sockfd, "receive", 1);
                        read(sockfd, answers[i], 256);
                    }
                }
        
                break;
            }
        }
        // ---------------------

        if(realMessage == "end game"){
            isWinning = 0;
            break;
        }

        qApp->processEvents();  
        qApp->processEvents();

        console_view->setText("Your turn now!!Press A,B,C,D for answering or Skip for skipping");
        question_view->setText(QString::fromStdString(string(message)));
        A_answer->setText(QString::fromStdString(string(answers[0])));
        B_answer->setText(QString::fromStdString(string(answers[1])));
        C_answer->setText(QString::fromStdString(string(answers[2])));
        D_answer->setText(QString::fromStdString(string(answers[3])));

        qApp->processEvents();  
        qApp->processEvents();

        // how to wait for client answer ??

        QElapsedTimer elapsedTime;
        elapsedTime.start();

        pressed = false;
        answer = 'S';

        while (!pressed && elapsedTime.elapsed() < 20000) // millisecs
        {
            timer_view->setText(QString::number((20000 - elapsedTime.elapsed()) / 1000));

            qApp->processEvents();  
            qApp->processEvents();
        }

        pressed = false;



        /*while(true){
            // enter answer
            cout<<"Answer:";
            cin>>answer;
            if(answer!='A' && answer!='B' && answer!='C' && answer!='D' && answer!='S'){
                isValidAnswer = false;
            }
            else{
                isValidAnswer = true;
            }
            
            if(isValidAnswer == true){
                if(outOfSkipTurn && answer == 'S'){
                    cout<<"You have no skip option, please answer"<<endl;
                    continue;
                }
                else break;
            }
            else{
                cout<<"Invalid answer, please press capital A,B,C,D for answering or S for skipping"<<endl;
            }
        }
        if (n < 0) 
            error("ERROR reading from socket");*/

        // send answer
        bzero(buffer, 256);
        buffer[0] = answer;
        n = write(sockfd, buffer, 1);

        if (n < 0) 
            error("ERROR writing to socket");

        // Waiting for server to response upon player's answer
        cout<<"Server is checking your answer"<<endl;
        bzero(message, 256);

        // ------------------------
        while(true){
            n = read(sockfd, message, 256);
            realMessage = convertMes(message);
            if(realMessage == "correct" || realMessage == "skip" || realMessage == "winner" || realMessage == "incorrect")
                break;

            
        }
        // ------------------------

        // send confirmation back to server
        // write(sockfd, "receive response", 1);   

        // check condition in buffer
        if(realMessage == "correct"){
            // moving to next answer
            cout<<"Correct"<<endl;
            continue;
        }
        else if(realMessage == "skip" && outOfSkipTurn){
            // player skip the answer
            isWinning = -1;
            break;
        }
        else if(realMessage == "skip"){
            // player skip the answer
            outOfSkipTurn = true;
            continue;
        }
        else if(realMessage == "winner"){
            isWinning = 1;
            break;
        }
        else if(realMessage == "incorrect"){
            cout<<"Incorrect"<<endl;
            isWinning = -1;
            break;
        }
    }
    if(isWinning==1){
        console_view->setText("Congratulations!!! You are the winner");
    }
    else if(isWinning==-1){
        console_view->setText("You are disqualified :( Good luck for next game");
    }
    else if(isWinning==0){
        console_view->setText("Some one has successfully answered all questions. Good luck for next game!");
    }
    ::close(sockfd);

}

void MainWindow::handleAButton() {
    pressed = true;
    answer = 'A';
}

void MainWindow::handleBButton() {
    pressed = true;
    answer = 'B';
    
}

void MainWindow::handleCButton() {
    pressed = true;
    answer = 'C';
}

void MainWindow::handleDButton() {
    pressed = true;
    answer = 'D';
}

void MainWindow::handleSkipButton() {

    if (!outOfSkipTurn) {
        pressed = true;
        answer = 'S';
    }
    else {
        console_view->setText("You have no skip option, please answer");
    }
}