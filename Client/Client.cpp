/*
    Create a TCP socket
*/
 
#include<stdio.h>
#include<winsock2.h>
#include <windows.h>
#include <iostream>

#include <string.h>
#include <stdio.h>
#pragma comment(lib,"ws2_32.lib") //Winsock Library
using namespace std;
 

int fileSEND(char *inputf,char *outf){    


  
    FILE * file_to_send;
    int ch;
    char toSEND[1];
    char remoteFILE[4096];
    int count1=1,count2=1, percent;

    
    WSADATA wsa;
    SOCKET s;
    struct sockaddr_in server;
    
    printf("\nInitialising Winsock...");
    if (WSAStartup(MAKEWORD(2,2),&wsa) != 0)
    {
        printf("Failed. Error Code : %d",WSAGetLastError());
        return 1;
    }
     
    printf("Initialised.\n");
     
    //Create a socket
    if((s = socket(AF_INET , SOCK_STREAM , 0 )) == INVALID_SOCKET)
    {
        printf("Could not create socket : %d" , WSAGetLastError());
    }
 
    printf("Socket created.\n");
     
     
    server.sin_addr.s_addr = inet_addr("192.168.2.10");
    server.sin_family = AF_INET;
    server.sin_port = htons( 8081);
 
    //Connect to remote server
    if (connect(s , (struct sockaddr *)&server , sizeof(server)) < 0)
    {
        puts("connect error");
        return 1;
    }
     
    puts("Connected");
     
	
		 
    file_to_send = fopen (inputf,"r");
      printf("opening file\n");
    if(!file_to_send) {
        printf("Error opening file\n");
        closesocket(s);
        return 0;
        } else {
    long fileSIZE;
    fseek (file_to_send, 0, SEEK_END);     fileSIZE =ftell (file_to_send);
    rewind(file_to_send);
 
    sprintf(remoteFILE,"FBEGIN:%s:%d\r\n", outf, fileSIZE);
    send(s, remoteFILE, sizeof(remoteFILE), 0);
    percent = fileSIZE / 100;
    
    
    while((ch=getc(file_to_send))!=EOF){
        toSEND[0] = ch;
        send(s, toSEND, 1, 0);
        if( count1 == count2 ) {
            printf("Filesize: %d Kb\n", fileSIZE / 1024);
            printf("Percent : %d%% ( %d Kb)\n",count1 / percent ,count1 / 1024);
            count1+=percent;
        }
        count2++;
    }
    }
    fclose(file_to_send);
    
    return 0;
    
		}
		  
int main()
{
	fileSEND("Hifumi.png","Hifumi2.png");

	return 0;
}

