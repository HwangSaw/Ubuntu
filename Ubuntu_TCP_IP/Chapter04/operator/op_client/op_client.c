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
    int str_len, recv_len, recv_cnt;
    struct sockaddr_in serv_adr;

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
    int count;
    scanf("%d", &count);

    // 강제로 1로한다 지금은 테스트라서 
    count = 1;
    for(int i = 0; i<count; i++)
    {
        printf("Operand %d :", i+1);
        int number = 11;
        fputs("숫자 테스트 : \n", stdout);
        scanf("%d",&number);
        printf("test %d ", number);

        str_len = write(sock, (char*)&number, sizeof(number));


        recv_len = 0;
        int getServerValue;
        while(recv_len < sizeof(int))
        {
            recv_cnt = read(sock, (char*)(&getServerValue+recv_len), sizeof(int));
            if(recv_cnt == -1)
                error_handling("read() error");
            recv_len += recv_cnt;

        }
        
        printf("Message from server: %d ", getServerValue);
        
    }
    close(sock);

/*
    recv_len = 0;
        while(recv_len < str_len)
        {
            recv_cnt = read(sock, &message[recv_len], BUF_SIZE -1);
            if(recv_cnt == -1)
                error_handling("read() error");

            recv_len += recv_cnt;
        }
    */


    return 0;
}

void error_handling(char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}