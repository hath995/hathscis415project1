#include <stdio.h>
#include <linux/kernel.h>
#include <sys/syscall.h>
#include <unistd.h>

int main()
{
	long int rtn = syscall(351);
	printf("System call sys_hello return %ld\n",rtn);
	return 0;
}
