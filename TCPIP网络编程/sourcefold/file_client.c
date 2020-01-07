#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#define BUF_SIZE 30

int main(int argc,char *argv[])
{
    int sock;
    int read_cnt;
    FILE *fp;
    char buf[BUF_SIZE];

    struct sockaddr_in serv_addr;
    
    fp = fopen("receive.dat","wb");

    sock = socket(PF_INET,SOCK_STREAM,0);

    memset(&serv_addr,0,sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(atoi(argv[2]));

    connect(sock,(struct sockaddr*)&serv_addr,sizeof(serv_addr));

    while((read_cnt = read(sock,buf,BUF_SIZE)) != 0)
            fwrite((void*)buf,1,read_cnt,fp);

    puts("Received file data");
    write(sock,"Thank you",10);
    close(sock);
    fclose(fp);
    return 0;
}
