//Header Files
#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h>
#include <stdio.h>  
#include <arpa/inet.h>

#define MAX_NO_OF_CLIENTS 1000

void start(int sock_cli){
    char username[1024];
    char password[1024];

    send(sock_cli, "welcome to bank :) \n" , strlen("welcome to bank :) \n") , 0 );
    send(sock_cli, "Enter you username:" , strlen("Enter you username:") , 0 );

    bzero(username, sizeof(username)); 
    read(sock_cli,username, sizeof(username)); 
    printf("%s",username);

    send(sock_cli, "Enter you password:" , strlen("Enter you password:") , 0 );


    bzero(password, sizeof(password)); 
    read(sock_cli,password, sizeof(password)); 
    printf("%s",password);
}

int main(int argc,char *argv[]){

    if(argc!=2){
        printf("Invalid arguments in command line \n");
        return -1;
    }

    int portNo = atoi(argv[1]);
    printf("Port No: %d \n",portNo);

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd < 0){
        printf("error in socket \n");
        return -1;
    }else{
        printf("Socket made \n");
    }

    struct sockaddr_in sAddress;
    sAddress.sin_family = AF_INET; 
    sAddress.sin_addr.s_addr = INADDR_ANY; 
    sAddress.sin_port = htons(portNo);  

    if(bind(sockfd,(struct sockaddr *)&sAddress,sizeof(sAddress)) < 0){ 
        printf("binding failed \n"); 
        return -1; 
    }else{
        printf("binding done \n");
    }

    if(listen(sockfd,MAX_NO_OF_CLIENTS) < 0){ 
        printf("listening error \n"); 
        return -1;
    }else{
        printf("listening \n");
    }
    
    int addrlen = sizeof(sAddress);
    int sock_cli;

    while(1){

        sock_cli = accept(sockfd, (struct sockaddr *)&sAddress,(socklen_t*)&addrlen);

        if(sock_cli < 0){
            printf("error");
            return -1;
        }

        int p_id = fork();

        if(p_id < 0 ){
            printf("fork error");
            close(sock_cli);
            return -1;
        }
        
        if(p_id == 0){
            start(sock_cli);
            close(sock_cli);
            return 0;
        }
    }
    
    return 0;
}