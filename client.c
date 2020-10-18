//Header Files
#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h>
#include <stdio.h>  
#include <arpa/inet.h>

void customer(int clientfd){
    while(1){
        printf("To see your\n Available Balance type BAL\n MINI Statement type MINI\n To exit Type EXIT\n");
        char buffer[1024];

        bzero(buffer, sizeof(buffer)); 
        int n = 0; 
        while((buffer[n++] = getchar()) != '\n'); 
        write(clientfd, buffer , strlen(buffer)); 

        if(strcmp(buffer,"EXIT\n")==0){
            printf("EXITING\n");
            close(clientfd);
            break;
        }

        bzero(buffer, sizeof(buffer)); 
        read(clientfd,buffer, sizeof(buffer)); 
        printf("%s",buffer);

    }
    return;
}

void admin(int clientfd){
    while(1){
        printf("If you want to DEBIT/CREDIT some amount of customer type YES or to exit type EXIT\n");

        char buffer[1024];
        char buffer1[1024];

        bzero(buffer, sizeof(buffer)); 
        int n = 0; 
        while((buffer[n++] = getchar()) != '\n'); 
        write(clientfd, buffer , strlen(buffer)); 

        if(strcmp(buffer,"EXIT\n")==0){
            printf("EXITING\n");
            close(clientfd);
            break;
        }

        bzero(buffer1, sizeof(buffer1)); 
        read(clientfd,buffer1, sizeof(buffer1)); 
        printf("%s",buffer1);

        if(strcmp(buffer,"YES\n")==0){
            bzero(buffer, sizeof(buffer)); 
            n = 0; 
            while((buffer[n++] = getchar()) != '\n'); 
            write(clientfd, buffer , strlen(buffer)); 

            if(strcmp(buffer,"EXIT\n")==0){
                printf("EXITING\n");
                close(clientfd);
                break;
            }else{

                bzero(buffer, sizeof(buffer)); 
                read(clientfd,buffer, sizeof(buffer)); 
                //printf("%s",buffer);

                if(strcmp(buffer,"0")==0){
                    printf("Not a valid CustomerID.\n");
                }else{
                    printf("Type CREDIT or DEBIT to credit or debit respectively \n to exit type EXIT\n");

                    bzero(buffer, sizeof(buffer)); 
                    n = 0; 
                    while((buffer[n++] = getchar()) != '\n'); 
                    write(clientfd, buffer , strlen(buffer)); 
                    
                    if(strcmp(buffer,"EXIT\n")==0){

                        printf("EXITING\n");
                        close(clientfd);
                        break;

                    }else if(strcmp(buffer,"CREDIT\n")==0){

                        printf("Type the amount to credit\n");

                        bzero(buffer, sizeof(buffer)); 
                        n = 0; 
                        while((buffer[n++] = getchar()) != '\n'); 
                        write(clientfd, buffer , strlen(buffer));

                        bzero(buffer, sizeof(buffer)); 
                        read(clientfd,buffer, sizeof(buffer)); 
                        printf("%s",buffer);

                    }else if(strcmp(buffer,"DEBIT\n")==0){

                        printf("Type the amount to debit\n");

                        bzero(buffer, sizeof(buffer)); 
                        n = 0; 
                        while((buffer[n++] = getchar()) != '\n'); 
                        write(clientfd, buffer , strlen(buffer));

                        bzero(buffer, sizeof(buffer)); 
                        read(clientfd,buffer, sizeof(buffer)); 
                        printf("%s",buffer);

                    }else{
                        printf("Not an valid input!! Pls Try again.\n");
                    }
                }
            }
        }

    }
    return;
}

void police(int clientfd){
    printf("To see Customer MINI Statement type CustomerID\n To exit Type EXIT\n");
    while(1){
        char buffer[1024];

        bzero(buffer, sizeof(buffer)); 
        int n = 0; 
        while((buffer[n++] = getchar()) != '\n'); 
        write(clientfd, buffer , strlen(buffer)); 

        if(strcmp(buffer,"EXIT\n")==0){
            printf("EXITING\n");
            close(clientfd);
            break;
        }

        bzero(buffer, sizeof(buffer)); 
        read(clientfd,buffer, sizeof(buffer)); 
        printf("%s",buffer);

    }
    return;
}

int main(int argc,char *argv[]){

    if(argc!=3){
        printf("Invalid arguments in command line \n");
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
        printf("Server not available \n"); 
        return -1; 
    }else{
        printf("Connection made \n");
    }

    //Displays Enter You Username
    char buffer[1024];
    int valread = read(clientfd, buffer, 1024); 
    printf("%s",buffer);

    //Username
    bzero(buffer, sizeof(buffer)); 
    int n = 0; 
    while((buffer[n++] = getchar()) != '\n'); 
    write(clientfd, buffer , strlen(buffer)); 

    //Displays Enter You Password
    bzero(buffer, sizeof(buffer)); 
    valread = read(clientfd, buffer, 1024); 
    printf("%s",buffer);

    //Password
    bzero(buffer, sizeof(buffer)); 
    n = 0; 
    while((buffer[n++] = getchar()) != '\n'); 
    write(clientfd, buffer , strlen(buffer)); 

    bzero(buffer, sizeof(buffer)); 
    valread = read(clientfd, buffer, 1024); 

    if(buffer[0] == '0'){
        printf("Invaild Credentials\nLogin Failed\n");
        close(clientfd);
        return -1;
    }

    printf("Login Successful\n");

    if(buffer[0] == '1'){
        printf("Welcome Customer\n");
        customer(clientfd);
    }else if(buffer[0] == '2'){
        printf("Welcome Admin\n");
        admin(clientfd);
    }else if(buffer[0] == '3'){
        printf("Welcome Police\n");
        police(clientfd);
    }

    return 0;
}