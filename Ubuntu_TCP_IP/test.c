#include<stdio.h>
#include<stdlib.h>
int main(int argc, char *argv[])
{
    // launch.json 의 인자 argv 테스트  "args": ["9190"],
    int temp = 0;
    int temp2 = 1;
    printf("argv[0] %s \n", argv[0]);

    printf("argv[1] %s \n", argv[1]);
    printf("argv[2] %s \n", argv[2]);
    printf("temp + temp2 = %d \n", temp+temp2);

    return 0;
}
