// Created by Bhimesh Chauhan & YuWei Xi


#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <string.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <signal.h>

// Error Method
void error(char *msg)
{
    perror(msg);
    exit(0);
}

// Finish Data Transfer
void stop(int sig)
{
    puts("finish data transfer.");
    exit(0);
}

// Main Mthod Entry
int main(int argc, const char* argv[])
{
    
    // Check for arguments
    if (argc != 6)
    {
        fprintf(stderr, "usage: %s <Source IP> <Source Port> <Destination IP> <Destination Port> <Loss Rate>\n", argv[0]);
        exit(1);
    }
    
    // Server 1
    signal(SIGINT,stop);
    struct sockaddr_in serv1, serv2, client;
    bzero(&serv1,sizeof(serv1));
    
    // First Server : Use server to recieve data and transmit
    serv1.sin_family=AF_INET;
    serv1.sin_addr.s_addr=inet_addr(argv[1]);
    serv1.sin_port=htons(atoi(argv[2]));
    
    // Create Socket
    int fd1 = socket(AF_INET,SOCK_DGRAM,0);
    if(fd1 == -1)
    {
        error("Socket Creation Error");
        exit(1);
    }
    
    // Bind Socket
    if(bind(fd1,(struct sockaddr*)&serv1,sizeof(serv1))==-1)
    {
        error("ERROR Binding Server 1");
        exit(1);
    }
    
    // Server 2
    bzero(&serv2,sizeof(serv2));
    
    // Second Server :  Use server to send to vlc
    serv2.sin_family=AF_INET;
    serv2.sin_addr.s_addr=inet_addr(argv[3]);
    serv2.sin_port=htons(atoi(argv[4]));
    
    // VLC data (1316 bits)
    void *buff = malloc(1500);
    
    // Main loop for data transmission
    while(1)
    {
    
        // Variables
        int addr_len = sizeof(client);
        memset(buff,0,1500);
        
        // Recieve data from first server 1
        int ret = recvfrom(fd1,buff,1500,0,(struct sockaddr *)&client,&addr_len);
        
        // Debug
        printf("%5d ",ret);
        if(ret == -1){
           perror("Recieve Error");
           exit(1);
        }
        
        // If the drop is less than the random number
        if((rand()%101)<=atoi(argv[5]))
        {
            // continue
           continue;
        }
        // If the drop is more than the random number
        else
        {
            // Send the data to the second VLC
            ret = sendto(fd1,buff,ret,0,(struct sockaddr *)&serv2,sizeof(serv2));
            
            // Debug
            printf("%5d\n",ret);
            
            // Error detection
            if(ret == -1)
            {
                error("Sending Error");
                exit(1);
           }
        }
    }
    return 0;
}
