#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <dirent.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <pthread.h>

#define PORT 34761

int main(int argc, char const *argv[]) 
{ 
    struct sockaddr_in address; 
    int sock = 0, valread; 
    struct sockaddr_in serv_addr; 
    char request[256];
    char *sendline; 
    char recline[256]; 
    char accountName[256];
    int i;
    int serve =0;
    //char buffer[1024] = {0}; 
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
    { 
        printf("\n Socket creation error \n"); 
        return -1; 
    } 
   
    memset(&serv_addr, '0', sizeof(serv_addr)); 
   
    serv_addr.sin_family = AF_INET; 
    serv_addr.sin_port = htons(PORT); 
      
    // Convert IPv4 and IPv6 addresses from text to binary form 
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0)  
    { 
        printf("\nInvalid address/ Address not supported \n"); 
        return -1; 
    } 
   
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) 
    { 
        printf("\nConnection Failed \n"); 
        return -1; 
    }
    while(1){
		bzero(sendline, 256);
		bzero(recline, 256);
		bzero(request, 256);
		
    	fgets(request, 256, stdin);
    	request[strlen(request)-1] = '\0';
    	if (strncmp(request, "quit", 4)==0){
    		sendline = "000Q";
    		write(sock,sendline,strlen(sendline)+1);
    		read(sock,recline,256);
    		printf("%s\n", recline);
    		break;
    	}
    	if (strncmp(request, "create", 6)==0){
    		if (serve==1){
    			printf("Cannot create account while serving another\n");
    		}else{
    			for (i=0;i<7;i++){
    				memmove(request, request+1, strlen(request));
    			}
    			char name[256];
    			strcpy(name, request);
    			char temp[256] = "C";
    			strcat(temp,"\"");
    			strcat(temp, name);
    			strcat(temp, "\"");
    			size_t len = strlen(temp)-1;
    			sprintf(sendline, "%03d", len);
    			//itoa(len, sendline, 256);
    			strcat(sendline, temp);
    			write(sock, sendline, strlen(sendline)+1);
    			read(sock, recline, 256);
    			printf("%s\n", recline);
    			
    		}
    	}
    	if (strncmp(request, "serve", 5)==0){
    		if (serve==1){
    			printf("Please end current service session before starting another\n");
    		}else{
    			serve =1;
    			for (i=0;i<6;i++){
    				memmove(request, request+1, strlen(request));
    			}
    			strcpy(accountName, request);
    			//accountName = request;
    		}
    		
    	}
    	if (strncmp(request, "deposit", 7)==0){
    		if (serve==0){
    			printf("Serve an account before this request\n");
    		}else{
    			char temp[256] = "+";
    			strcat(temp,"\"");
    			strcat(temp, accountName);
    			strcat(temp, "\"");
    			for (i=0;i<8;i++){
    				memmove(request, request+1, strlen(request));
    			}
    			strcat(temp,request);
    			size_t len = strlen(temp)-1;
    			sprintf(sendline, "%03d", len);
    			//itoa(len, sendline, 256);
    			strcat(sendline, temp);
    			write(sock, sendline, strlen(sendline)+1);
    			read(sock, recline, 256);
    			printf("%s\n", recline);
    		}
    		
    	}
    	if (strncmp(request, "withdraw", 8)==0){
    		if (serve==0){
    			printf("Serve an account before this request\n");
    		}else{
    			char temp[256] = "-";
    			strcat(temp,"\"");
    			strcat(temp, accountName);
    			strcat(temp, "\"");
    			for (i=0;i<9;i++){
    				memmove(request, request+1, strlen(request));
    			}
				strcat(temp,request);
    			size_t len = strlen(temp)-1;
    			sprintf(sendline, "%03d", len);
    			//itoa(len, sendline, 256);
    			strcat(sendline, temp);
    			write(sock, sendline, strlen(sendline)+1);
    			read(sock, recline, 256);
    			printf("%s\n", recline);
    		}
    	}
    	if (strncmp(request, "query", 5)==0){
    		if (serve==0){
    			printf("Serve an account before this request\n");
    		}else{
    			char temp[256] = "=";
    			strcat(temp,"\"");
    			strcat(temp, accountName);
    			strcat(temp, "\"");
    			size_t len = strlen(temp)-1;
    			sprintf(sendline, "%03d", len);
    			//itoa(len, sendline, 256);
    			strcat(sendline, temp);
    			write(sock, sendline, strlen(sendline)+1);
    			read(sock, recline, 256);
    			printf("%s\n", recline);
    		}
    	}
    	if (strncmp(request, "end", 3)==0){
    		if (serve==0){
    			printf("Can not end when not servicing\n");
    		}else{
    			serve=0;
    			bzero(accountName, 256);
    		}
    		
    	}
    
    }
     /*
    send(sock , hello , strlen(hello) , 0 ); 
    printf("Hello message sent\n"); 
    valread = read( sock , buffer, 1024); 
    printf("%s\n",buffer ); 
    */
    return 0; 
} 
