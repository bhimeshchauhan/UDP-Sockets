#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>

#define BUFFER 1024

// Global Variables
int socketsd;
int socketcd;
int sourcePort;
unsigned int sourceLen;
int drop;
int destPort;
int destLen;
struct sockaddr_in sourceaddr;
struct sockaddr_in destaddr;
struct hostent *host;
char buf[BUFFER];
char *hostaddr;
int flag;
int msgsize;

// Server Methods
void s_socket();
void s_bind();
