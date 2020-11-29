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
    cout<<"Welcome player "<<buffer<<" to join the game"<<endl;
    cout<<"Please wait for other players"<<endl;
    char message[256];
    string realMessage;

    //-----------------------
    // waiting for confirmation of server to start game
    while(true){
        n = read(sockfd, message, 255);
        realMessage = convertMes(message);
        if(realMessage == "start game")
            break;
    }
    //-----------------------

    //After receive, send verification back
    write(sockfd, "ready", 1);

    // enter the game, init game properties from player
    char answer; // answer from player
    bool isValidAnswer=true; // condition for a valid answer from player's input
    char * responseServer; // response from server upon player's answer
    bool outOfSkipTurn = false;
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
        cout<<"Waiting for your turn!"<<endl;

        // ---------------------
        while(true){
            n = read(sockfd, message, 255);
            cout<<"Message receive"<<message<<endl;
            realMessage = convertMes(message);
            if(realMessage == "question" || realMessage == "end game")
                break;
        }
        // ---------------------

        if(realMessage == "end game"){
            isWinning = 0;
            break;
        }
        cout<<"Your turn now!!Input A,B,C,D for answering or S for skipping"<<endl;
        cout<<"Question:"<<message<<endl;

        while(true){
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
            error("ERROR reading from socket");

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
            n = read(sockfd, message, 255);
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
        cout<<"Congratulations!!! You are the winner"<<endl;
    }
    else if(isWinning==-1){
        cout<<"You are disqualified :( Good luck for next game"<<endl;
    }
    else if(isWinning==0){
        cout<<"Some one has successfully answered all questions. Good luck for next game!"<<endl;
    }
    close(sockfd);
    return 0;
}