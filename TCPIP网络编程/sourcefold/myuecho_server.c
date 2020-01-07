#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#define  BUF_SIZE 30

void error_handling(char*message)
{
    fputs(message,stderr);
    fputc('\n',stderr);
    exit(1);
}

int main(int argc,char *argv[])
{
    int sock;
    int str_len;
    char message[BUF_SIZE];

    struct sockaddr_in from_addr,self_addr;
    socklen_t from_addr_len;

    if(argc != 2)
    {
        printf("Usage:%s <port>\n",argv[1]);
        exit(1);
    }

    sock = socket(PF_INET,SOCK_DGRAM,0);
    if(sock == -1)
            error_handling("socket() error");

    memset(&self_addr,0,sizeof(self_addr));
    self_addr.sin_family = AF_INET;
    self_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    self_addr.sin_port = htons(atoi(argv[1]));

    if(bind(sock,(struct sockaddr*)&self_addr,sizeof(self_addr)) == -1)
            error_handling("bind() error");

    from_addr_len = sizeof(from_addr);
    while(1)
    {
        str_len = recvfrom(sock,message,BUF_SIZE,0,(struct sockaddr*)&from_addr,&from_addr_len);
        message[str_len] = 0;
        printf("Message from client: %s",message);
        
        fputs("Please input message(q to quit): ",stdout);
        fgets(message,sizeof(message),stdin);

        sendto(sock,message,strlen(message),0,(struct sockaddr*)&from_addr,from_addr_len);
        if(strcmp(message,"q\n") == 0)
                break;
    }
    close(sock);
    return 0;
}
