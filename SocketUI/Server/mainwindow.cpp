#include "mainwindow.h"

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

MainWindow::MainWindow(QWidget *parent)
  : QMainWindow(parent)
{

  console_view = new QTextEdit("text edit",this);
  console_view->setGeometry(QRect(QPoint(20, 20), QSize(450, 100)));
  console_view->setDisabled(true);

  player_number = new QTextEdit("",this);
  player_number->setGeometry(QRect(QPoint(40, 130), QSize(50, 20)));

  question = new QTextEdit("",this);
  question->setGeometry(QRect(QPoint(20, 160), QSize(450, 50)));
  question->setDisabled(true);

  true_answer = new QTextEdit("",this);
  true_answer->setGeometry(QRect(QPoint(200, 220), QSize(100, 30)));
  true_answer->setDisabled(true);

  A_answer = new QTextEdit("",this);
  A_answer->setGeometry(QRect(QPoint(20, 260), QSize(120, 40)));
  A_answer->setDisabled(true);

  B_answer = new QTextEdit("",this);
  B_answer->setGeometry(QRect(QPoint(350, 260), QSize(120, 40)));
  B_answer->setDisabled(true);

  C_answer = new QTextEdit("",this);
  C_answer->setGeometry(QRect(QPoint(20, 320), QSize(120, 40)));
  C_answer->setDisabled(true);

  D_answer = new QTextEdit("",this);
  D_answer->setGeometry(QRect(QPoint(350, 320), QSize(120, 40)));
  D_answer->setDisabled(true);

  listen_button = new QPushButton("Listen", this);
  listen_button->setGeometry(QRect(QPoint(350, 400), QSize(80, 20)));

  connect(listen_button, &QPushButton::released, this, &MainWindow::handleListen);


  /*text_edit = new QTextEdit("text edit", this);
  text_edit->setGeometry(QRect(QPoint(50, 50), QSize(200, 50)));
  text_edit->setDisabled(true);

  // Create the button, make "this" the parent
  m_button = new QPushButton("My Button", this);
  // set size and location of the button
  m_button->setGeometry(QRect(QPoint(100, 100), QSize(200, 50)));

  // Create the button, make "this" the parent
  m_button_1 = new QPushButton("My Button", this);
  // set size and location of the button
  m_button_1->setGeometry(QRect(QPoint(500, 100), QSize(200, 50)));

  // Create the button, make "this" the parent
  m_button_2 = new QPushButton("My Button", this);
  // set size and location of the button
  m_button_2->setGeometry(QRect(QPoint(100, 500), QSize(200, 50)));

  // Create the button, make "this" the parent
  m_button_3 = new QPushButton("My Button", this);
  // set size and location of the button
  m_button_3->setGeometry(QRect(QPoint(500, 500), QSize(200, 50)));
  
 
  // Connect button signal to appropriate slot
  connect(a_button, &QPushButton::released, this, &MainWindow::handleButton);
  connect(b_button, &QPushButton::released, this, &MainWindow::handleButton1);*/
}
 
/*void MainWindow::handleButton()
{
  // change the text
  m_button->setText("Example");
  // resize button
  m_button->resize(100,100);

  
}

void MainWindow::handleButton1() 
{
    // change the text
  m_button_1->setText("Exampledddddd");
  // resize button
  m_button_1->resize(40,40);

}*/

void MainWindow::handleListen() {

  QString text = player_number->toPlainText();
  int client_number = text.toInt();

  int opt = 1;
    int sockfd, newsockfd, portno, newsockfd2, master_socket, 
        max_sd, sd, activity, new_socket, addrlen, valread;
    socklen_t clilen;
    char buffer[256];
    struct sockaddr_in serv_addr, cli_addr;
    int n;
    
    fd_set readfds;

    //if (argc < 2) {
    //    fprintf(stderr,"ERROR, no port provided\n");
    //    exit(1);
    //}
    // create a socket
    // socket(int domain, int type, int protocol)

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
    //  sockfd =  socket(AF_INET, SOCK_STREAM, 0);
    //  if (sockfd < 0) 
    //     error("ERROR opening socket");

    
     // clear address structure
     bzero((char *) &serv_addr, sizeof(serv_addr));

     //portno = atoi(argv[1]);

    portno = 15;

     /* setup the host_addr structure for use in bind call */
     // server byte order
     serv_addr.sin_family = AF_INET;  

     // automatically be filled with current host's IP address
     serv_addr.sin_addr.s_addr = INADDR_ANY;  

     // convert short integer value for port must be converted into network byte order
     serv_addr.sin_port = htons(portno);

    //bind(master_socket, (struct sockaddr *) &serv_addr,sizeof(serv_addr));

     // This bind() call will bind  the socket to the current IP address on port, portno
    //  if (bind(sockfd, (struct sockaddr *) &serv_addr,
    //           sizeof(serv_addr)) < 0) 
    //           error("ERROR on binding");
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
    int max_num_client=3;
    console_view->setText("max client");
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

    //while(num_client_to_start != max_num_client)   
    //{   
     // console_view->setText("go into loop");
        //clear the socket set  
        FD_ZERO(&readfds);   
     
        //add master socket to set  
        FD_SET(master_socket, &readfds);   
        max_sd = master_socket;   
      
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
     
        //wait for an activity on one of the sockets , timeout is NULL ,  
        //so wait indefinitely 

        /*activity = select( max_sd + 1 , &readfds , NULL , NULL , NULL);   
       
        if ((activity < 0) && (errno!=EINTR))   
        {   
            printf("select error");   
        } */

        FD_ISSET(master_socket, &readfds);

        
        //If something happened on the master socket ,  
        //then its an incoming connection  
        if (FD_ISSET(master_socket, &readfds))   
        {   
            //accept(master_socket,  
            //        (struct sockaddr *)&serv_addr, (socklen_t*)&addrlen);

            accept(master_socket, (struct sockaddr *)&serv_addr, (socklen_t*)&addrlen);

            /*
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



            //add new socket to array of sockets  
            for (int i = 0; i < max_num_client; i++)   
            {   
                //if position is empty  
                if( clien_socket[i] == 0 )   
                {   
                    clien_socket[i] = new_socket;   
                         
                    break;   
                }   
            }   */
        }   
        /*
        //else its some IO operation on some other socket 
        for (int i = 0; i < max_num_client; i++)   
        {   
            sd = clien_socket[i];   
                 
            if (FD_ISSET( sd , &readfds))   
            {   
                //Check if it was for closing , and also read the  
                //incoming message  
                valread = read(sd, buffer, 1024);
                if (valread == 0)   
                {   
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
                    strtok(buffer, "\n");
                    exist = checkExistPlayer(list_player, buffer);
                    if(exist){
                        // exist
                        cout<<"Fail to register player "<< buffer <<endl;
                        send(sd, "fail", 5, 0);
                    }
                    else{
                        // not exist
                        cout<<"Welcome player "<<buffer<<" to join the game"<<endl;
                        char * tmp = (char*) malloc(strlen(buffer)+1);
                        strcpy(tmp, buffer);
                        list_player.insert(tmp);
                        client_name[i] = tmp;
                        send(sd, buffer, strlen(buffer), 0);
                        cur_num_client += 1;
                        num_client_to_start += 1;
                    }
                }   
            }   
        }   
        //if(num_client_to_start == max_num_client){
        //    break;
        //}
    //}   */

    
}

