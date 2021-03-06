#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/socket.h>

#define BUF_SIZE 1024
void error_handling(const char *message);
void ReadUntillFullData(int sock, char* pointRef, int byteSize);
int operatorFunc(int result, int second, char operatorChar);

int main(int argc, char* argv[])
{
    int serv_sock, clnt_sock;
    char message[BUF_SIZE];

    struct sockaddr_in serv_adr, clnt_adr;
    socklen_t clnt_adr_sz;

    if(argc != 2 ){
        printf("Usage : %s <port>\n", argv[0]);
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

    clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_adr, &clnt_adr_sz);
    if(clnt_sock == -1)
        error_handling("accept() error");
    else
        printf("connected client %d \n", 1);
    
    // 클라가준 인트값 먼저 한게 읽자
    char clientCount;  
    ReadUntillFullData(clnt_sock, &clientCount, sizeof(clientCount));
    
    printf("clientCharCountValue %d \n" , clientCount);

    int *intArray = (int*)malloc(sizeof(int) * clientCount);
    
    for(int i=0; i<clientCount; i++)
    {
        ReadUntillFullData(clnt_sock, (char*)&intArray[i], sizeof(int));
        printf("client get intValue: %d \n ", intArray[i]);
    }

    char operator1;
    ReadUntillFullData(clnt_sock, &operator1, sizeof(operator1));
    printf("clientGetOperator %c \n", operator1);

    int firstValue = intArray[0];
    for(int i=1; i<clientCount; i++)
    {
        firstValue = operatorFunc(firstValue, intArray[i], operator1);

    }

    int serverReturnValue = firstValue;
    write(clnt_sock, &serverReturnValue, sizeof(int));

    close(clnt_sock);
    close(serv_sock);

    return 0;
}

int operatorFunc(int result, int second, char operatorChar)
{
    if(operatorChar == '+')
    {
        return result + second;
    }else if(operatorChar == '-')
    {
        return result - second;
    }else if(operatorChar == '*')
    {
        return result * second;
    }else
    {
        error_handling("operatorChar is error");
        return 0;
    }
    
}

void ReadUntillFullData(int sock, char* pointRef, int byteSize)
{
    int recv_len = 0;
    int recv_cnt = 0;
    while(recv_len < byteSize)
    {
        recv_cnt = read(sock, pointRef + recv_len, byteSize);
        if(recv_cnt == -1)
            error_handling("read() error");
        recv_len += recv_cnt;
    }
}

void error_handling(const char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}