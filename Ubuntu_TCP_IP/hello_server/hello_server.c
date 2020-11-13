#include<stdio.h>
#include<stdlib.h>
#include <string.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/socket.h>
void error_handling(char *message);

int main(int argc, char *argv[])
{
    int serv_sock;
    int clnt_sock;

    struct sockaddr_in serv_addr;
    struct sockaddr_in clnt_addr;
    socklen_t clnt_addr_size;

    char message[] = "Hello world!";

    if(argc != 2)
    {
        printf("Usage : %s <port>\n", argv[0]);
        exit(1);   
    }

    serv_sock = socket(PF_INET, SOCK_STREAM, 0); // 서버 프로그램의 구현과정에서 제일 먼저 해야 할일이 소켓의 생성이다. 따라서 21행에서는 소켓을 생성하고 있다 단 이때 생성되는 소켓은 아직 서버소켓이라 부르기 어려운 상태다 
    if(serv_sock == -1)
        error_handling("socket() error");

    memset(&serv_addr, 0, sizeof(serv_addr)); // 소켓의 주소할당을 위해 구조체 변수를 초기화 하고 bind함수를 호출하고 있다. 
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));

    if(bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
        error_handling("bind() error");
    
    if(listen(serv_sock, 5) == -1) // 연결요청 대기상태로 들어가기 위해서 listen 함수를 호출하고 있다. 연결요청 대기큐의 크기도 5로 설정하고 있다. 이제야 비로소 socket 함수에서 생성한 소켓을 가리켜 서버소켓이라 할수있다. 
        error_handling("listen() error");

    clnt_addr_size = sizeof(clnt_addr);
    clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_addr_size); // accept 함수가 호출되었으니 대기큐에서 첫번째로 대기 중에 있는 연결 요청을 참조하여 클라이언트와의 연결을 구성하고 이때
    // 생성된 소켓의 파일 디스크립터를 반환한다. 참고로 이 함수가 호출되었을 때 대기큐가 비어있는 상태라면 대기큐가 찰때까지 다시 말해서 클라이언트의 연결요청이 들어올때까지 accept함수는 반환하지않는다.
    if(clnt_sock == -1)
        error_handling("accept() error");
    
    write(clnt_sock, message, sizeof(message)); // write 함수호출을 통해 클라이언트에 데이터를 전송하고 있다. 그리고는 close함수를 통해 연결을 끊고있다. 
    close(clnt_sock);
    close(serv_sock);
    
    return 0;
}

void error_handling(char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}