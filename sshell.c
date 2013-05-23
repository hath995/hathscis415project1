#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
/**

**/

#define MAX_ARG_LENGTH 1024
#define MAX_ARGS 100

const char PROMPT[] = "\nshredder# ";
int pid;

void writeprompt() {
	write(STDOUT_FILENO,PROMPT,sizeof(PROMPT));
}
void handler(int signum) {
	if(kill(pid,SIGKILL) <0) {
		char procsmote[] = "\nDrat!";
		write(STDOUT_FILENO,procsmote,sizeof(procsmote));
	}else{
		char procsmote[] = "\nYour father smelled of elderberries.";
		write(STDOUT_FILENO,procsmote,sizeof(procsmote));
	}
}
int mystrlen(char* astring) {
	int i=0;
	while(astring[i] != '\0') {
		i++;
	}
	i++;
	return i;
}

//based on function shown by daveti in lab section
void parsecommand(char* input, char** cmd,char**args) {
	char* placeholder = input;
	char* placeholderhead = input;
	int numberofparam = 0;
	while(*placeholder != '\0') {
		if(*placeholder != ' ' && *placeholder != '\n') {
			placeholder++;
		}else{
			*placeholder = '\0';
			args[numberofparam] = placeholderhead;
			numberofparam++;
			placeholder++;
			placeholderhead = placeholder;	
		}
	}	
	args[numberofparam] = placeholderhead;
	args[numberofparam+1] = NULL; 
	unsigned int size =mystrlen(args[0]); 
	//printf("size %u",size);
	*cmd = malloc(size*sizeof(char)); 
	//*cmd = args[0]; 
	if(*cmd != NULL)
	{
		int i;
		int k = 0;
		for(i = 0; i < size; i++) {
			if(args[0][i] != '\n') {
				(*cmd)[k] = args[0][i];
			}
			k++;
		}
	}
	
	args[numberofparam] = NULL;
}

int main(int argc, char **argv) {
	
	int waittime = -1;
	char inputbuffer[1024];
	char *newenviron[] = {NULL};
	char *newargv[MAX_ARGS];
	char *newcmd = NULL;
	if(argc > 1) {
		waittime = atoi(argv[1]); 
	}
	signal(SIGALRM,handler);
	writeprompt();
	while(1) {
		int numread = read(STDIN_FILENO,inputbuffer,1024);
		//test for reading in return
		//check for empty array/set null
		if(numread >= MAX_ARG_LENGTH) {
			perror("Length of arguments exceeed the limitation");
		}
		inputbuffer[numread] = '\0';
		parsecommand(inputbuffer,&newcmd,newargv);
		pid = fork();
		if(!pid) {
			execve(newcmd,newargv,newenviron); 
			perror("execve failed");
			_exit(0);
		}else if(pid <0) {
			perror("fork() failed");
			_exit(0);
		}else{
			if(waittime > 0) {
				alarm(waittime);
			}
			wait(pid);
			writeprompt();
		}
		free( newcmd);
	}
	
}
