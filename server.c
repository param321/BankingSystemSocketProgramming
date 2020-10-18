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

char* purifyString(char *stg){
    char s[1024]={0};
    int n=0;
    while(stg[n]!='\n'){
        s[n]=stg[n];
        n++;
    }
    char *ss=s;
    return ss;
}

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
    char date[11] = {sday[0] , sday[1] , '/', smonth[0],smonth[1],'/',syear[0],syear[1],syear[2],syear[3]};
    char *d=date;
    return d;
}

void sendText(int sock_cli,char* text){
    send(sock_cli,text,strlen(text),0);
    return;
}

char* readText(int sock_cli){
    char buffer[1024];
    bzero(buffer, sizeof(buffer)); 
    read(sock_cli,buffer, sizeof(buffer)); 
    char *ans=buffer;
    return ans;
}

int getBal(char *username){

    char *uName = purifyString(username);

    FILE* ptr = fopen(uName,"r"); 
    if(ptr==NULL){ 
        return -1; 
    } 
    char sBal[100];
    char *fBal;
    while(fscanf(ptr,"%*s %*s %s",sBal)==1){ 
        fBal = sBal;
    }
    fclose(ptr);
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
    }else{
        sendText(sock_cli,"Person does not exist\n"); 
    }
}

void showMini(int sock_cli,char *username){
    char *uName = purifyString(username); 

    char mini[1024] = "MINI Statement Here:\n";
    char br[2] = "\n";
    char s[2] = " ";
    printf("%s",uName);
    FILE* ptr = fopen(uName,"r"); 
    if(ptr==NULL){ 
        sendText(sock_cli,"MINI does not exist\n"); 
        return; 
    } 
    char sDat[100],sCod[10],sBal[100];
    while(fscanf(ptr,"%s %s %s",sDat,sCod,sBal)==3){ 
        strcat(mini,sDat);
        strcat(mini,s);
        strcat(mini,sCod);
        strcat(mini,s);
        strcat(mini,sBal);
        strcat(mini,br);
    }
    fclose(ptr);
    sendText(sock_cli,mini);
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
            int bal = getBal(username);
            showBal(sock_cli,username,bal);
        }else if(strcmp(buffer,"MINI\n")==0){
            showMini(sock_cli,username);
        }else{
            sendText(sock_cli,"Not an valid input!! Pls Try again.\n");
        }
    }
}

int custORnot(char* username){
    char *uName = purifyString(username);
    FILE* ptr = fopen("Login_file","r"); 
    if(ptr==NULL){ 
        printf("Login File Missing"); 
        return 0; 
    } 
    char uname[100];
    char pWord[100];
    char type[10]; 
    char n[2] = "\n";
    while(fscanf(ptr,"%s%s%s",uname,pWord,type)==3){ 
        if(strcmp(uname,uName)==0){
            if(strcmp(type,"C")==0){
                return 1;
            }else if(strcmp(type,"A")==0){
                return 0;
            }else if(strcmp(type,"P")==0){
                return 0;
            }
        } 
    }
    return -1;
}

int isNumber(char *amount){
    int n=0;
    while(amount[n]!='\n'){
        if(amount[n]>='0'&&amount[n]<='9'){

        }else{
            return 0;
        }
        n++;
    }
    return 1;
}

void credit_amount(char *amount,char *username,int cur_bal){
    printf("11111");
    int am = atoi(amount);
    int new_bal = 0 + am;
    printf("22222");
    char *uName = purifyString(username);
    printf(":%s:",uName);
    char *date = getDate();
    printf("333333");
    FILE* ptr = fopen(uName,"a"); 
    if(ptr==NULL){ 
        printf("Noo :(((");
        return; 
    } 
    printf("Done :)");
    char *sbal;
    sprintf(sbal, "%d",new_bal);

    fprintf(ptr,"\n");
    fprintf(ptr,"%s",date);
    fprintf(ptr," ");
    fprintf(ptr,"%s","CREDIT");
    fprintf(ptr," ");
    fprintf(ptr,"%s",sbal);

    printf("Done :))");

    fclose(ptr);
    return;
}

