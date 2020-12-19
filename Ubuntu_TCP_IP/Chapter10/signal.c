#include <stdio.h>
#include <unistd.h>
#include <signal.h>


// 시그널이 발생했을 때 호출되어야 할 함수가 각각 정의되어 있다.
// 이러한 유형의 함수를 가리켜 시그널 핸들러(handler)라고 한다.
void timeout(int sig)
{
    if(sig == SIGALRM)
        puts("Time out!");

    // 2초 간격으로 SIGALRM 시그널을 반복 발생시키기 위해 시그널 핸들러 내에서 
    // alarm함수를 호출하고 있다.
    alarm(2);
}

// 시그널이 발생했을 때 호출되어야 할 함수가 각각 정의되어 있다.
// 이러한 유형의 함수를 가리켜 시그널 핸들러(handler)라고 한다.
void keycontrol(int sig)
{
    if(sig == SIGINT)
        puts("CTRL+C pressed");
}

int main(int argc, char *argv[])
{
    int i;

    // 시그널 SIGALRM, SIGINT에 대한 시그널 핸들러를 등록하고 있다. 
    signal(SIGALRM, timeout);
    signal(SIGINT, keycontrol);

    // 시그널 SIGALRM의 발생을 2초뒤로 예약하였다. 
    alarm(2);

    for(i = 0; i<3; i++)
    {
        puts("wait ...");

        // 시그널의 발생과 시그널 핸들러의 실행을 확인하기 위해서 100초간
        // 총 3회의 대기시간을 갖도록 반복문 내에서  sleep함수를 호출하고있다.
        // 그렇다면 총 300초, 대략 5분정도가 지나야 프로그램이 종료된다는 계산이 나오는데 
        // 이는 상당히 긴 시간이다. 하지만 실제 실행시간을 보면 채 10초가 걸리지 않는다.
        // 이유는 잠시후에 설명한다. 
        sleep(100);
    }

    return 0;
}