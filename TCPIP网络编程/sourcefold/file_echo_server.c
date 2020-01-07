#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <fcntl.h>
#define BUF_SIZE 1024
void error_handling(char*message)
{
    fputs(message,stderr);
    fputc('\n',stderr);
    exit(1);
}

int main(int argc,char *argv[])
{
    int serv_sock,clnt_sock,file_fd;
    struct sockaddr_in serv_addr,clnt_addr;
    int i,read_len;
    socklen_t clnt_addr_len;
    
    char filename[BUF_SIZE];
    char message[BUF_SIZE];

    if(argc != 2)
    {
        printf("Usage: %s<Port>\n",argv[0]);
        exit(1);
    }

    serv_sock = socket(PF_INET,SOCK_STREAM,0);
    if(serv_sock == -1)
            error_handling("socket() error");

    memset(&serv_addr,0,sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));

    if(bind(serv_sock,(struct sockaddr*)&serv_addr,sizeof(serv_addr)) == -1)
            error_handling("bind() error");

    if(listen(serv_sock,5) == -1)
            error_handling("listen() error");

    clnt_addr_len = sizeof(clnt_addr);
    for(i = 0;i<5;++i)
    {
        clnt_sock = accept(serv_sock,(struct sockaddr*)&clnt_addr,&clnt_addr_len);
        if(clnt_sock == -1)
                error_handling("accept() error");
        else
                printf("Client connect : %d",i+1);
        
        read_len = read(clnt_sock,filename,BUF_SIZE-1);
        filename[read_len] = 0;

        //打开文件，进行读操作
        file_fd = open(filename,O_RDONLY);
        if(file_fd == -1)
        {
            fputs("file open error\n",stdout);
            close(clnt_sock);
            break;     
        }

        while((read_len = read(file_fd,message,BUF_SIZE-1)))
                write(clnt_sock,message,read_len);
        close(clnt_sock);
        
    }
    close(serv_sock);
    return 0;
}
