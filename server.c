//Header Files
#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h>
#include <stdio.h>  
#include <arpa/inet.h>
#include <time.h>

#define MAX_NO_OF_CLIENTS 100

char* getDate(){
    int day, month, year;
	time_t now;
	time(&now);
	struct tm *local = localtime(&now);
	day = local->tm_mday;			
	month = local->tm_mon + 1;   	
	year = local->tm_year + 1900;
    char sday[2], smonth[2], syear[4];
    sprintf(sday, "%d", day); 
    sprintf(smonth, "%d", month);
    sprintf(syear, "%d", year);
    char date[11] = {sday[0] , sday[1] , ':', smonth[0],smonth[1],':',syear[0],syear[1],syear[2],syear[3]};
    char *d=date;
    return d;
}

void sendText(int sock_cli,char* text){
    send(sock_cli,text,strlen(text),0);
    return;
}

int getBal(char *username,int sock_cli){
    char uName[200];
    int n=0;
    while(username[n]!='\n'){
        uName[n]=username[n];
        n++;
    }
    FILE* ptr = fopen(uName,"r"); 
    if(ptr==NULL){ 
        sendText(sock_cli,"Person does not exist\n"); 
        return -1; 
    } 
    char sBal[100];
    char *fBal;
    while(fscanf(ptr,"%*s %*s %s",sBal)==1){ 
        fBal = sBal;
    }
    printf(":%s:\n",fBal);
    int bal = atoi(fBal);
    return bal;
}

void showBal(int sock_cli,char *username,int bal){
    if(bal>=0){
        char balS[20];
        sprintf(balS,"%d",bal);
        char sen[20] = "Your Balance is: ";
        strcat(sen,balS);
        char c[2]="\n";
        strcat(sen,c);
        sendText(sock_cli,sen);
    }
}

void showMini(int sock_cli){
    sendText(sock_cli,"Mini Statement Here\n");
    return;
}

void customer(int sock_cli,char *username){
    while(1){
        char buffer[1024];
        bzero(buffer, sizeof(buffer)); 
        read(sock_cli,buffer, sizeof(buffer)); 
        printf("%s",buffer);

        if(strcmp(buffer,"EXIT\n")==0){
            close(sock_cli);
            printf("SERVER EXITING\n");
            return;
        }else if(strcmp(buffer,"BAL\n")==0){
            int bal = getBal(username,sock_cli);
            showBal(sock_cli,username,bal);
        }else if(strcmp(buffer,"MINI\n")==0){
            showMini(sock_cli);
        }else{
            sendText(sock_cli,"Not an valid input!! Pls Try again.\n");
        }
    }
}

void admin(int sock_cli){
    
    return ;
}

void police(int sock_cli){
    int x;
    return ;
}

void assign_user(int sock_cli,int auth_id,char *username){
    if(auth_id == 1){
        sendText(sock_cli,"1");
        customer(sock_cli,username);
    }else if(auth_id == 2){
        sendText(sock_cli,"2");
        admin(sock_cli);
    }else if(auth_id == 3){
        sendText(sock_cli,"3");
        police(sock_cli);
    }else{
        sendText(sock_cli,"0");
        return ;
    }
    return;
}

int auth_check(char *username,char *password){
    FILE* ptr = fopen("Login_file","r"); 
    if(ptr==NULL){ 
        printf("Login File Missing"); 
        return 0; 
    } 
    char uName[100];
    char pWord[100];
    char type[10]; 
    char n[2] = "\n";
    while(fscanf(ptr,"%s%s%s",uName,pWord,type)==3){ 
        strcat(uName,n);
        strcat(pWord,n);
        if(strcmp(username,uName)==0&&strcmp(pWord,password)==0){
            if(strcmp(type,"C")==0){
                return 1;
            }else if(strcmp(type,"A")==0){
                return 2;
            }else if(strcmp(type,"P")==0){
                return 3;
            }
        } 
    }
    return 0;
}

void start(int sock_cli){
    char username[1024];
    char password[1024];
    sendText(sock_cli,"Welcome to Bank :) \n");
    sendText(sock_cli,"Enter you Username:");
    // send(sock_cli, "Welcome to Bank :) \n" , strlen("Welcome to Bank :) \n") , 0 );
    // send(sock_cli, "Enter you Username:" , strlen("Enter you Username:") , 0 );

    bzero(username, sizeof(username)); 
    read(sock_cli,username, sizeof(username)); 
    printf("%s",username);

    sendText(sock_cli,"Enter you password:");
    // send(sock_cli, "Enter you password:" , strlen("Enter you password:") , 0 );

    bzero(password, sizeof(password)); 
    read(sock_cli,password, sizeof(password)); 
    printf("%s",password);


    int auth_id = auth_check(username,password);

    assign_user(sock_cli,auth_id,username);

    return;
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