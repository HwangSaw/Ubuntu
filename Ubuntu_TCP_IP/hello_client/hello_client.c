#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/socket.h>
void error_handling(char * message);

int main(int argc, char* argv[])
{
    int sock;
    struct sockaddr_in serv_addr;
    char message[30];
    int str_len;

    if(argc != 3)
    {
        printf("Usage : %s <IP> <port>\n", argv[0]);
        exit(1);
    }

    sock = socket(PF_INET, SOCK_STREAM, 0); // 서버 접속을 위한 소켓을 생성하고 있다. 이때 생성하는 것은 tcp소켓이여야한다( == SOCK_STREAM )
    if(sock == -1)
        error_handling("socket() error");

    memset(&serv_addr, 0, sizeof(serv_addr)); // 구조체 변수 serv_addr에 IP와 port정보를 초기화하고있다. 초기화되는값은 연결을 목적으로하는 서버소켓의 IP와 port정보이다. 
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(atoi(argv[2]));

    if( connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1) // connect 함수호출을 통해 서버로 연결요청을 하고있다.
        error_handling("connect() error");
    
    str_len = read(sock, message, sizeof(message) -1); // 연결요청을 성공한 후 서버로부터 전송되는 데이터를 수신하고 있다. 
    if(str_len == -1)
        error_handling("read() error!");
    
    printf("Message from server : %s \n", message);
    close(sock); // 데이터 수신 이후에 close 함수호출을 통해서 소켓을 닫고 있다. 따라서 서버와의 연결은 종료 
    
    return 0;
}

void error_handling(char *message)
{
    fputs(message, stderr);
    fputc('\n',stderr);
    exit(1);
}