#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/socket.h>

#define BUF_SIZE 1024
#define RLT_SIZE 4
#define OPSZ 4 // 피 연산자의 바이트 수와 연산결과의 바이트 수를 상수화 하였다.

void error_handling(const char* message);

int main(int argc, char *argv[])
{
    int sock;
    char opmsg[BUF_SIZE]; // 데이터의 송수신을 위한 메모리 공간은 이렇듯 배열을 기반으로 
    // 생성하는 것이 좋다. 데이터를 누적해서 송수신해야 하기 때문이다.
    int result, opnd_cnt, i;
    struct sockaddr_in serv_adr;
    if(argc != 3)
    {
        printf("Usage : %s <IP> <port> \n", argv[0]);
        exit(1);
    }

    sock = socket(PF_INET, SOCK_STREAM, 0);
    if( sock == -1)
        error_handling("socket() error");
    
    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_adr.sin_port  = htons(atoi(argv[2]));

    if(connect(sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1)
        error_handling("connect error");
    else
        puts("Connected.............");
    
    fputs("Operand count: ", stdout);
    scanf("%d", &opnd_cnt); // 프로그램 사용자로부터 피연산자의 개수정보를 입력 받은 후
    // 이를 배열 opmsg에 저장하고 있다. char형으로의 형변환은 "피연산자의 개수정보를 
    // 1바이트 정수형대로 전달한다" 라고 정의한 프로토콜에 근거한 형변환이다. 때문에 
    // 1바이트로 표현 불가능한 범위의 정수가 입력되면 안된다. 참고로 여기서는 부호 있는
    // 정수의 형태로 예제를 작성하였지만. 피연산자의 개수정보는 음수가 될 수 없으므로
    // 부호 없는 양의 정수 형태로 예제를 작성하는 것이 보다 합리적이다. 
    opmsg[0] = (char)opnd_cnt;

    for(i = 0; i<opnd_cnt; i++)
    {
        printf("Operand %d: ", i+ 1);
        scanf("%d", (int*)&opmsg[i*OPSZ + 1]);
    } // 프로그램 사용자로부터 정수를 입력받아서 배열 opmsg에 이어서 저장하고 있다. 
    // char형 배열에 4바이트 int 형 정수를 저장해야 하기 때문에 int 형 포인터로 형변환을
    // 하고있다. 만약에 이부분이 이해되지 않는다면 포인터에 대한 별도의 학습이 필요한 
    // 상태임을 인식하기 바란다.

    fgetc(stdin); // 아래에서 문자를 scanf로 입력받아야하는데 이에 앞서 버퍼에 남아있는 \n 
    // 문자의 삭제를 위해 fgetc 함수가 호출되고 있다. 
    fputs("Operator: ",stdout);
    scanf("%c", &opmsg[opnd_cnt*OPSZ + 1]); // 마지막으로 연산자 정보를 입력 받아서 
    // 배열 opmsg에 저장하고 있다. 
    write(sock, opmsg, opnd_cnt*OPSZ + 2); // 드디어 write 함수를 통해서 opmsg에 저장되어
    // 있는 연산과 관련된 정보를 한번에 전송하고 있다. 이렇듯 한번의 write함수호출을 통해서 
    // 묶어서 보내도 되고, 여러번의 write함수호출을 통해서 나눠서 보내도 된다. 여러차례
    // 강조했듯이 TCP는 데이터의 경계가 존재하지 않기 때문이다. 
    read(sock, &result, RLT_SIZE);  // 서버가 전송해주는 연산결과의 저장과정을 보이고
    // 있다. 수신할 데이터의 크기가 4바이트이기 때문에 이렇게 한번의 read함수호출로
    // 충분히 수신이 가능하다.

    printf("Operation result: %d \n", result);
    close(sock);
    return 0;
}

void error_handling(const char* message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}