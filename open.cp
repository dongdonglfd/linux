#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
int main()
{

      // int fd=open("./test.txt",O_RDONLY);
      int fd=open("./test.out",O_RDONLY|O_CREAT,0664);
	printf("%d\n",fd);
      int ret=close(fd);
      printf("%d\n",ret);
      return 0;
}