void admin(int sock_cli){
    while(1){

        char buffer[1024];
        bzero(buffer, sizeof(buffer)); 
        read(sock_cli,buffer, sizeof(buffer)); 
        printf("%s",buffer);

        if(strcmp(buffer,"EXIT\n")==0){
            close(sock_cli);
            printf("SERVER EXITING\n");
            return;
        }else if(strcmp(buffer,"YES\n")==0){
            sendText(sock_cli,"Type the CustomerID to proceed or type EXIT to exit\n");

            bzero(buffer, sizeof(buffer)); 
            read(sock_cli,buffer, sizeof(buffer)); 
            printf("%s",buffer);

            char *username=readText(sock_cli);

            if(strcmp(buffer,"EXIT\n")==0){

                close(sock_cli);
                printf("SERVER EXITING\n");
                return;

            }else{
                int pre = custORnot(buffer);
                if(pre==1){

                    sendText(sock_cli,"1");

                    bzero(buffer, sizeof(buffer)); 
                    read(sock_cli,buffer, sizeof(buffer)); 
                    printf("%s",buffer);

                    if(strcmp(buffer,"EXIT\n")==0){

                        close(sock_cli);
                        printf("SERVER EXITING\n");
                        return;

                    }else if(strcmp(buffer,"CREDIT\n")==0){

                        bzero(buffer, sizeof(buffer)); 
                        read(sock_cli,buffer, sizeof(buffer)); 
                        printf("%s",buffer);
                        char *amount = purifyString(buffer);
                            // char uName[1024]={0};
                            // strcpy(uName,username);
                            // //int cur_bal = getBal(uName);
                            // printf(":%s:\n",username);

                            printf("11111");

                            int am = atoi(purifyString(amount));
                            int new_bal = 0 + am;
                            printf("22222");
                            char *uName = purifyString(username);
                            printf(":%s:",uName);
                            char *date = getDate();
                            printf("333333");
                            FILE* ptr = fopen(uName,"a"); 
                            if(ptr==NULL){ 
                                printf("Noo :(((");
                            } 
                            printf("Done :)");
                            char *sbal;
                            sprintf(sbal, "%d",new_bal);

                            fprintf(ptr,"\n%s %s %s",date,"CREDIT",sbal);
                            printf("Done :))");

                            fclose(ptr);


                            sendText(sock_cli,"Amount Sucessfully Credited\n");

                            bzero(buffer, sizeof(buffer)); 
                            read(sock_cli,buffer, sizeof(buffer)); 
                            printf("%s",buffer);
                            //credit_amount(amount,username,0);
                    }else if(strcmp(buffer,"DEBIT\n")==0){
                        bzero(buffer, sizeof(buffer)); 
                        read(sock_cli,buffer, sizeof(buffer)); 
                        printf("%s",buffer);

                        char *amount = purifyString(buffer);

                        if(isNumber(amount)){
                            sendText(sock_cli,"Amount Sucessfully Credited\n");
                        }else{
                            sendText(sock_cli,"The number was invalid!!\nThe Transaction Falied!!\n");
                        }
                    }else{
                        
                    }
                }else{
                    sendText(sock_cli,"0");
                }
            }
        }else{
            sendText(sock_cli,"Not an valid input!! Pls Try again.\n");
        }
    }
    return ;
}

void police(int sock_cli){
    while(1){
        char buffer[1024];
        bzero(buffer, sizeof(buffer)); 
        read(sock_cli,buffer, sizeof(buffer)); 
        printf("%s",buffer);

        if(strcmp(buffer,"EXIT\n")==0){
            close(sock_cli);
            printf("SERVER EXITING\n");
            return;
        }else{
            if(custORnot(buffer)==-1){
                sendText(sock_cli,"Not an valid CustomerID. Please try again\n");
            }else if(custORnot(buffer) == 0){
                sendText(sock_cli,"You have no access to view MINI Statement of Police or Admin. Please try again\n");
            }else if(custORnot(buffer) == 1){
                showMini(sock_cli,buffer);
            }
        }
    }
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
    sendText(sock_cli,"Welcome to Bank :) \nEnter you Username:");
    // sendText(sock_cli,"Enter you Username:");
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