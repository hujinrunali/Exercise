#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#define BUF_SIZE 30

void error_handling(char *message)
{
    fputs(message,stderr);
    fputc('\n',stderr);
    exit(1);
}

int main(int argc,char *argv[])
{
    int sock;
    struct sockaddr_in serv_addr,clnt_addr;
    socklen_t clnt_addr_len;
    char message[BUF_SIZE];
    int str_len;

    if(argc != 3)
    {
        printf("Usage: %s<IP><Port>\n",argv[0]);
        exit(1);
    }

    sock = socket(PF_INET,SOCK_DGRAM,0);
    if(sock == -1)
            error_handling("socket() error");

    memset(&serv_addr,0,sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(atoi(argv[2]));

    clnt_addr_len = sizeof(clnt_addr);
    while(1)
    {
        fputs("Please input the message(q to quit):",stdout);
        fgets(message,sizeof(message),stdin);

        sendto(sock,message,strlen(message),0,(struct sockaddr*)&serv_addr,sizeof(serv_addr));
        if(strcmp(message,"q\n") == 0)    
            break;
        str_len = recvfrom(sock,message,BUF_SIZE,0,(struct sockaddr*)&clnt_addr,&clnt_addr_len);
        message[str_len] = 0;
        printf("Message from server: %s",message);
    }
    close(sock);
    return 0;
}
