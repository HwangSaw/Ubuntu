#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/select.h>
#define BUF_SIZE 30

int main(int argc, char argv[])
{
    fd_set reads, temps;
    int result, str_len;
    char buf[BUF_SIZE];
    struct timeval timeout;

    // 14 
    // 다소 복잡해 보이지만 아주 쉬운 예제다 일단 이 라인에서 fd_set형 변수를 
    // 초기화하고 아래에서 파일 디스크립터 0의 위치를 1로 설정해 주고 있다.
    // 즉 표준 입력에 변화가 있는지 관심을 두고 보겠다는 뜻이다.
    FD_ZERO(&reads);
    FD_SET(0, &reads); // 0 is standard input (console)

    // 18  이부분에서 주석처리된 코드를 보자 이는 select함수의 타임아웃 
    // 설정을 위한 코드이다.그런데 이위치에서 타임아웃을 설정하면 안된다. 왜냐하면 
    // select함수호출 후에는 구조체 timeval의 멤버tv_sec와 tv_usec에 저장된 값이
    //  타임아웃이 발생하기까지 남았던 시간으로 바뀌기 때문이다. 
    // 따라서 select 함수호출하기전에 매번 timeval구조체변수의 초기화를 반복해야한다.
    /*
    timeout.tv_sec = 5;
    timeout.tv_usec = 5000;
    */

    while(1)
    {
        // 24 라인
        // 미리 준비해둔 fd_set형 변수 reads의 내용을 변수 temps에 복사하고 있는데 
        // 여기에는 그럴만한 이유가 있다. 앞서 설명했듯이 select함수호출이 끝나면
        // 변화가 생긴 파일 디스크립터의 위치를 제외한 나머지 위치의 비트들은 0으로 초기화
        // 된다. 따라서 원본의 유지를 위해서는 이렇게 복사의 과정을 거쳐야 한다.
        // 이는 일반적인 select함수의 사용방법중 일부이니 반드시 기억하자 
        temps = reads;
        //이렇듯 timeval구조체 변수의 초기화 코드를 반복문 안에 삽입해서 
        // select함수가 호출되기 전에 매번 새롭게 값이 초기화 되도록 해야한다.
        timeout.tv_sec = 5;
        timeout.tv_usec = 0;
        //select함수를 호출하고있다. 콘솔로부터 입력된 데이터가 있다면0보다 
        // 큰수가 반환되며 입력된 데이터가 없어서 타임아웃이 발생하는 경우에는 
        // 0이 반환된다.
        result = select(1, &temps, 0, 0, &timeout);
        if(result == -1)
        {
            puts("select() error!");
            break;
        }
        else if(result == 0)
        {
            puts("Time-out!");
        }
        else
        {
            // select함수가 0보다 큰수를 반환했을 떄 실행되는 영역이다.
            // 변화를 보인 파일 디스크립터가 표준입력이 맞는지 확인하고 
            // 맞으면 표준입력으로부터 데이터를 읽어서 
            // 콘솔로 데이터를 출력하고 있다. 
            if(FD_ISSET(0, &temps))
            {
                str_len = read(0, buf, BUF_SIZE);
                buf[str_len] = 0;
                printf("message from console: %s", buf);
            }
        }
        
    }

    return 0;
}