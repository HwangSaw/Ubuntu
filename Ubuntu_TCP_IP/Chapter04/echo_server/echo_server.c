#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/socket.h>

#define BUF_SIZE 1024
void error_handling(char *message);

int main(int argc, char* argv[])
{
    int serv_sock, clnt_sock;
    char message[BUF_SIZE];
    int str_len, i;

    struct sockaddr_in serv_adr, clnt_adr;
    socklen_t clnt_adr_sz;

    if( argc != 2 ){
        printf("Usage : %s <port>\n ", argv[0]);
        exit(1);
    }

    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    if(serv_sock == -1)
        error_handling("socket() error");
    
    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_adr.sin_port = htons(atoi(argv[1]));

    if(bind(serv_sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1)
        error_handling("bind() error");
    
    if(listen(serv_sock, 5) == -1)
        error_handling("listen() error");

    clnt_adr_sz = sizeof(clnt_adr);

    // 총 5개의 클라이언트에게 서비스를 제공하기 위한 반복문이다. 결과적으로 accept 함수가 총 5회 호출되어 총 5개의 클라이언트에게 순서대로 에코 서비스를 제공한다.
    for(i = 0; i< 5; i++) 
    {
        clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_adr, &clnt_adr_sz);
        if(clnt_sock == -1)
            error_handling("accept() error");
        else
            printf("connected client %d \n", i+1);

        while((str_len = read(clnt_sock, message, BUF_SIZE)) != 0)  // 실제 에코 서비스가 이뤄지는 부분이다 읽어들인 문자열을 그대로 전송하고 있다. 
            write(clnt_sock, message, str_len);

        close(clnt_sock); //소켓을 대상으로 close함수가 호출되면 연결되어있던 상대방 소켓에게 EOF가 전달된다 
        // 즉 클라이언트 소켓이 close함수를 호출하면 while 문의 read함수호출의 결과값이 0이 되면서 반복문이 종료되고 
        // 서버에서도 close함수가 호출된다 
    }
    close(serv_sock);
    
    return 0;
}

void error_handling(char *message)
{
    fputs(message, stderr);
    fputc('\n',stderr);
    exit(1);
}