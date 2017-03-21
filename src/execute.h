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

/* class execute {
    public:
    	execute();
        bool exec(char **);
};*/

bool exec(char **forkn){
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

bool redirect(vector<string> piper) {
	//a bunch of variables needed for redirection
	int fds[2];
	int in = 0;
	int out = 1;
	int reput;
	bool infile = false;
	bool outfile = false;
	bool In = false;
	bool Out = false;
	bool Outs = false;
	pid_t pid;
	char** forkn = new char*[1028];
	int wait = 0;
	vector< vector<string> > whole;
	vector<string> single;
	//will go through vector to set flags and parse for redirection
	for(unsigned i = 0; i < piper.size(); i++){
		if(piper[i] == "<"){
			whole.push_back(single);
			In = true;
			single.clear();
		}
		else if(piper[i] == ">"){
			whole.push_back(single);
			Out = true;
			single.clear();
		}
		else if(piper[i] == ">>"){
			whole.push_back(single);
			Outs = true;
			single.clear();
		}
		else if(piper[i] == "|"){
			whole.push_back(single);
			single.clear();
		}
		else{
			single.push_back(piper.at(i));
		}
	}	
	//check each flag
	whole.push_back(single);
	if(In == true){
		in = open(whole[1].front().c_str(), O_RDONLY);
		reput = in;
		whole.erase(whole.begin() + 1);
		infile = true;
	}
	if(Out == true){
		out = open(whole.back().front().c_str(), O_WRONLY | O_TRUNC | O_CREAT, S_IRUSR | S_IRGRP | S_IWGRP | S_IWUSR);
		whole.pop_back();
		outfile = true;
	}
	else if(Outs == true){
		out = open(whole.back().front().c_str(), O_WRONLY | O_APPEND | O_CREAT, S_IRUSR | S_IRGRP | S_IWGRP | S_IWUSR);
		whole.pop_back();
		outfile = true;
	}
	for(unsigned x = 0; x < whole.size(); x++){
		pipe(fds);
		pid = fork();
		if(pid == 0){
			dup2(reput,0);
			if(x < whole.size()-1){
				dup2(fds[1],1);
			}
			else{
				dup2(out,1);
			}
			close(fds[0]);
			for(unsigned y = 0; y < whole[x].size(); y++){
				string temp = whole[x][y];
				char* f = (char *)temp.c_str();
				forkn[y] = f;
				if(whole[x].size() == 1){
					forkn[y+1] = '\0';
				}
				else if(y == whole[x].size()-1){
					forkn[y+1] = '\0';
				}
			}
			wait = execvp(forkn[0],forkn);
			perror("Execute failed");
			return false;
		}
		else if(pid < 0){
			perror("fork error");
			return false;
		}
		else{
			if(waitpid(pid, &wait, 0) != pid){
				perror("Error child");
			}
			close(fds[1]);
			reput = fds[0];
		}
	}
	//close files that were opened
	if(infile == true){
		close(in);
	}
	if(outfile == true){
		close(out);
	}
	return true;
}

#endif
