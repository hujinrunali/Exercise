#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#define BUF_SIZE 1024

void error_handling(char *message)
{
    fputs(message,stderr);
    fputc('\n',stderr);
    exit(1);
}

int main(int argc,char *argv[])
{
    int sock;
    struct sockaddr_in clnt_addr;
    char message[BUF_SIZE];
    int str_len;

    if(argc != 4)
    {
        printf("Usage: %s <IP><PORT><FileName>\n",argv[0]);
        exit(1);
    }

    sock = socket(PF_INET,SOCK_STREAM,0);
    if(sock == -1)
            error_handling("socket() error");

    memset(&clnt_addr,0,sizeof(clnt_addr));
    clnt_addr.sin_family = AF_INET;
    clnt_addr.sin_addr.s_addr = inet_addr(argv[1]);
    clnt_addr.sin_port = htons(atoi(argv[2]));

    if(connect(sock,(struct sockaddr*)&clnt_addr,sizeof(clnt_addr)) == -1)
        error_handling("connect() error");

    write(sock,argv[3],strlen(argv[3]));
    while((str_len = read(sock,message,BUF_SIZE-1)) != 0)
    {
        message[str_len] = 0;
        printf("%s",message);
    }
    close(sock);
    return 0;
}
