#include<stdio.h>
#include<string.h>
#include<arpa/inet.h>

int main(int argc, char *argv[])
{
    struct sockaddr_in addr1, addr2;
    char *str_ptr;
    char str_arr[20];

    addr1.sin_addr.s_addr = htonl(0x1020304);
    addr2.sin_addr.s_addr = htonl(0x1010101);

    str_ptr = inet_ntoa(addr1.sin_addr); // 구조체 변수 addr1에 저장된 ip 정보를 전달하면서 inet_ntoa 함수를 호출하고 있다. 
    // 따라서 ip주소 정보를 담은 문자열의 주소값이 반환된다.

    strcpy(str_arr, str_ptr); // inet_ntoa 로 반환된 주소값을 참조해서 문자열을 복사하고 있다. 
    printf("Dotted-Decimal notation1: %s \n", str_ptr); // 점으로 구분된 십진수 표기 1 

    inet_ntoa(addr2.sin_addr); // inet_ntoa가 다시 한번 호출되었다. 이로써 str_ptr에는 다른 문자열 정보가
    // 채워진다. 
    printf("Dotted-Decimal notation2: %s \n",str_ptr);
    printf("Dotted-Decimal notation3: %s \n",str_arr); // str_arr 은 strcpy로 복사해뒀었다 그것 출력하면 첫번째 inet_ntoa 로 호출된 ip값 출력
    
    //Dotted-Decimal notation1: 1.2.3.4 
    // Dotted-Decimal notation2: 1.1.1.1 
    // Dotted-Decimal notation3: 1.2.3.4 

    return 0;
}