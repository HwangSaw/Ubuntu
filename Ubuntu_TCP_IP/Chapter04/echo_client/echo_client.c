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
    int sock;
    char message[BUF_SIZE];
    int str_len;
    struct sockaddr_in serv_adr;

    if(argc != 3){
        printf("Usage :%s <IP> <port> \n", argv[0]);
        exit(1);
    }

    sock = socket(PF_INET, SOCK_STREAM, 0);
    if( sock == -1)
        error_handling("socket() error");
    
    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr =  inet_addr(argv[1]);
    serv_adr.sin_port = htons(atoi(argv[2]));

    if(connect(sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1) // connect 함수가 호출되고 있다. 앞서 언급했듯이 이 함수호출로 인한 연결요청 정보가
    //서버의 대기큐에 등록이 되면 connect함수는 정상적으로 호출을 완료한다. 때문에 Connected .................가 호출되고 문자열정보가 출력되더라도 서버에서 accept함수를
    // 호출하지 않은 상황이라면 실제 서비스가 이뤄지지 않음에 주의해야한다. 
        error_handling("connect() error");
    else
        puts("Connected.............");
    

    while(1)
    {
        fputs("Input message(Q to quit): ", stdout);
        fgets(message, BUF_SIZE, stdin);

        if( !strcmp(message, "q\n") || !strcmp(message, "Q\n"))
            break;
        
        write(sock, message, strlen(message));
        str_len = read(sock, message, BUF_SIZE -1 );
        message[str_len] = 0;
        printf("Message from server: %s", message);
    }

    close(sock); //이렇게 close함수가 호출되면 상대 소켓으로는 
    // eof가 전송된다. 즉 eof는 연결의 끝을 의미한다
    
    return 0;
}

void error_handling(char *message)
{
    fputs(message, stderr);
    fputc('\n',stderr);
    exit(1);
}