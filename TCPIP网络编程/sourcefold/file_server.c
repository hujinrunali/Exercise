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
    int serv_sock,clnt_sock;
    FILE *fp;
    int read_cnt;
    char message[BUF_SIZE];
    
    struct sockaddr_in serv_addr,clnt_addr;
    socklen_t clnt_addr_len;

    //去掉错误处理
    serv_sock = socket(PF_INET,SOCK_STREAM,0);
    memset(&serv_addr,0,sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));

    bind(serv_sock,(struct sockaddr*)&serv_addr,sizeof(serv_addr));
    listen(serv_sock,5);

    clnt_addr_len = sizeof(clnt_addr);
    clnt_sock = accept(serv_sock,(struct sockaddr*)&clnt_addr,&clnt_addr_len);
    
    fp = fopen("../sourcefolder/file_server.c","rb");
    while(1)
    {
        read_cnt = fread((void*)message,1,BUF_SIZE,fp);
        if(read_cnt < BUF_SIZE)
        {
            write(clnt_sock,message,read_cnt);
            break;
        }
        write(clnt_sock,message,BUF_SIZE);
    }

    //进入半关闭状态
    shutdown(clnt_sock,SHUT_WR);
    read_cnt = read(clnt_sock,message,BUF_SIZE);
    message[read_cnt] = 0;
    printf("Message from Client: %s",message);
    
    fclose(fp);
    close(clnt_sock);
    close(serv_sock);
    return 0;
}

