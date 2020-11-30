/* The port number is passed as an argument */
/*#include <stdio.h>
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
#include <arpa/inet.h>*/

/* ui lib*/
#include "mainwindow.h"

using namespace std;

struct Question{
    string question;
    char answer;
};


vector<Question> getListQuestion(){
    vector<Question> myQues;
    return myQues;
}


int main(int argc, char *argv[])
{
    // UI code
    QApplication app(argc, argv);
    MainWindow mainWindow;
    mainWindow.showMaximized();
    mainWindow.setFixedSize(500,500);
    mainWindow.setStyleSheet("background-color: white;");
    //mainWindow.printConsole("");
    //

    return app.exec();

    
    /*
    
    while(true)   
    {   
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
        activity = select( max_sd + 1 , &readfds , NULL , NULL , NULL);   
       
        if ((activity < 0) && (errno!=EINTR))   
        {   
            printf("select error");   
        }   
             
        //If something happened on the master socket ,  
        //then its an incoming connection  
        if (FD_ISSET(master_socket, &readfds))   
        {   
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
            }   
        }   
             
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
                    close( sd );   
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
        if(num_client_to_start == max_num_client){
            break;
        }
    }   
    // Game started 

    cout<<"Game started!"<<endl;
    cout<<"List of player name:"<<endl;
    for(int i=0; i<max_num_client; i++){
        cout<<i+1<<"."<<client_name[i]<<endl;
        // signal the player to start game, must wait until the client confirm 
        send(clien_socket[i], "start game", 10, 0);
        n = read(clien_socket[i], buffer, 255);
        cout<<"Response from player "<<buffer<<endl;
    }

    // init game properties from server
    int run_client_id = 0;
    int run_question_id = 0;
    bool isEndGame = false;
    char *clientAnswer = (char*) malloc(strlen(buffer)+1);
    char *respond;
    vector<Question> allQues;
    Question selectedQues;
    
    // /// insert demo question here
    Question tmp;
    char random_answer[4] = {'A', 'B', 'C', 'D'};
    for(int i = 0; i<max_num_client*3; i++){
        tmp.question = "question "+ to_string(i+1);
        tmp.answer = random_answer[i%4];
        allQues.push_back(tmp);
    }
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

    while(!isEndGame){
        /// getting the question from question id
        selectedQues = allQues[run_question_id];
        /// displaying it on screen
        cout<<"Current question:"<<endl;
        cout<<selectedQues.question<<endl;
        cout<<"Solution:"<<selectedQues.answer<<endl;
        /// select the player to send question via socket
        bzero(buffer,256);
        selectedQues.question.copy(buffer, selectedQues.question.size()+1); // copy to buffer 
        buffer[selectedQues.question.size()] = '\0';//set the delimeter

        send(clien_socket[run_client_id], buffer, strlen(buffer), 0);
        /// waiting for client's answer
        n = read(clien_socket[run_client_id], buffer, 255);

        strtok(buffer, "\n");
        strcpy(clientAnswer, buffer);
        cout<<"Answer from player "<<client_name[run_client_id]<<" is "<<clientAnswer[0]<<endl;
        bzero(buffer,256);
        /// examine client's answer
        if(num_alive == 1 || total_ques == 1){
            // he/she is the last one survive or this is the last question
            respond = "winner";
            send(clien_socket[run_client_id], respond, strlen(respond), 0);
            close(clien_socket[run_client_id]);
            break; //end game
        }
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
            close(clien_socket[run_client_id]);

            while(isAlive[run_client_id]==false){
                run_client_id = (run_client_id+1)%max_num_client;
            }
        }
        cout<<"On checking the answer"<<endl;
        // send response to client's answer
        // repeats
        // waiting to confirm that client have receive response
        // n = read(clien_socket[run_client_id], buffer, 255);
        sleep(1);
        cout<<respond<<endl;
    }
    for(int i=0; i<max_num_client; i++)
    {
        if(isAlive[i]){
            respond = "end game";
            send(clien_socket[i], respond, strlen(respond), 0);
            close(clien_socket[i]);
        }
    }
    cout<<"Congratulations "<<client_name[run_client_id]<<" on winning this game!!!"<<endl;

    return app.exec();*/
}   
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
//     int i = 0;
//     // Connect the client
//     for(int i = 0; i<cur_num_client; i++){
//         clienSocket[i] = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
//         cout<<"Connecting client:"<<i+1<<endl;
//     }

//     // Waiting for request from client
//     for(int i=0; i<cur_num_client; i++) {

//         // bzero(buffer, 256);
//         // get nick name from user
//         n = read(clienSocket[i], buffer, 255);
//         strtok(buffer, "\n");

//         // check existing name 
//         bool exist = checkExistPlayer(list_player, buffer);



//         if(exist){
//             // exist
//             cout<<"Fail to register player "<< buffer <<endl;
//             send(clienSocket[i], "fail", 5, 0);
//         }
//         else{
//             // not exist
//             cout<<"Welcome player "<<buffer<<" to join the game"<<endl;
//             char * tmp = (char*) malloc(strlen(buffer)+1);
//             strcpy(tmp, buffer);
//             list_player.insert(tmp);
//             send(clienSocket[i], buffer, 255, 0);
//             cur_num_client += 1;
//         }
//     }

//     printf("After listen");
//     printf("\n There are %d player register in this game", cur_num_client);
//      // The accept() call actually accepts an incoming connection
//     clilen = sizeof(cli_addr);

//      // This accept() function will write the connecting client's address info 
//      // into the the address structure and the size of that structure is clilen.
//      // The accept() returns a new socket file descriptor for the accepted connection.
//      // So, the original socket file descriptor can continue to be used 
//      // for accepting new connections while the new socker file descriptor is used for
//      // communicating with the connected client.
//     /*newsockfd = accept(sockfd, 
//                  (struct sockaddr *) &cli_addr, &clilen);

//     newsockfd2 = accept(sockfd, 
//                  (struct sockaddr *) &cli_addr, &clilen);*/

//      if (newsockfd < 0) 
//           error("ERROR on accept");

//     //  printf("server: got connection from %s port %d\n",
//     //         inet_ntoa(cli_addr.sin_addr), ntohs(cli_addr.sin_port));

//     /*for (int i = 0; i < num_client; i++)
//             // This send() function sends the 13 bytes of the string to the new socket
//             send(clienSocket[i], "Hello, world!\n", 13, 0);*/

//     int exit = 1;
//     int client_turn = 0;
//     int question_idx = 0;
//     string questions[5] = {"ques1","ques2","ques3","ques4","ques5"};

//     // for (int i = 0; i < max_num_client; i++) {
//     //     nickname = read(clienSocket[i], buffer, 255);
//     //     printf("\n Receive nickname %s from %d player\n", nickname, i);
//     // }
//     // start the game
//     // while (exit) {

//     //     send(clienSocket[client_turn], "some questions", 13, 0);

//     //     bzero(buffer,256);

//     //     n = read(clienSocket[client_turn],buffer,255);
//     //     if (n < 0) error("ERROR reading from socket");
//     //     else {
//     //         printf("Client answer: %s\n",buffer);
//     //         client_turn += 1;
//     //         if (client_turn == num_client) client_turn = 0;
//     //     }
//     //     //client_turn += 1;
//     // }
     
    


//      close(newsockfd);
//      close(sockfd);
    
//      return 0; 
// }
