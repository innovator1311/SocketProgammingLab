/* The port number is passed as an argument */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <set>
#include <string>

#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

using namespace std;

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
int main(int argc, char *argv[])
{
    int opt = 1;
    int sockfd, newsockfd, portno, newsockfd2, master_socket, 
        max_sd, sd, activity, new_socket, addrlen, valread;
    socklen_t clilen;
    char buffer[256];
    struct sockaddr_in serv_addr, cli_addr;
    int n;

    fd_set readfds;

    if (argc < 2) {
        fprintf(stderr,"ERROR, no port provided\n");
        exit(1);
    }
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

     portno = atoi(argv[1]);

     /* setup the host_addr structure for use in bind call */
     // server byte order
     serv_addr.sin_family = AF_INET;  

     // automatically be filled with current host's IP address
     serv_addr.sin_addr.s_addr = INADDR_ANY;  

     // convert short integer value for port must be converted into network byte order
     serv_addr.sin_port = htons(portno);

     // This bind() call will bind  the socket to the current IP address on port, portno
    //  if (bind(sockfd, (struct sockaddr *) &serv_addr,
    //           sizeof(serv_addr)) < 0) 
    //           error("ERROR on binding");
    if (bind(master_socket, (struct sockaddr *) &serv_addr,
              sizeof(serv_addr)) < 0) 
              error("ERROR on binding");

    // set number of pending connection
    if (listen(master_socket, 3) < 0)   
    {   
        perror("listen");   
        exit(EXIT_FAILURE);   
    } 
    //Nhap so luong client
    int max_num_client;
    int cur_num_client=0;
    printf("Enter number of player: ");
    scanf("%d", &max_num_client);
	printf("Waiting for players to register\n");
    
    set<char *> list_player; // init list of players nickname
    int * clien_socket = new int[max_num_client]; // init list of players socket

    // init all socket to 0
    for(int i=0; i<max_num_client; i++){
        clien_socket[i] = 0;
    }
    addrlen = sizeof(serv_addr); 
    char *message = "ECHO Daemon v1.0 \r\n";
    bool exist;
    // Waiting for players to register
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
                        send(sd, buffer, strlen(buffer), 0);
                        cur_num_client += 1;
                    }
                }   
            }   
        }   
    }   
         
    return 0;   
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
