#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 30
void error_handling(const char* message);
void read_childproc(int sig);

int main(int argc, char *argv[])
{
    int serv_sock, clnt_sock;
    struct sockaddr_in serv_adr, clnt_adr;

    pid_t pid;
    struct sigaction act;
    socklen_t adr_sz;
    int str_len, state;
    char buf[BUF_SIZE];
    if( argc != 2 ){
        printf("Usage : %s <port>\n", argv[0]);
        exit(1);
    }

    // 좀비 프로세스의 생성을 막기 위한 코드 구성이다.
    act.sa_handler = read_childproc;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    state = sigaction(SIGCHLD, &act, 0);
    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr =htonl(INADDR_ANY);
    serv_adr.sin_port = htons(atoi(argv[1]));

    if(bind(serv_sock, (struct sockaddr*) &serv_adr, sizeof(serv_adr)) == -1)
        error_handling("bind error!");
    
    if(listen(serv_sock, 5) == -1)
        error_handling("listen() error");
    
    while(1)
    {
        adr_sz = sizeof(clnt_adr);

        // 이 라인에서 accept 함수를 호출한 이후에 52행에서 fork함수를 호출하고있다.
        // 때문에 이 라인을 통해서 만들어진 소켓(클라이언트의 연결요청 수락과정에서 만들어진)
        // 의 파일 디스크립터를 부모 프로세스와 자식 프로세스가 동시에
        // 하나씩 갖게 된다. 
        clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_adr, &adr_sz);
        if(clnt_sock == -1)
            continue;
        else
            puts("new client connected ...");

        pid = fork();
        if( pid == -1)
        {
            close(clnt_sock);
            continue;
        }

        // 자식 프로세스에 의해 실행되는 영역이다. 이부분에 의해서 클라이언트에게 에코
        // 서비스가 제공된다. 그런데 아래 if문안 close함수호출로 위에서 만든 서버 소켓을
        // 닫고있다.이는 자식 프로세스로 서버 소켓의 파일 디스크립터까지 복사되기 때문인데,
        // 이와 관련해서 잠시후에 별도로 설명한다. 
        if(pid == 0) /* 자식 프로세스 */
        {
            close(serv_sock);
            while((str_len = read(clnt_sock, buf, BUF_SIZE)) != 0)
                write(clnt_sock, buf, str_len);

            close(clnt_sock);
            puts("client disconnected...");
            return 0;
        }
        else
            close(clnt_sock); // accept함수호출을 통해서 만들어진 소켓의 파일 디스크립터가 자식
            // 프로세스에게 복사되었으니 서버는 자신이 소유하고있는 파일 디스크립터를 
            // 소멸시켜야 한다. 이와 관련해서도 잠시 후에 별도로 설명한다. 
    }
    close(serv_sock);
     
    return 0;
}

void read_childproc(int sig)
{
    pid_t pid;
    int status;
    pid = waitpid(-1, &status, WNOHANG);
    printf("removed proc id : %d \n", pid);
}

void error_handling(const char* message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}