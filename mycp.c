#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<errno.h>
int main(int argc,char *argv[])
{
    //创建缓冲区
    char buf[1024]={0};
    int n=0;
    //打开源文件
    int fd1=open(argv[1],O_RDONLY);
    if (fd1 == -1) {
        perror("open src err");
        exit(1);//程序非正常退出
    }
    //打开目标文件
    int fd2=open(argv[2],O_RDWR|O_CREAT|O_TRUNC,0644);
    if (fd2 == -1) {
        perror("open dst err");
        exit(1);//程序非正常退出
    }
    //循环从源文件读数据，写到目标文件
    while(( n=read(fd1,buf,1024)))
    {
        if(n<0)
        {
            perror("read error");
            break;
        }
        write(fd2,buf,n);
    }
    close(fd1);
    close(fd2);
    return 0;
}