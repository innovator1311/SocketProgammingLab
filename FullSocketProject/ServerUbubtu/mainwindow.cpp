#include "mainwindow.h"

struct Question{
    string question;
    string answerSentence[4];
    char answer;
};


vector<Question> getListQuestion(){
    vector<Question> myQues;
    return myQues;
}

void error(const char *msg)
{
    perror(msg);
    exit(1);
}


bool checkExistPlayer(set<char *> list_player, char *check_name){
    set< char * >::iterator it; 
    bool exist = false;
    for (it=list_player.begin(); it!=list_player.end(); ++it)
    {
        if(strcmp(*it, check_name)==0) { exist = true; break;}
    }
    return exist;
}

int randomQuestionNo(int min, int max) //range : [min, max]
{
    static bool first = true;
    if (first) 
    {  
        srand( time(NULL) ); //seeding for the first time only!
        first = false;
    }
    return min + rand() % (( max + 1 ) - min);
}
// for string delimiter
vector<string> split (string s, string delimiter) {
    size_t pos_start = 0, pos_end, delim_len = delimiter.length();
    string token;
    vector<string> res;

    while ((pos_end = s.find (delimiter, pos_start)) != string::npos) {
        token = s.substr (pos_start, pos_end - pos_start);
        pos_start = pos_end + delim_len;
        res.push_back (token);
    }

    res.push_back (s.substr (pos_start));
    return res;
}

vector<Question> getQuestion (int player_number){

    std::ifstream inFile;
    inFile.open("data.txt");
    std::stringstream strStream;
    strStream << inFile.rdbuf();
    string str = strStream.str();
    inFile.close();

    string delimiter = "===";
    vector<string> v = split (str, delimiter);

    cout<< v.size()<<endl;

    vector<int> quesIdx;
    vector<Question> SetQues;
    for (int i = 1; i <= player_number * 3; i++){
        int quesNo = randomQuestionNo(0,v.size());

        while (std::count(quesIdx.begin(), quesIdx.end(), quesNo))
        {
            quesNo = randomQuestionNo(0,v.size());
        }

        quesIdx.push_back(quesNo);

        vector<string> quesSet = split (v[quesNo], "\n");
        vector<string> subvector = quesSet; // = {quesSet.begin() + 1, quesSet.end() - 1};

        Question Q = {
            subvector[1],
            {subvector[2],subvector[3],subvector[4],subvector[5]},
            subvector[6][0]
        };

        SetQues.push_back(Q);
    }
    return SetQues;
}

