#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 100
void error_handling(const char *message);
void read_childproc(int sig);

int main(int argc, char *argv[])
{
    int serv_sock, clnt_sock;
    struct sockaddr_in serv_adr, clnt_adr;
    int fds[2];

    pid_t pid;
    struct sigaction act;
    socklen_t adr_sz;
    int str_len, state;
    char buf[BUF_SIZE];
    if( argc != 2 ){
        printf("Usage : %s <port>\n", argv[0]);
        exit(1);
    }

    act.sa_handler = read_childproc;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    state = sigaction(SIGCHLD, &act, 0);

    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_adr.sin_port = htons(atoi(argv[1]));

    if(bind(serv_sock, (struct sockaddr*) &serv_adr, sizeof(serv_adr)) == -1)
        error_handling("bind() error");
    if(listen(serv_sock, 5) == 1)
        error_handling("listen() error");

    // 아래 라인에서 파이프를 생성하고 fork라인에서는 파일의 데이터 저장을 담당할 프로세스를
    // 생성하고 있다. 
    pipe(fds);
    pid = fork();

    // fork에서 생성한 자식 프로세스에 의해 실행되는 영역이다. 이 영역에서는 파이프의 출구인
    // fds[0]으로 전달되는 데이터를 읽어서 파일에 저장하고 있다. 
    // 참고로 위의 서버는 종료되지 않고 클라이언트에게 계속해서 서비스를 제공하는 
    // 형태이기 때문에 파일에 데이터가 어느정도 채워지면 파일을 닫도록 for (int 10 ; i++)의 
    // 반복문을 구성하였다. 
    if(pid == 0)
    {
        FILE * fp = fopen("echomsg.txt", "wt");
        char msgbuf[BUF_SIZE];
        int i, len;

        for(i = 0; i< 10; i++)
        {
            len = read(fds[0], msgbuf, BUF_SIZE);
            fwrite((void*)msgbuf, 1, len, fp);
        }
        fclose(fp);
        return 0;
    }

    while(1)
    {
        adr_sz = sizeof(clnt_adr);
        clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_adr, &adr_sz);
        if(clnt_sock == -1)
        {
            continue;
        }
        else
        {
            puts("new client connected...");
        }

        pid = fork();
        if(pid == 0)
        {
            close(serv_sock);
            while((str_len = read(clnt_sock, buf, BUF_SIZE)) != 0)
            {
                write(clnt_sock, buf, str_len);
                // 84라인의 (바로위의 fork함수행)fork함수호출로 생성되는 모든 자식 프로세스는
                // 첫번째 pipe(fds);함수호출로 생성한 파이프의 파일 디스크립터를 복사한다. 때문에 파이프의 입구인 
                // fds[1]을 통해서 문자열 정보를 전달할 수 있다. 
                write(fds[1], buf, str_len);
            }

            close(clnt_sock);
            puts("client disconnected...");
            return 0;
        }
        else
        {
            close(clnt_sock);
        }
    }
    
    close(serv_sock);
    return 0;
    
}