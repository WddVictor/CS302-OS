#include <stdio.h>
#include <signal.h>
#include <stddef.h>
#include <sys/wait.h>
#include <sys/ioctl.h>
#include <sys/termios.h>

int main()
{
	int cpid;/* 保存子进程的id号 */
	int ppid;/* 保存父进程的id号 */
	char *prog1_argv[2];//启动进程的时候的参数
	char buf[256];
  
	ppid=getpid();//得到近程号
	cpid=fork();
  
	if (cpid<0)
		exit(-1);

    //子进程进入
	if (!cpid)
	{
		fprintf(stdout,"ID(child)=%d\n",getpid());

		/* 使子进程所在的进程组成为前台进程组，然后执行vi */
		
		setpgid(ppid,0);
		tcsetpgrp(0,getpid());

		prog1_argv[0]="vi";/* 命令ls的参y 数表 */
		prog1_argv[1]=NULL;

		execvp(prog1_argv[0], prog1_argv);//调用ls

		exit(-1);
	}
   
	//父进程等待终端输入，然后回显
	fprintf(stdout,"ID(parent)=%d\n",ppid);

	// setpgid(cpid,cpid);
	tcsetpgrp(0,cpid);/* 设置控制终端为子进程拥有 */
	waitpid(cpid,NULL,0); /*父进程等待子进程执行完毕，所在进程组成为前台进程组*/
	tcsetpgrp(0,ppid);/* 设置控制终端为子进程拥有 */


	while(1)
	{
		memset(buf, 0, 256);
		fgets(buf, 256, stdin);
		puts("ECHO: ");
		puts(buf);
		puts("\n");
	}

	return 0;
}