MainWindow::MainWindow(QWidget *parent)
  : QMainWindow(parent)
{
 
  console_view = new QTextEdit("Welcome to the game !!\nPress listen to start hosting the game.",this);
  console_view->setGeometry(QRect(QPoint(20, 20), QSize(450, 100)));
  console_view->setDisabled(true);

  player_number_view = new QLineEdit("Enter number of players: ",this);
  player_number_view->setGeometry(QRect(QPoint(20, 130), QSize(180, 20)));
  player_number_view->setDisabled(true);
  player_number_view->setStyleSheet("QLineEdit { border: none}");

  player_number = new QLineEdit("",this);
  player_number->setGeometry(QRect(QPoint(180, 130), QSize(50, 20)));

  question_title = new QLineEdit("Question:",this); 
  question_title->setGeometry(QRect(QPoint(20, 150), QSize(180, 20)));
  question_title->setDisabled(true);
  question_title->setStyleSheet("QLineEdit { border: none}");
  

  question_view = new QTextEdit("",this);
  question_view->setGeometry(QRect(QPoint(20, 180), QSize(450, 50)));
  question_view->setDisabled(true);

  right_answer_title = new QLineEdit("Right answer",this);
  right_answer_title->setGeometry(QRect(QPoint(200, 285), QSize(180, 20)));
  right_answer_title->setDisabled(true);
  right_answer_title->setStyleSheet("QLineEdit { border: none}");

  true_answer = new QTextEdit("",this);
  true_answer->setGeometry(QRect(QPoint(200, 305), QSize(100, 30)));
  true_answer->setDisabled(true);

  a_title = new QLineEdit("A",this);
  a_title->setGeometry(QRect(QPoint(20, 250), QSize(180, 20)));
  a_title->setDisabled(true);
  a_title->setStyleSheet("QLineEdit { border: none}");

  A_answer = new QTextEdit("",this);
  A_answer->setGeometry(QRect(QPoint(20, 270), QSize(120, 40)));
  A_answer->setDisabled(true);

  b_title = new QLineEdit("B",this);
  b_title->setGeometry(QRect(QPoint(350, 250), QSize(180, 20)));
  b_title->setDisabled(true);
  b_title->setStyleSheet("QLineEdit { border: none}");

  B_answer = new QTextEdit("",this);
  B_answer->setGeometry(QRect(QPoint(350, 270), QSize(120, 40)));
  B_answer->setDisabled(true);

  c_title = new QLineEdit("C",this);
  c_title->setGeometry(QRect(QPoint(20, 320), QSize(180, 20)));
  c_title->setDisabled(true);
  c_title->setStyleSheet("QLineEdit { border: none}");

  C_answer = new QTextEdit("",this);
  C_answer->setGeometry(QRect(QPoint(20, 340), QSize(120, 40)));
  C_answer->setDisabled(true);

  d_title = new QLineEdit("D",this);
  d_title->setGeometry(QRect(QPoint(350, 320), QSize(180, 20)));
  d_title->setDisabled(true);
  d_title->setStyleSheet("QLineEdit { border: none}");

  D_answer = new QTextEdit("",this);
  D_answer->setGeometry(QRect(QPoint(350, 340), QSize(120, 40)));
  D_answer->setDisabled(true);

  listen_button = new QPushButton("Listen", this);
  listen_button->setGeometry(QRect(QPoint(350, 400), QSize(80, 20)));

  connect(listen_button, &QPushButton::released, this, &MainWindow::handleListen);
}

