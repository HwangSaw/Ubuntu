#include <stdio.h>
#include <unistd.h>

int gval = 10;
int main(int argc, char *argv[])
{
    pid_t pid;
    int lval = 20;
    gval++, lval+= 5;

    pid = fork(); // 자식 프로세스를 생성하고 있다. 따라서 부모프로세스의 pid에는 
    // 자식 프로세스의 pid가 저장되며 자식 프로세스의 pid에는 0이 저장된다. 

    // 자식 프로세스는 이 문장을 실행한다. 
    if(pid == 0) // if child process  
        gval += 2, lval += 2 ;
    else    // if Parent process // 부모 프로세스는 이 문장을 실행한다. 
        gval -= 2, lval -= 2;

    // 자식 프로세스는 이 문장을 실행한다. 
    if( pid == 0 )
        printf("Child Proc: [%d, %d] \n", gval, lval);
    else  // 부모 프로세스는 이 문장을 실행한다. 
        printf("Parent Proc: [%d %d] \n", gval, lval);


    return 0;
}