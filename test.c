#include<stdio.h>
int add(int,int);
int sub(int,int);
int main()
{
	printf("hello\n");
	printf("%d+%d=%d\n",10,20,add(10,20));
	printf("%d-%d=%d\n",10,20,sub(10,20));

	return 0;
}
