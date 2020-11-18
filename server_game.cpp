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
    
     int sockfd, newsockfd, portno, newsockfd2;
     socklen_t clilen;
     char buffer[256];
     struct sockaddr_in serv_addr, cli_addr;
     int n;
     if (argc < 2) {
         fprintf(stderr,"ERROR, no port provided\n");
         exit(1);
     }
     // create a socket
     // socket(int domain, int type, int protocol)
     sockfd =  socket(AF_INET, SOCK_STREAM, 0);
     if (sockfd < 0) 
        error("ERROR opening socket");

    
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

     // bind(int fd, struct sockaddr *local_addr, socklen_t addr_length)
     // bind() passes file descriptor, the address structure, 
     // and the length of the address structure
     // This bind() call will bind  the socket to the current IP address on port, portno
     if (bind(sockfd, (struct sockaddr *) &serv_addr,
              sizeof(serv_addr)) < 0) 
              error("ERROR on binding");

     // This listen() call tells the socket to listen to the incoming connections.
     // The listen() function places all incoming connection into a backlog queue
     // until accept() call accepts the connection.
     // Here, we set the maximum size for the backlog queue to 5.
    
    //Nhap so luong client
    int max_num_client = 100;
    int cur_num_client;
    printf("Enter number of player: ");
    scanf("%d", &cur_num_client);
	printf("Waiting for players to register\n");

    set<char *> list_player; // init list of players nickname
    int * clienSocket = new int[max_num_client]; // init list of players socket

    listen(sockfd,5);
    int i = 0;
    // Connect the client
    for(int i = 0; i<cur_num_client; i++){
        clienSocket[i] = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
        cout<<"Connecting client:"<<i+1<<endl;
    }

    // Waiting for request from client
    for(int i=0; i<cur_num_client; i++) {

        // bzero(buffer, 256);
        // get nick name from user
        n = read(clienSocket[i], buffer, 255);
        strtok(buffer, "\n");

        // check existing name 
        bool exist = checkExistPlayer(list_player, buffer);



        if(exist){
            // exist
            cout<<"Fail to register player "<< buffer <<endl;
            send(clienSocket[i], "fail", 5, 0);
        }
        else{
            // not exist
            cout<<"Welcome player "<<buffer<<" to join the game"<<endl;
            char * tmp = (char*) malloc(strlen(buffer)+1);
            strcpy(tmp, buffer);
            list_player.insert(tmp);
            send(clienSocket[i], buffer, 255, 0);
            cur_num_client += 1;
        }
    }

    printf("After listen");
    printf("\n There are %d player register in this game", cur_num_client);
     // The accept() call actually accepts an incoming connection
    clilen = sizeof(cli_addr);

     // This accept() function will write the connecting client's address info 
     // into the the address structure and the size of that structure is clilen.
     // The accept() returns a new socket file descriptor for the accepted connection.
     // So, the original socket file descriptor can continue to be used 
     // for accepting new connections while the new socker file descriptor is used for
     // communicating with the connected client.
    /*newsockfd = accept(sockfd, 
                 (struct sockaddr *) &cli_addr, &clilen);

    newsockfd2 = accept(sockfd, 
                 (struct sockaddr *) &cli_addr, &clilen);*/

     if (newsockfd < 0) 
          error("ERROR on accept");

    //  printf("server: got connection from %s port %d\n",
    //         inet_ntoa(cli_addr.sin_addr), ntohs(cli_addr.sin_port));

    /*for (int i = 0; i < num_client; i++)
            // This send() function sends the 13 bytes of the string to the new socket
            send(clienSocket[i], "Hello, world!\n", 13, 0);*/

    int exit = 1;
    int client_turn = 0;
    int question_idx = 0;
    string questions[5] = {"ques1","ques2","ques3","ques4","ques5"};

    // for (int i = 0; i < max_num_client; i++) {
    //     nickname = read(clienSocket[i], buffer, 255);
    //     printf("\n Receive nickname %s from %d player\n", nickname, i);
    // }
    // start the game
    // while (exit) {

    //     send(clienSocket[client_turn], "some questions", 13, 0);

    //     bzero(buffer,256);

    //     n = read(clienSocket[client_turn],buffer,255);
    //     if (n < 0) error("ERROR reading from socket");
    //     else {
    //         printf("Client answer: %s\n",buffer);
    //         client_turn += 1;
    //         if (client_turn == num_client) client_turn = 0;
    //     }
    //     //client_turn += 1;
    // }
     
    


     close(newsockfd);
     close(sockfd);
    
     return 0; 
}
