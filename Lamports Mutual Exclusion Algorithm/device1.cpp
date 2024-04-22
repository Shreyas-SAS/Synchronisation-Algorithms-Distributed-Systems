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

string ipDev2 = ""; //B
string ipDev3 = ""; //C

int PORT = 8001;

int PIDS = 0;
int TSS = 0;

pthread_t sender, receiver;

map<pair<int,int>,int> mp;

struct arg_struct {
    int arg1;
    int arg2;
}senderArgs;

int replyCount=0;

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
    struct arguments *arguments_struct = (struct arguments *)args;
    int mType = arguments_struct->num1;
    int PIDR = arguments_struct->num2;

    TSS+=1; // clock increment before send

    char *message = (char *)malloc(messageSize);

    memset(message, 0, sizeof(message));
    message = "";

    if(mType==0){ //Request (broadcast to ipDev2 and ipDev3)
        message += to_string(0);
        message += to_string(PIDS);
        message += to_string(TSS);

        int client_fd = initiateSenderSocket(ipDev2);
        printf("%s\n")
        send(client_fd, message, strlen(message), 0);
        close(client_fd);

        int client_fd = initiateSenderSocket(ipDev3);
        send(client_fd, message, strlen(message), 0);
        close(client_fd);
    }
    else if(mType==1){ //reply to requester
        message += to_string(1);
        message += to_string(TSS);

        if(PIDR==1)iprec=ipDev2;
        else if(PIDR==2)iprec=ipDev3;

        int client_fd = initiateSenderSocket(iprec);
        send(client_fd, message, strlen(message), 0);
        close(client_fd);
    }
    else if(mType==2){ //release broadcast
        message += to_string(2);
        message += to_string(TSS);

        int client_fd = initiateSenderSocket(ipDev2);
        send(client_fd, message, strlen(message), 0);
        close(client_fd);

        int client_fd = initiateSenderSocket(ipDev3);
        send(client_fd, message, strlen(message), 0);
        close(client_fd);
    }
    else{
        printf("Error! invalid mType!");
    }
}

void *receive_thread(void *arg)
{   
    // message received in buffer
    int mType = buffer[0];
    if(mtype==0){
        int PIDR=buffer[1];
        int TSR=0, i=2;
        while(i<=messageSize){
            TSR=TSR*10+buffer[i];
        }
    }
    else{
        int TSR=0, i=1;
        while(i<=messageSize){
            TSR=TSR*10+buffer[i];
        }
    }

    TSS = max(TSS,TSR)+1; //Lamports clock on receive

    if(mType==0){
        // add to queue
        pair<int,int> pair;
        pair.first = TSR;
        pair.second = PIDR;
        mp[pair]++;

        senderArgs rep;
        rep.first = 1;
        rep.second = PIDR;

        if (pthread_create(&sender, NULL, &send_thread, (void *)&rep) != 0) {
            printf("Couldn't send reply message request to sender\n");
            return -1;
        }
    }
    else if(mType==1){
        replyCount++;
        if(replyCount==2){
            senderArgs rep;
            rep.first = 2;
            rep.second = PIDR;
            printf("P%d entered critical section",&PIDS)
            wait(1000); // CS access wait
            if (pthread_create(&sender, NULL, &send_thread, (void *)&rep) != 0) {
                printf("Couldn't send reply message request to sender\n");
                return -1;
            }
            replyCount=0;
        }
    }
    else if(mType==2){
        mp.erase(mp.begin()); // pop top in ordered map
    }
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

    pthread_create(&receiver, NULL, &recive_thread, &server_fd);

    memset(buffer, 0, sizeof(buffer));
    recv(server_fd, buffer, messageSize, 0); //run listner

    pthread_join(receive,NULL);

    // simutaion
    srand(time(0));
    delay = rand() % 11;
    wait(delay*1000);

    senderArgs rep;
    rep.first = 0;
    rep.second = PIDS;
    pthread_create(&sender, NULL, &send_thread, (void *)&rep);
    pthread_join(server,NULL);

    // simulation close
    wait(5000); // close simulation after 5s of 1st request
    return 0;
}