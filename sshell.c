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

void printarray(char**stringar) {
	int arlen = sizeof(stringar)/sizeof(char*);
	int i;
	printf("[");
	for(i = 0; i < arlen; i++) {
		//write(STDOUT_FILENO,stringar[i],sizeof(stringar[i]));
		printf("%s,",stringar[i]);
	}
	printf("]\n");
}

void contain(char*test) {
	int len = mystrlen(test);
	int i = 0;
	for(i=0; i < len; i++) {
		if(test[i] == '\n') {
			printf("New Line found");
		}
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
	
	//strip path from command
	
	int lastslash = NULL;
	int j;
	for(j =0; j<size; j++)
	{
		if(args[0][j] == '/') {
			lastslash = j;
		}
	}
	char* simplecommand = malloc(size-lastslash);
	for(j =0; j < size-lastslash-1; j++)
	{
		simplecommand[j] = args[0][lastslash+j+1];
	}
	args[0] = simplecommand;
	
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
	//alarm(1);
	//while(1);
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
		contain(newcmd);
		contain(newargv[0]);
		pid = fork();
		if(!pid) {
			//printarray(newargv);
			//printf("\ncommand: %s\n",newcmd);
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
	}
	
}
