#include <stdio.h>
#include <unistd.h>
#define BUF_SIZE 30

int man(int argc, char *argv[])
{
	int fds1[2], fds2[2];
	char str1[] = "Who are you?";
	char str2[] = "Thank you for your message";
	char buf[BUF_SIZE];
	pid_t pid;
	
	// 두 개의 파이프를 생성하고 있다.
	pipe(fds1), pipe(fds2);
	pid = fork();
	if(pid == 0)
	{
		// 자식 프로세스에서 부모 프로세스로의 데이터 전송은 배열 fds1이 참조하는 
        // 파이프를 통해서 이뤄진다.
		write(fds1[1],str1, sizeof(str1));
		read(fds2[0], buf, BUF_SIZE);
		printf("Child proc output: %s \n", buf);
	}
	else
	{
        // 자식 프로세스에서 부모 프로세스로의 데이터 전송은 배열 fds1이 참조하는 
        // 파이프를 통해서 이뤄진다.
		read(fds1[0], buf, BUF_SIZE);
        // 부모 프로세스에서 자식 프로세스로의 데이터 전송은 배열 fds2가 참조하는 파이프를
        // 통해서 이뤄진다. 
		printf("Parent proc output: %s \n", buf);
		write(fds2[1], str2, sizeof(str2));
		// 큰 의미는 없다 다만 부모 프로세스의 종료를 지연시키기 위해 
        // 삽입되었다. 
        sleep(3);
	}
	return 0;
}