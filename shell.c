#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sysexits.h>
#include <unistd.h>
#include <stdbool.h>
#include<fileHandler.h>

char *getinput(char *buffer, size_t buflen) {
	printf("$$ ");
	return fgets(buffer, buflen, stdin);
}
bool isValidCmd(char *command) {
	return strcmp(command,"read") == 0 || strcmp(command,"write")==0 || strcmp(command,"append")==0;
}
/**
 * Check if the options and command are together,
 * if not, get the options
 */
char* getOptions(char *options){
	/**
	* Check if the length is greater than 1 if so then the options and redirection
	* command are together.
	*/
	if(strlen(options)>1){
		return options+1;
	}
	//Else the length is one then get the options from the input
	else {
		return strtok(NULL," \t");
	}
}

int main(int argc, char **argv) {
	char buf[1024],*command,*options;
	pid_t pid;
	int status;

	while (getinput(buf, sizeof(buf))) {
		buf[strlen(buf) - 1] = '\0';
		if(strcmp("exit",buf) == 0)
			exit(EX_OK);
		/*
		* I user will press just enter without any input, show an empty prompt.
		*/
		if(strlen(buf) == 1)
			continue;
		/*
		* Read the command
		*/
		command = strtok(buf," \t");

		if((pid=fork()) == -1) {
			fprintf(stderr, "shell: can't fork: %s\n", strerror(errno));
			continue;
		}
		else if (pid == 0) {
			while(command != NULL){
				/*
				*Validate command
				*/
				if(!isValidCmd(command)) {
					printf("Invalid command %s !!!\n", command);
					break;
				}

				options = strtok(NULL," \t");
				//Check for options
				if(options == NULL) {
					printf("arguments are needed for command %s !!!\n", command);
					continue;
				}

				printf("Command : %s\n", command);
				//Check command type
				if(strcmp(command,"read") == 0 && options[0] == '<') {
						simpleCat(getOptions(options),"temp.txt",false);
				}
				else if(strcmp(command,"write") == 0 && options[0] == '>') {
						simpleCat("temp.txt",getOptions(options),false);
				}
				else if(strcmp(command,"append") == 0 && options[0] == '>' && options[1] == '>') {
						simpleCat("temp.txt",getOptions(options+1),true);
				}
				else {
					printf("Invalid options!! : %s", options);
					break;
				}

				/**
				 *Check if the next command is pipe, then get he next command.
				 */
				command = strtok(NULL," \t");
				if(command!=NULL && command[0] == '|'){
					command = getOptions(command);
				}
			}
			exit(EX_OK);
		}

		if ((pid=waitpid(pid, &status, 0)) < 0)
			fprintf(stderr, "shell: waitpid error: %s\n",
					strerror(errno));
	}
	exit(EX_OK);
}
