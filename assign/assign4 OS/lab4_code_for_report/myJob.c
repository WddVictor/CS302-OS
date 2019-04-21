#include <stdio.h>
#include <time.h>

void delay(int sec)
{
	time_t start_time, cur_time; // 变量声明
	time(&start_time);
	do {

		time(&cur_time);
	} while((cur_time - start_time) < sec );
}

int main(int argc,char *argv[])
{
	// delay(3);
	// sleep(10);
	while(1);
	return 0;
}