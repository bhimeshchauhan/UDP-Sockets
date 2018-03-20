#include "udp_streamer.h"
/*
*   Created by Bhimesh Chauhan
*/

// error detection

void error(char *msg)
{
    perror(msg);
    exit(0);
}

void s_socket()
{

    socketsd = socket(AF_INET, SOCK_DGRAM, 0);
    if (socketsd < 0)
    {
        error("ERROR opening socket");
    }
    socketcd = socket(AF_INET, SOCK_DGRAM, 0);
    if (socketcd < 0)
    {
        error("ERROR opening socket");
    }
}

void s_bind()
{
    flag = 1;
    setsockopt(socketsd, SOL_SOCKET, SO_REUSEADDR,
         (const void *)&flag , sizeof(int));
    bzero((char *) &sourceaddr, sizeof(sourceaddr));
    sourceaddr.sin_family = AF_INET;
    sourceaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    sourceaddr.sin_port = htons((unsigned short)sourcePort);
    if (bind(socketsd, (struct sockaddr *) &sourceaddr, sizeof(sourceaddr)) < 0)
    {
        error("ERROR on binding");
    }
    
    setsockopt(socketcd, SOL_SOCKET, SO_REUSEADDR,
         (const void *)&flag , sizeof(int));
    bzero((char *) &destaddr, sizeof(destaddr));
    destaddr.sin_family = AF_INET;
    destaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    destaddr.sin_port = htons((unsigned short)sourcePort);
    if (bind(socketcd, (struct sockaddr *) &destaddr, sizeof(destaddr)) < 0)
    {
        error("ERROR on binding");
    }
    
}



int main(int argc, char **argv)
{
    //Declare drops
    drop = 0;
    if( rand()%100 < loss_rate )
         drop = 1;
    
    // Check Command Line input
    if (argc != 6)
    {
        fprintf(stderr, "usage: %s <Source IP> <Source Port> <Destination IP> <Destination Port> <Loss Rate>\n", argv[0]);
        exit(1);
    }
    
    sourcePort = atoi(argv[1]);
    destPort = atoi(argv[3]);
    
    // Server: Create Socket
    s_socket();
    // Server: Bind Socket
    s_bind();
    destLen = sizeof(destaddr);
    sourceLen = sizeof(sourceaddr);
    // Main Loop
    
    while(1){
        
        // Recieve from
        bzero(buf, BUFFER);
        msgsize = recvfrom(socketsd, buf, BUFFER, 0,
         (struct sockaddr *) &sourceaddr, &sourceLen);
        if (msgsize < 0)
            error("ERROR in recvfrom");
        
        
        // Get Address of the Host
        host = gethostbyaddr((const char *)&destaddr.sin_addr.s_addr,
              sizeof(destaddr.sin_addr.s_addr), AF_INET);
        if (host == NULL)
          error("ERROR on gethostbyaddr");
        hostaddr = inet_ntoa(destaddr.sin_addr);
        if (hostaddr == NULL)
          error("ERROR on inet_ntoa\n");
        printf("server received datagram from %s (%s)\n",
           host->h_name, hostaddr);
        printf("server received %lu/%d bytes: %s\n", strlen(buf), msgsize, buf);
        
        
        // Send to with Loss Rate
        if (!drop)
        {
            msgsize = sendto(socketsd, buf, strlen(buf), 0, (struct sockaddr *) &destaddr, destLen);
            if (msgsize < 0)
                error("ERROR in sendto");
        }
        else
        {
            printf("Dropping few data");
        }
        
    }
    return 0;
    
}
