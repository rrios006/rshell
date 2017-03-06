#ifndef EXECUTE_H
#define EXECUTE_H

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
#include <limits.h>

#include <cstdio>
#include <cstring>
#include <iostream>
#include <iomanip>
#include <map>
#include <vector>
#include <algorithm>

using namespace std;

class execute {
    public:
    	execute();
        bool exec(char **);
};

bool execute::exec(char **forkn){
    int status = 0;
	pid_t pid= fork();
	if (pid < 0){
		perror("Forking child process failed");
		return false;
		exit(1);
	}
	else if (pid == 0) {
		if (execvp(forkn[0], forkn) < 0) {
			perror("Execute failed");
			return false;
			exit(1);
		}
	}
	else {
	    waitpid(pid,&status,0);
	}
	return true;
}

#endif