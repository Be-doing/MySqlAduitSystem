/*░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░
▩ File Name: OPProcess.h
▩ Author: top-down
▩ Description: 
▩ Created Time: 2020年05月27日 星期三 15时47分26秒
░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░*/

#include<iostream>
#include <unistd.h>
#include <sys/wait.h>
#include<stdio.h>
#include<stdlib.h>
using namespace std;
static struct pid {
	struct pid *next;
	FILE *fp;
	pid_t pid;
} *pidlist;
class OPProcess {
public:
	void RESET() {
		FILE *fd = popen("bash /home/top-down/workspace/MySqlAduitSystem/code/new_parse_dir/script/reset_process","r");
		if(fd == NULL) {
			cout << "reset_process faild" << endl;
			return;
		} 
		cout << "reset_process successful" << endl;
		mypclose(fd);
	}
	void KILL() {
		FILE *fd = popen("bash /home/top-down/workspace/MySqlAduitSystem/code/new_parse_dir/script/kill_process","r");
		if(fd == NULL) {
			cout << "kill_process faild" << endl;
			return;
		} 
		cout << "kill_process successful" << endl;
		mypclose(fd);
	}
	void  START() {
		FILE *fd = popen("bash /home/top-down/workspace/MySqlAduitSystem/code/new_parse_dir/script/start_process","r");
		if(fd == NULL) {
			cout << "start_process faild" << endl;
			return;
		} 
		mypclose(fd);
		cout << "start_process successful" << endl;
	}
private:
	int mypclose(FILE *iop,int flag = 0 /*加入控制标志，若为0，则调用waitpid直到返回；若为1,则立即返回*/)   
	{
		struct pid *cur, *last;
		int pstat;
		pid_t pid;
		/* Find the appropriate file pointer. */
		for (last = NULL, cur = pidlist; cur; last = cur, cur = cur->next)
			if (cur->fp == iop)
				break;
		if (cur == NULL)
			return (-1);
		(void)fclose(iop);
		if(0 == flag)
		{  //源代码方式
			do {
				pid = waitpid(cur->pid, &pstat, 0);
			} while (pid == -1 && errno == EINTR);
		}
		else  
		{ //增加方式
			if(pid != 0)
				kill(pid,SIGKILL);  //强制结束调用
		}
		/* Remove the entry from the linked list. */
		if (last == NULL)
			pidlist = cur->next;
		else
			last->next = cur->next;
		free(cur);
		return (pid == -1 ? -1 : pstat);
	}
};
