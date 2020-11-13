#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
void Test(char *addr, int port, int T)
{
    int sockfd,numbytes;
    char buf[BUFSIZ];
    struct sockaddr_in their_addr;
    while((sockfd = socket(AF_INET,SOCK_STREAM,0)) == -1);
    their_addr.sin_family = AF_INET;
    their_addr.sin_port = htons(port);
    their_addr.sin_addr.s_addr=inet_addr(addr);
    bzero(&(their_addr.sin_zero), 8);
    
    while(connect(sockfd,(struct sockaddr*)&their_addr,sizeof(struct sockaddr)) == -1);
    char str[] = "HelloWorld\n";
sprintf(buf, "%d", T);
	numbytes = send(sockfd, buf, strlen(buf), 0);
    while(T--)
    {
        numbytes = recv(sockfd, buf, BUFSIZ, 0);
	numbytes = send(sockfd, str, strlen(str), 0);
        buf[numbytes]='\0';  
    }
	//send(sockfd, str, strlen(str), 0);
    close(sockfd);
   // return 0;
}

int main(int argc, char** argv)
{
	int port = atoi(argv[2]);
	while(1)
{
	int T = rand();
	Test(argv[1], port, T);
}
	return 0;
}
