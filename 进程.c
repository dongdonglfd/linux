#include<stdio.h>
#include<unistd.h>
#include<pthread.h>
int main()
{
    printf("===============\n");
    pid_t pid=fork();
    if(pid==0)//子进程
    {
        printf("child,child id=%d,parent id=%d\n",getpid(),getppid());
    }
    else if(pid>0)//父进程
    {
        printf("parent,child id=%d,parent id =%d,ppid=%d\n",pid,getpid(),getppid());
        sleep(1);
    }
    printf("=======\n");
    return 0;
}