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
    return 0;
}