
#include <iostream>
#include <winsock2.h>
using namespace std;
 
int parseARGS(char **args, char *line){
int tmp=0;
args[tmp] = strtok( line, ":" );
while ( (args[++tmp] = strtok(NULL, ":" ) ) != NULL );
return tmp - 1;
}
 
int main()
{
   
    char *header[4096];
    char recvBUFF[4096];
    char *filename, *filesize;
    FILE * recvFILE;
    int received = 0;
    char tempstr[4096];
 
    int count1=1,count2=1, percent;
 
    WSADATA WSAData;
 
    SOCKET server, client;
 
    SOCKADDR_IN serverAddr, clientAddr;
 
    WSAStartup(MAKEWORD(2,0), &WSAData);
    server = socket(AF_INET, SOCK_STREAM, 0);
 
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8081);
 
    bind(server, (SOCKADDR *)&serverAddr, sizeof(serverAddr));
	listen(server, 5);	
    cout << "Listening for incoming connections..." << endl;
    
 

    int clientAddrSize = sizeof(clientAddr);
    
    client = accept(server, (SOCKADDR *)&clientAddr, &clientAddrSize);
     if (client < 0) {
        printf("Cannot accept connection\n");
       // close(listenSOCKET);
        return 1;
    }
  
    
    while(1){   

        if( recv(client, recvBUFF, sizeof(recvBUFF), 0) ){
            if(!strncmp(recvBUFF,"FBEGIN",6)) {
                recvBUFF[strlen(recvBUFF) - 2] = 0;
                parseARGS(header, recvBUFF);
                filename = header[1];
                filesize = header[2];
            }
            recvBUFF[0] = 0;
            recvFILE = fopen ( filename,"w" );
            percent = atoi( filesize ) / 100;
            while(1){
                if( recv(client, recvBUFF, 1, 0) != 0 ) {
                    fwrite (recvBUFF , sizeof(recvBUFF[0]) , 1 , recvFILE );
 
                    if( count1 == count2 ) {
                        printf("33[0;0H"); //move cursor to 0, 0
                        printf( "\33[2J"); //clear line
                        printf("Filename: %s\n", filename);
                        printf("Filesize: %d Kb\n", atoi(filesize) / 1024);
                        printf("Percent : %d%% ( %d Kb)\n",count1 / percent ,count1 / 1024);
                        count1+=percent;
                    }
                    count2++;
                    received++;
                    recvBUFF[0] = 0;
                } else {
                	
                return 0;
            }
            }
        } else {
        printf("Client dropped connection\n");
        }
 
    return 0;
    }

}

