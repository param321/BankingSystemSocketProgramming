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
        print("Invalid arguments in command line");
        return -1;
    }

    char *serverIPAddr = argv[1];
    int serverPortNo = atoi(argv[2]);

    printf("IP addr:%s \n",serverIPAddr);
    printf("Port No:%d \n",serverPortNo);

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd < 0){
        printf("error in socket");
        return -1;
    }
    return 0;
}