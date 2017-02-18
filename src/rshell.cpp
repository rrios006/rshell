#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <pwd.h>
#include <errno.h>
#include <sys/wait.h>

#include <cstdio>
#include <cstring>
#include <iostream>
#include <iomanip>
#include <map>
#include <vector>
#include <algorithm>

using namespace std;

void exit(); // used to end shell
// excute command
void execute(char *x[])
{
	int status;
	pid_t pid= fork();
	if (pid < 0){
		perror("Forking child process failed");
		exit(1);
	}
	else if (pid == 0) {
		if (execvp(x[0], x) < 0) {
			perror("Execute failed");
			exit(1);
		}
	}
	else {
		waitpid(pid,&status,0);
	}
}
			
			
int main(int argc, char *argv[])
{
	cout << "$ ";
	for (int i = 0; i < argc; i++)
	{
  		//will get every input
		char *input;
		input=argv[i];

		//check for ;
		if(strchr(input,';')-input){
			int in = strchr(input,';')-input;
			input[in] = '\0';
		}
        	
		//check for && a
		execute(input);		 
	}
}
        	
