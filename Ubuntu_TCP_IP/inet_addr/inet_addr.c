#include<stdio.h>
#include<arpa/inet.h>

int main(int argc, char *argv[])
{
    char *addr1 = "1.2.3.4";
    char *addr2 = "1.2.3.256"; // 1바이트당 표현할 수 있는 최대 크기의 정수는 255이므로 
    // 이는 분명 잘못된 IP 주소이다. 이 잘못된 주소를 이용해서 inet_addr 함수의 
    // 오류 검출능력을 확인하고자 하였다. 

    // 아래 실행 결과를 통해서  아래의 함수호출은 정상적인 결과로 이어지는걸 확인할 수 있다. 
    //  Network ordered integer 0x4030201 출력된다. 
    unsigned long conv_addr = inet_addr(addr1);
    if( conv_addr == INADDR_NONE )
        printf("Error occured! \n");
    else 
        printf("Network ordered integer addr: %#lx \n", conv_addr);

    conv_addr = inet_addr(addr2);
    // 아래 실행결과를 통해서 아래의 함수호출은 정상적인 결과로 이어지지 않음을 확인할 수 있다.
    // Error occured! 출력된다. 
    if( conv_addr == INADDR_NONE )
        printf("Error occureded \n");
    else 
        printf("Network ordered integer addr: %#lx \n\n", conv_addr);
        
    return 0;
}