#include <stdio.h>
#include <signal.h>
#include <stddef.h>
#include <sys/wait.h>
#include <sys/ioctl.h>
#include <sys/termios.h>

int main()
{
	int cpid;/* �����ӽ��̵�id�� */
	int ppid;/* ���游���̵�id�� */
	char *prog1_argv[2];//�������̵�ʱ��Ĳ���
	char buf[256];
  
	ppid=getpid();//�õ����̺�
	cpid=fork();
  
	if (cpid<0)
		exit(-1);

    //�ӽ��̽���
	if (!cpid)
	{
		fprintf(stdout,"ID(child)=%d\n",getpid());

		/* ʹ�ӽ������ڵĽ������Ϊǰ̨�����飬Ȼ��ִ��vi */
		
		setpgid(ppid,0);
		tcsetpgrp(0,getpid());

		prog1_argv[0]="vi";/* ����ls�Ĳ�y ���� */
		prog1_argv[1]=NULL;

		execvp(prog1_argv[0], prog1_argv);//����ls

		exit(-1);
	}
   
	//�����̵ȴ��ն����룬Ȼ�����
	fprintf(stdout,"ID(parent)=%d\n",ppid);

	// setpgid(cpid,cpid);
	tcsetpgrp(0,cpid);/* ���ÿ����ն�Ϊ�ӽ���ӵ�� */
	waitpid(cpid,NULL,0); /*�����̵ȴ��ӽ���ִ����ϣ����ڽ������Ϊǰ̨������*/
	tcsetpgrp(0,ppid);/* ���ÿ����ն�Ϊ�ӽ���ӵ�� */


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
