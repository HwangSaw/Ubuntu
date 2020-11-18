#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/socket.h>

#define BUF_SIZE 1024
#define INT_SIZE 4
#define CHAR_SIZE 1

//  - 클라이언트는 서버에 접속하자마자 피연산자의 개수정보를 
// 1바이트 정수형태로 전달한다
// - 클라이언트가 서버에 전달하는 정수하나는 4바이트로 표현한다.
// - 정수를 전달한 다음에는 연산의 종류를 전달한다. 연산정보는 1바이트로 전달한다
// - 문자 +, - ,* 중 하나를 선택해서 전달한다.
// - 서버는 연산결과를 4바이트 정수의 형태로 클라이언트에 전달한다
// - 연산결과르 ㄹ얻은 클라이언트는 서버와의 연결을 종료한다. 
void error_handling(const char *message);
void ReadUntillFullData(int sock, char* pointRef, int byteSize);

int main(int argc, char* argv[])
{
    int sock;
    char message[BUF_SIZE];
    int  recv_len, recv_cnt;
    struct sockaddr_in serv_adr;
    int getServerResult;
    

    if(argc != 3)
    {
        printf("Usage :%s <IP> <port> \n", argv[0]);
        exit(1);
    }

    sock = socket(PF_INET, SOCK_STREAM, 0);
    if(sock == -1)
        error_handling("socket() error");

    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_adr.sin_port = htons(atoi(argv[2]));

    if(connect(sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1)
        error_handling("connect() error");
    else
        puts("Connected.........................");
    
    puts("Operand count:");
    char count;
    scanf("%c", &count);
    count = atoi(&count);
    write(sock, &count, sizeof(count));
    printf("input num : %c %d ", count, count);

    for(int i = 0; i<count; i++)
    {
        printf("Operand %d :", i+1);
        int number = 11;
        scanf("%d",&number);
        printf("test %d  \n", number);
        write(sock, (char*)&number, sizeof(number));        

        printf(" i : %d , count :%d \n",i, count);
    }

    printf("Operator: ");
    char operator1;
    scanf("%c", &operator1);
    write(sock, &operator1, sizeof(operator1));

    recv_len = 0;
    int getServerValue;
    ReadUntillFullData(sock, (char*)&getServerValue, sizeof(int));

    printf("Message from server: %d ", getServerValue);
    close(sock);

    return 0;
}

void ReadUntillFullData(int sock, char* pointRef, int byteSize)
{
    int recv_len = 0;
    int recv_cnt = 0;
    while(recv_len < byteSize)
    {
        recv_cnt = read(sock, pointRef +recv_len, byteSize);
        if(recv_cnt == -1)
            error_handling("read() error");
        recv_len += recv_cnt;
    }
}
// 해당 바이트 만큼 주소값에 데이터 read 하는 함수

void error_handling(const char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}