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
/*void connected(char *x[], vector <int> c, int count)
{
	c.clear();
	for(int i = 0; i < count; i++){
		if(x[i] == "&&"){
			c.push_back(1);
		}
		else if(x[i] == "||"){
                        c.push_back(2);
		}
		else{
			c.push_back(0);
		}
	}		
}*/
int main()
{
	//get user login and hostname
	string log;
	log = getlogin();
	char hostname[HOST_NAME_MAX];
	if (log==""){
		perror("login failed");
	}
	if(gethostname(hostname, sizeof hostname)){
		perror("hostname failed");
	}
		
	//get input
	bool run = true;
	string input;
	while(run){
  		//output user and login
		cout << log << '@' << hostname << " $ ";
	
		//will get every input before a newline
		getline(cin,input);
		
		//check for comments
		if(input.find('#') != string::npos){
                        input.erase(input.find('#'), input.size());
                }

		//check for ;
		if(input.find(';') != string::npos){
			std::replace(input.begin(), input.end(), ';', '\0');
		}	
	
		//check for && a
		char c[1024];
		strncpy(c, input.c_str(), sizeof(c));
		c[sizeof(c)-1]=0;
		vector <int> conn;
		char **forkn = new char *[1024];
		char *tok = strtok(c, " ");
		int count = 0;
		while(tok != NULL){
			forkn[count] = tok;
			count++;
			tok = strtok(NULL," ");
		}
		forkn[count]=NULL;
		//connected(forkn, conn, count);
	//	for(int j = 0; j < count; j++){
			if(strcmp(forkn[0],"exit")==0){
				run=false;
				break;
			}
			execute(forkn);	
	//	}	 			 
	}
	return 0;
}
        	
