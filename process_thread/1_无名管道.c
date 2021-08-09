#include <stdio.h>
#include <unistd.h>
#include <string.h>

#define N 64
int main(int argc, const char *argv[])
{
	pid_t pid;
	int fd[2];
	char buf[N] = "";
	if(pipe(fd) < 0){		/*创建无名管道*/
		perror("pipe error");
		return -1;
	}
	pid = fork();	/*创建两个线程*/
	if(pid < 0){
		perror("fork error");
		return -1;
	}
	else if(pid > 0){		/*父进程 作为通信管道的写端*/
		printf("father :%d",getpid());
		while(1){
			fgets(buf, N, stdin);		/*键盘输入数据*/
			buf[strlen(buf) - 1] = '\0';
			write(fd[1], buf, strlen(buf));			/*写入管道*/
			if(strncmp(buf, "quit", 4) == 0){		/*一定模式退出，写端退出，读端能够正常读取，直到管道数据读取完毕*/
				close(fd[1]);
			}
		}
	}
	else{		/*子进程 作为通信管道的读端*/
		printf("client :%d",getpid());
		int count = 0;
		while(1){
			sleep(5);
			printf("ing\r\n");
			read(fd[0], buf, N);	/*读取管道中的数据*/
			if(strncmp(buf, "quit", 4) == 0){
			
				break;
			}
			printf("read:%s\n", buf);
		}
	}
	return 0;
}


