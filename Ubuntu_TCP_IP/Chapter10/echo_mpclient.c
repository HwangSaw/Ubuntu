#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 30
void error_handling(const char *message);
void read_routine(int sock, char *buf);
void write_routine(int sock, char *buf);

int main(int argc, char *argv[])
{
    int sock;
    pid_t pid;
    char buf[BUF_SIZE];
    struct sockaddr_in serv_adr;
    if(argc != 3){
        printf("Usage : %s <IP> <port> \n", argv[0]);
        exit(1);
    }

    sock = socket(PF_INET, SOCK_STREAM, 0);
    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_adr.sin_port = htons(atoi(argv[2]));

    if( connect(sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == 1)
        error_handling("connect() error!");
    
    pid = fork();
    if( pid == 0) 
    {
        // 여기서 호출하는 write_Routine함수에는 데이터 출력에 관련된 코드만 존재한다.
        // 반면 else문에서 호출하는 read_Routine함수에는 데이터 입력에 관련된 코드만 존재한다. 
        // 이렇듯 입력출력루틴을 구분해서 각각의 함수로 정의하는것은
        // 구현의 편의를 가져다 준다.
        write_routine(sock, buf); 
    }
    else
    {
        read_routine(sock, buf);
    }

    close(sock);
    return 0;

}

void read_routine(int sock, char *buf)
{
    while(1)
    {
        int str_len = read(sock, buf, BUF_SIZE);
        if( str_len == 0 )
            return;
        
        buf[str_len] = 0;
        printf("Message from server: %s", buf);
    }
}

void write_routine(int sock, char *buf)
{
    while(1)
    {
        fgets(buf, BUF_SIZE, stdin);
        if( !strcmp(buf,"q\n") || !strcmp(buf, "Q\n"))
        {
            // 서버로의 EOF전달을 위해서shutdown함수가 호출되어싿. 
            // 물론 아래의 return 문 실행 이후에 main함수 끝부분의 close함수 호출을 통해
            // eof 의 전달을 기대할 수 잇지만 현재 fork함수호출을 통해서
            // 파일 디스크립터가 복사된 상황이다그리고 이러한 상황에서는 한번의 close함수호출로 
            // EOF의 전달을 기대할 수 없다 따라서 반드시shutdown함수호출을통해서
            // EOF의 전달을 별도로 명시해야한다.
            shutdown(sock, SHUT_WR);
            return;
        }
        write(sock, buf, strlen(buf));
    }
}

void error_handling(const char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}