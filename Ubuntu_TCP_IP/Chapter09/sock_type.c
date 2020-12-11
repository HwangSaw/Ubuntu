#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
void error_handling(const char* message);

int main(int argc, char * argv[])
{
    int tcp_sock, udp_sock;
    int sock_type;
    socklen_t optlen;
    int state;

    optlen = sizeof(sock_type);
    tcp_sock = socket(PF_INET, SOCK_STREAM, 0); // TCP소켓을 생성한다. 
    udp_sock = socket(PF_INET, SOCK_DGRAM, 0); // UDP 소켓을 생성한다.

    // TCP소켓 생성시 인자로 전달하는 SOCK_STREAM 상수값을 출력한다.  
    printf("SOCK_STREAM: %d \n", SOCK_STREAM); 
    // UDP 소켓 생성시 인자로 전달되는 SOCK_DGRAM 상수값을 출력하고 있다. 
    printf("SOCK_DGRAM: %d \n",SOCK_DGRAM);

    // 소켓의 타입정보를 얻고 있다. TCP소켓이라면 SOCK_STREAM의 상수값인
    // 1을 얻게 될것이다. 
    state = getsockopt(tcp_sock, SOL_SOCKET, SO_TYPE, (void*)&sock_type, &optlen);
    if(state)
        error_handling("getsockopt() error!");
    
    printf("socket type one: %d \n",sock_type);

    // 소켓의 타입정보를 얻고있다. UDP 소켓이라면 SOCK_DGRAM의 상수값인 2를 얻게된다.
    state = getsockopt(udp_sock, SOL_SOCKET, SO_TYPE, (void*)&sock_type, &optlen);
    if(state)
        error_handling("getsockopt() error!");
    
    printf("Socket type two: %d \n", sock_type);
    return 0;
}

void error_handling(const char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}