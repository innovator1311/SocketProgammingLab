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
void error(const char *msg)
{
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[])
{
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;

    char buffer[256];
    if (argc < 3) {
       fprintf(stderr,"usage %s hostname port\n", argv[0]);
       exit(0);
    }
    portno = atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
        error("ERROR opening socket");
    server = gethostbyname(argv[1]);
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

    

    if (connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) 
        error("ERROR connecting");
    //else
    //    printf("Login successfully");

    // confirm success
    printf("Enter your nickname: ");
    bzero(buffer,256);
    fgets(buffer,255,stdin);

    n = write(sockfd, buffer, strlen(buffer));
    if (n < 0) 
        error("ERROR writing from socket");

    bzero(buffer,256);
    n = read(sockfd, buffer, 255);
    if (n < 0) 
        error("ERROR reading from socket");

    char * tmp = (char*) malloc(strlen(buffer)+1);
    strcpy(tmp, buffer);
    while (strcmp(tmp, "fail")==0){
        // received zero bytes means to be failed 
        printf("Your nickname is not valid or has exist, re-enter your nickname pls\n");
        printf("Enter your nickname again: ");

        bzero(buffer,256);
        fgets(buffer,255,stdin);
        n = write(sockfd, buffer, strlen(buffer));
        if (n < 0) 
            error("ERROR writing from socket");
        bzero(buffer,256);
        n = read(sockfd, buffer, 255);
        if (n < 0) 
            error("ERROR reading from socket");
        tmp = (char*) malloc(strlen(buffer)+1);
        strcpy(tmp, buffer);
    }
    printf("%s\n", buffer);
        
    int exit = 1;

    // enter the game
    // while (exit) {

    //     // receive question
    //     bzero(buffer,256);
    //     n = read(sockfd, buffer, 255);
    //     if (n < 0) 
    //         error("ERROR reading from socket");
    //     printf("%s\n", buffer);

    //     // send answer
    //     printf("\n Enter your nickname: ");
    //     bzero(buffer,256);
    //     fgets(buffer,255,stdin);
    //     n = write(sockfd, buffer, strlen(buffer));
    //     if (n < 0) 
    //         error("ERROR writing to socket");
        
    // }
    close(sockfd);
    return 0;
}