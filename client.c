//Header Files
#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h>
#include <stdio.h>  
#include <arpa/inet.h>

int main(int argc,char *argv[]){

    if(argc!=3){
        printf("Invalid arguments in command line");
        return -1;
    }

    char *serverIPAddr = argv[1];
    int serverPortNo = atoi(argv[2]);

    printf("IP addr:%s \n",serverIPAddr);
    printf("Port No:%d \n",serverPortNo);

    int clientfd = socket(AF_INET, SOCK_STREAM, 0);

    if(clientfd < 0){
        printf("error in socket \n");
        return -1;
    }else{
        printf("Socket made \n");
    }

    struct sockaddr_in serverAddr;

    serverAddr.sin_family = AF_INET; 
    serverAddr.sin_port = htons(serverPortNo); 
       
    // Convert IPv4 and IPv6 addresses from text to binary form 
    if(inet_pton(AF_INET, serverIPAddr , &serverAddr.sin_addr) <= 0){ 
        printf("Invaild Address \n"); 
        return -1; 
    }  

    if(connect(clientfd, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0){ 
        printf("Connection Failed \n"); 
        return -1; 
    }else{
        printf("Connection made \n");
    }

    send(clientfd, "hello" , strlen("hello") , 0 ); 

    printf("Hello message sent\n"); 

    // int valread = read( sock , buffer, 1024); 

    // printf("%s\n",buffer ); 

    return 0;
}