#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char * argv[])
{
    int status;
    pid_t pid = fork(); // 여기서 생성된 자식 프로세스는
    //  아래의 if pid == 0 안의 return 3행에서 보이듯이 main함수 내에서의 
    // return 문 실행을 통해서 종료하게 된다. 

    if(pid == 0)
    {
        return 3;
    }
    else
    {
        printf("Child PID: %d \n", pid);
        pid = fork(); // 여기서 실행된 자식 프로세스는 아래 if문 안 exit(7)에서 종료하게된다.
        if(pid == 0)
        {
            exit(7);
        }
        else
        {
            printf("Child PID: %d \n", pid);
            wait(&status); //wait 함수를 호출하고있다 이로인해서 종료된 프로세스 관련정보는 
            //status에 담기게되고 해당 정보의 프로세스는 완전히 소멸된다. 

            // 이 매크로함수 WIFEXITED를통해서자식프로세스의 정상종료 여부를
            // 확인하고 있다.  정상종료인 경우에 한해서 WEXITSTATUS함수를호출하여
            //자식프로세스가 전달한값을 출력하고있다. 
            if(WIFEXITED(status))             
                printf("Child send one: %d \n", WEXITSTATUS(status));
            
            // 앞서 생성한 자식 프로세스가 두 개이므로 또한번의 wait함수호출과
            // 매크로함수의 호출을 진행하고있다.
            wait(&status);
            if(WIFEXITED(status))
                printf("Child send two :%d \n", WEXITSTATUS(status));
            
            // 모든 프로세스의 종료를 멈추기 위해서 삽입한 코드이다 이 순간에 여러분은 자식 
            // 프로세스의 상태를 확인하면 된다. 
            sleep(30); // sleep 30 sec
        }
        
    }
    return 0;
}