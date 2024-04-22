#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <semaphore.h>
#include<bits/stdc++.h>

string ipDev2 = ""; //A
string ipDev3 = ""; //B

int PORT = 8001;

int PIDS = 2;
int TSS = 0;

pthread_t sender, receiver;

map<pair<int,int>,int> mp;

struct arg_struct {
    int arg1;
    int arg2;
}senderArgs;

int replyCount=0;
string iprec;

int initiateSenderSocket(ip)
{
    int status, valread, client_fd;
    struct sockaddr_in serv_addr;

    char buffer[1024] = { 0 };
    if ((client_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        return -1;
    }
 
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
 
    if (inet_pton(AF_INET, ip, &serv_addr.sin_addr)
        <= 0) {
        printf(
            "\nInvalid address/ Address not supported \n");
        return -1;
    }
 
    if ((status = connect(client_fd, (struct sockaddr*)&serv_addr,sizeof(serv_addr)))< 0) {
        printf("\nConnection Failed \n");
        return -1;
    }

    return client_fd;
}

void *send_thread(void *arg)
{
    TSS+=1; // clock increment before send

    char *message = (char *)malloc(messageSize);

    memset(message, 0, sizeof(message));
    message = "";
    message += to_string(PIDS);
    message += to_string(TSS);

    // Decide recipend of send
    srand(time(0));
    rec = rand() % 11;
    if(rec<=5)iprec=ipDev2;
    else iprec=ipDev3;
    printf("send to IP %s\n"&iprec);
    int client_fd = initiateSenderSocket(iprec);
    send(client_fd, message, strlen(message), 0);
    close(client_fd);
}

void *receive_thread(void *arg)
{   
    // message received in buffer
    int TSR=0, i=1;
    int PIDR = buffer[0];
    while(i<=messageSize){
        TSR=TSR*10+buffer[i];
    }
    printf("receive from P%d at TS %d"&PIDS,&TSR);
    TSS = max(TSS,TSR)+1; //Lamports clock on receive
    
    else{
        printf("Error! invalid mType!");
    }

    free(buffer);
    pthread_join(receiver,NULL);
    recv(server_fd, buffer, messageSize, 0); // infinite recursive call
}

void main(){
    int server_fd, new_socket;
    ssize_t valread;
    struct sockaddr_in address;
    int opt = 1;
    socklen_t addrlen = sizeof(address);
    char buffer[1024] = { 0 };
 
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
 
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
 
    if (bind(server_fd, (struct sockaddr*)&address,sizeof(address))< 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    if ((new_socket= accept(server_fd, (struct sockaddr*)&address,&addrlen))< 0) {
        perror("accept");
        exit(EXIT_FAILURE);
    }
    valread = read(new_socket, buffer,1024 - 1); 

    pthread_create(&receiver, NULL, &recive_thread, NULL);

    memset(buffer, 0, sizeof(buffer));
    recv(server_fd, buffer, messageSize, 0); //run listner

    pthread_join(receive,NULL);

    // simutaion
    while(1){
        srand(time(0));
        choice = rand() % 11;
        if(choice<=5){
            printf("internal event at P%d",&PIDS);
            TSS++;
        }else{
            pthread_create(&sender, NULL, &send_thread,NULL);
            pthread_join(receive,NULL);
        }
        wait(1000+(choice*1000)%4000); // wait for random 1-5s
    }

    return 0;
}