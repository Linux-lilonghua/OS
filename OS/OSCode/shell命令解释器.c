#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include<sys/types.h>

#define MAX_CMD_LEN 1024	//最大命令长度
#define MAX_ARG_NUM 64 		//最大参数命令
				//
void print_prompt(){
	printf(">>>");
}
int read_command(char* cmd_buf,char ** arg_bufs,int max_arg_num){
	int arg_num=0;
	fgets(cmd_buf,MAX_CMD_LEN,stdin);
	if(cmd_buf[strlen(cmd_buf)-1]=='\n'){
		cmd_buf[strlen(cmd_buf)-1]='\0';
	}
	char *token =strtok(cmd_buf," ");
	while(token!=NULL && arg_num<max_arg_num){
		arg_bufs[arg_num++]=token;
		token=strtok(NULL," ");
	}
	arg_bufs[arg_num]=NULL;
	return arg_num;
}
void execute_command(int arg_num,char ** args){
	pid_t pid =fork();
	if(pid==0){
		execvp(args[0],args);
		printf("Fail to execute command %s.\n",args[0]);
		exit(1);
	}else if(pid<0){
		printf("Fail to create child process.\n");
	}else{
		waitpid(pid,NULL,0);
	}
}

int cd(char *path) {
    if (path == NULL) {
        // 如果没有指定路径，则切换到用户的home目录
        path = getenv("HOME");
    }
    if (chdir(path) == -1) {
        // 切换目录失败
        perror("cd");
        return -1;
    }
    return 0;
}

void jobs(){
pid_t ppid=getpid();
printf("cpid=%d\n",ppid);
int i;
char **str;
for(i=0;i<3;i++){
str[i]=(char*)malloc(sizeof(char) * MAX_ARG_NUM);
}
pid_t pid=fork();
if(pid==0){
str[0]="pstree";
str[1]="-p";
sprintf(str[2],"%d",ppid);
str[3]=NULL;
execvp("pstree",str);
}else if(pid<0){
printf("Fail to execute command %s.\n",str[0]);
}else{
waitpid(pid,NULL,0);
}
}

int main(){
	char cmd_buf[MAX_CMD_LEN];	//命令缓冲区
	char* arg_bufs[MAX_ARG_NUM+1];	//参数缓冲区，多留一位作为结束标志
	while(1){
		print_prompt();
		int arg_num=read_command(cmd_buf,arg_bufs,MAX_ARG_NUM);
	if(arg_num==0){
		continue;
	}
	if(strcmp(arg_bufs[0],"cd")==0){
	     cd(arg_bufs[1]);
	     continue;
	}
	if(strcmp(arg_bufs[0],"jobs")==0){
	jobs();
       continue;	
	}
	if(strcmp(arg_bufs[0],"exit")==0){
		break;
	}
	 execute_command(arg_num,arg_bufs);
	}

return 0;
}