void MainWindow::handleListen() {

    if (listenning) return;
    listenning = true;
    
    QString text = player_number->text();
    int client_number = text.toInt();

    if (client_number <= 0) {
        console_view->setText("Please enter number of players bigger than 0");
        listenning = false;
        return;
    }

    console_view->setText("Server is listenning !!!");

    int opt = 1;
    int sockfd, newsockfd, portno, newsockfd2, master_socket, 
        max_sd, sd, activity, new_socket, addrlen, valread;
    
    socklen_t clilen;
    char buffer[256];
    struct sockaddr_in serv_addr, cli_addr;
    int n;
    
    fd_set readfds;

    //create master socket
    if( (master_socket = socket(AF_INET , SOCK_STREAM , 0)) == 0)   
    {   
        perror("socket failed");   
        exit(EXIT_FAILURE);   
    }

    // set sock option
    if( setsockopt(master_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&opt,  
          sizeof(opt)) < 0 )   
        {   
            
            perror("setsockopt");   
            exit(EXIT_FAILURE);   
        }   
    
    // clear address structure
    bzero((char *) &serv_addr, sizeof(serv_addr));

    portno = 15;

    /* setup the host_addr structure for use in bind call */
    // server byte order
    serv_addr.sin_family = AF_INET;  

    // automatically be filled with current host's IP address
    serv_addr.sin_addr.s_addr = INADDR_ANY;  

    // convert short integer value for port must be converted into network byte order
    serv_addr.sin_port = htons(portno);

    if (::bind(master_socket, (struct sockaddr *) &serv_addr,
              sizeof(serv_addr)) < 0)
              error("ERROR on binding");

    // set number of pending connection
    if (listen(master_socket, 3) < 0)   
    {   
        perror("listen");   
        exit(EXIT_FAILURE);   
    } 

    //Nhap so luong client
    int max_num_client=client_number;
    int cur_num_client=0;
    
    set<char *> list_player; // init list of players nickname
    int * clien_socket = new int[max_num_client]; // init list of players socket
    string * client_name = new string[max_num_client];
    // init all socket to 0
    for(int i=0; i<max_num_client; i++){
        clien_socket[i] = 0;
    }
    addrlen = sizeof(serv_addr); 
    char *message = "ECHO Daemon v1.0 \r\n";
    bool exist;
    // Waiting for players to register
    int num_client_to_start = 0;

    qApp->processEvents();  
    qApp->processEvents();  

    while(true)   
    {   
        qApp->processEvents();  
        qApp->processEvents();  
        //clear the socket set  
        FD_ZERO(&readfds);   
     
        //add master socket to set  
        FD_SET(master_socket, &readfds);   
        max_sd = master_socket;   
        
        qApp->processEvents();  
        qApp->processEvents();  
        //add child sockets to set  
        for (int i = 0 ; i < max_num_client ; i++)   
        {   
            //socket descriptor  
            sd = clien_socket[i];   
                 
            //if valid socket descriptor then add to read list  
            if(sd > 0)   
                FD_SET( sd , &readfds);   
                 
            //highest file descriptor number, need it for the select function  
            if(sd > max_sd)   
                max_sd = sd;   
        }   
     
     qApp->processEvents();  
        //wait for an activity on one of the sockets , timeout is NULL ,  
        //so wait indefinitely  
        activity = select( max_sd + 1 , &readfds , NULL , NULL , NULL);   
       
        if ((activity < 0) && (errno!=EINTR))   
        {   
            printf("select error");   
        }   

        qApp->processEvents();  
             
        //If something happened on the master socket ,  
        //then its an incoming connection  
        if (FD_ISSET(master_socket, &readfds))   
        {   

            qApp->processEvents();  
            if ((new_socket = accept(master_socket,  
                    (struct sockaddr *)&serv_addr, (socklen_t*)&addrlen))<0)   
            {   
                perror("accept");   
                exit(EXIT_FAILURE);   
            }   
             
            //inform user of socket number - used in send and receive commands   
           
            //send new connection greeting message  
            // if( send(new_socket, message, strlen(message), 0) != strlen(message) )   
            // {   
            //     perror("send");   
            // }   

            qApp->processEvents();  

            //add new socket to array of sockets  
            for (int i = 0; i < max_num_client; i++)   
            {   
                //if position is empty  
                if( clien_socket[i] == 0 )   
                {   
                    clien_socket[i] = new_socket;   
                         
                    break;   
                }   
            }   
        }   
             
        qApp->processEvents();  
        qApp->processEvents();  
        //else its some IO operation on some other socket 
        for (int i = 0; i < max_num_client; i++)   
        {   
            qApp->processEvents();  
            qApp->processEvents();  

            sd = clien_socket[i];   
                 
            if (FD_ISSET( sd , &readfds))   
            {   
                //Check if it was for closing , and also read the  
                //incoming message  
                bzero(buffer, 256);
                //valread = read(sd, buffer, 1024);
                valread = read(sd, buffer, 256);
                if (valread == 0)   
                {   
                    qApp->processEvents();  
                    //Somebody disconnected , get his details and print  
                    getpeername(sd , (struct sockaddr*)&serv_addr , \ 
                        (socklen_t*)&addrlen);   
                    // printf("Host disconnected , ip %s , port %d \n" ,  
                    //       inet_ntoa(address.sin_addr) , ntohs(address.sin_port));   
                         
                    //Close the socket and mark as 0 in list for reuse  
                    ::close( sd );   
                    clien_socket[i] = 0;   
                }   
                     
                //Echo back the message that came in  
                else 
                {   
                    //set the string terminating NULL byte on the end  
                    //of the data read  

                    qApp->processEvents();  
                    qApp->processEvents();  

                    strtok(buffer, "\n");
                    exist = checkExistPlayer(list_player, buffer);

                    console_view->setText("Checking existing player");

                    qApp->processEvents();  
                    qApp->processEvents();  


                    if(exist){
                        // exist
                        qApp->processEvents();   
                        qApp->processEvents();         
                        console_view->setText("Fail to register player ");
                        send(sd, "fail", 5, 0);

                        qApp->processEvents();   
                        qApp->processEvents();         
                    }
                    else{
                        // not exist
                        qApp->processEvents();  
                        qApp->processEvents();      

                        string full_str = "Welcome player " + string(buffer) + " to join the game";
                        console_view->setText(QString::fromStdString(full_str));
                        
                        //cout<<"Welcome player "<<buffer<<" to join the game"<<endl;
                        char * tmp = (char*) malloc(strlen(buffer)+1);
                        strcpy(tmp, buffer);
                        list_player.insert(tmp);
                        client_name[i] = tmp;
                        send(sd, buffer, strlen(buffer), 0);
                        cur_num_client += 1;
                        num_client_to_start += 1;

                        qApp->processEvents();   
                        qApp->processEvents();         
                    }

                    
                }   
            }

            
        }   

        if(num_client_to_start == max_num_client){
            break;
        }

        qApp->processEvents();  
    }   

    qApp->processEvents();  

    console_view->setText("Game started!");

    qApp->processEvents();  
    qApp->processEvents(); 

    // init game properties from server
    int run_client_id = 0;
    int run_question_id = 0;
    bool isEndGame = false;
    char *clientAnswer = (char*) malloc(strlen(buffer)+1);
    char *respond;
    vector<Question> allQues = getQuestion(max_num_client);
    Question selectedQues;

    qApp->processEvents();  
    
    
    // /// insert demo question here
    /*Question tmp;
    char random_answer[4] = {'A', 'B', 'C', 'D'};
    for(int i = 0; i<max_num_client*3; i++){
        tmp.question = "question "+ to_string(i+1);
        tmp.answerSentence[0] = "this is sentenceA";
        tmp.answerSentence[1] = "this is sentenceB";
        tmp.answerSentence[2] = "this is senCSSADS";
        tmp.answerSentence[3] = "this is senDff";
        tmp.answer = random_answer[i%4];
        allQues.push_back(tmp);
    }*/

    qApp->processEvents();  
    // Question ques1;
    // ques1.question = "question 1";
    // ques1.answer = 'A';

    // Question ques2;
    // ques2.question = "question 2";
    // ques2.answer = 'B';

    // Question ques3;
    // ques3.question = "question 3";
    // ques3.answer = 'C';

    // allQues.push_back(ques1);
    // allQues.push_back(ques2);
    // allQues.push_back(ques3);
    int total_ques = allQues.size();

    qApp->processEvents();  

    /// init all users as alive and with only one turn 
    int *numTurnClient;
    numTurnClient = new int[max_num_client];
    bool *isAlive;
    isAlive = new bool[max_num_client];
    int num_alive = max_num_client;
    for(int i = 0;i <max_num_client;i++){
        numTurnClient[i] = 1;
        isAlive[i] = true;
    }

    

    while(!isEndGame) {

        qApp->processEvents();  
        /// getting the question from question id
        selectedQues = allQues[run_question_id];

        
        /// displaying it on screen
        
        //cout<<"Current question:"<<endl;
        //cout<<selectedQues.question<<endl;
        //cout<<"Solution:"<<selectedQues.answer<<endl;
        qApp->processEvents();  
        qApp->processEvents();  
        qApp->processEvents();  
        qApp->processEvents();  

        question_view->setText(QString::fromStdString(selectedQues.question));
        A_answer->setText(QString::fromStdString(selectedQues.answerSentence[0]));
        B_answer->setText(QString::fromStdString(selectedQues.answerSentence[1]));
        C_answer->setText(QString::fromStdString(selectedQues.answerSentence[2]));
        D_answer->setText(QString::fromStdString(selectedQues.answerSentence[3]));
        
        //QString x=""; 
        //QChar y=selectedQues.answer; 
        //x += y;

        string s(1, selectedQues.answer);
        //cout << "This is answerAAA: " << selectedQues.answer << endl;
        //cout << s << endl;
        true_answer->setText(QString::fromStdString(s));

        qApp->processEvents();  
        qApp->processEvents();  
        qApp->processEvents();  
        qApp->processEvents();  
        /// select the player to send question via socket
        bzero(buffer,256);
        selectedQues.question.copy(buffer, selectedQues.question.size()); // copy to buffer 
        buffer[selectedQues.question.size()] = '\0';//set the delimeter

        send(clien_socket[run_client_id], buffer, strlen(buffer), 0);

        for (int i = 0; i < 4; i++) {

            read(clien_socket[run_client_id], buffer, 256);

            bzero(buffer,256);
            selectedQues.answerSentence[i].copy(buffer, selectedQues.answerSentence[i].size()); // copy to buffer 
            buffer[selectedQues.answerSentence[i].size()] = '\0';//set the delimeter

            send(clien_socket[run_client_id], buffer, strlen(buffer), 0);
        }

        bzero(buffer,256);
        /// waiting for client's answer
        n = read(clien_socket[run_client_id], buffer, 256);

        strtok(buffer, "\n");
        strcpy(clientAnswer, buffer);
        cout<<"Answer from player "<<client_name[run_client_id]<<" is "<<clientAnswer[0]<<endl;
        bzero(buffer,256);
        /// examine client's answer
        if(num_alive == 1 || total_ques == 1){
            // he/she is the last one survive or this is the last question
            respond = "winner";
            send(clien_socket[run_client_id], respond, strlen(respond), 0);
            ::close(clien_socket[run_client_id]);
            break; //end game
        }

        qApp->processEvents();  
        if(clientAnswer[0] == 'S'){
            // first check if that user has any remaining turn
            if(numTurnClient[run_client_id] > 0){
                // decrease turn of that user by one and move to
                // next player (still using the old question id)
                numTurnClient[run_client_id] -= 1;
                respond = "skip";
                send(clien_socket[run_client_id], respond, strlen(respond), 0);
                
                run_client_id = (run_client_id+1)%max_num_client;

                // send feed back    
            }
            else {
                respond = "incorrect";
                isAlive[run_client_id] = false; 
                num_alive -= 1;
                send(clien_socket[run_client_id], respond, strlen(respond), 0);
                ::close(clien_socket[run_client_id]);

                while(isAlive[run_client_id]==false){
                    run_client_id = (run_client_id+1)%max_num_client;
                }
            }
        }
        else if (clientAnswer[0] == selectedQues.answer){
            // client's answer is correct 
            run_question_id += 1;
            respond = "correct";
            total_ques -= 1;
            //send feedback
            send(clien_socket[run_client_id], respond, strlen(respond), 0);
        }
        else{
            // client's answer is incorrect
            respond = "incorrect";
            isAlive[run_client_id] = false; 
            num_alive -= 1;
            send(clien_socket[run_client_id], respond, strlen(respond), 0);
            ::close(clien_socket[run_client_id]);

            while(isAlive[run_client_id]==false){
                run_client_id = (run_client_id+1)%max_num_client;
            }
        }

        qApp->processEvents();  

        cout<<"On checking the answer"<<endl;
        // send response to client's answer
        // repeats
        // waiting to confirm that client have receive response
        // n = read(clien_socket[run_client_id], buffer, 256);
        sleep(1);
        cout<<respond<<endl;

        qApp->processEvents();  
    }

        for(int i=0; i<max_num_client; i++)
        {
            if(isAlive[i]){
                respond = "end game";
                send(clien_socket[i], respond, strlen(respond), 0);
                ::close(clien_socket[i]);
        }
    }

    string full_final = "Congratulations " + client_name[run_client_id] + " on winning this game!!!";
    console_view->setText(QString::fromStdString(full_final));
}
