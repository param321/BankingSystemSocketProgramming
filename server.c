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

    if(argc!=2){
        printf("Invalid arguments in command line \n");
        return -1;
    }

    int portNo = atoi(argv[1]);
    printf("Port No: %d \n",portNo);


    return 0;
}