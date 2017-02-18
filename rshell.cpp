#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <cstring.h>
#include <dirent.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <pwd.h>
#include <errno.h>
#include <grp.h>
#include <time.h>

#include <iostream>
#include <iomanip>
#include <map>
#include <vector>
#include <algorithm>

void exit(); // used to end shell
void parse(); // will parse the input
void print(); // will print user input
// excute commands
void execute(string x)
{
	int status;
	pid_t pid= fork();
	if (pid < 0){
		perror("Forking child process failed");
		exit();
	}
	else if (pid == 0) {
		if (execvp(*x, x) < 0) {
			perrror("Execute failed");
			exit();
		}
	}
	else {
		waitpit(pid,&status,0);
	}
}
			
			
int main(int argc, char *argv[])
{
	for (i = 0; i < argc; i++)
	{
  		//will get every input
		char[] input = argv[i];

		//check for ;
		if(strchr(input,';')-input!=NULL){
			int in = strchr(input,';')-input;
			input[in] = '\0';
		}
        	
		//
	}
}
        	
