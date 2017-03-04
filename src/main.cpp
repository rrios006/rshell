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
		
		//check for comments and deletes anything after
		if(input.find('#') != string::npos){
            input.erase(input.find('#'), input.size());
        }
		
		char c[1024];
		strncpy(c, input.c_str(), sizeof(c));
		c[sizeof(c)-1]=0;
		vector <string> commtree;
		char **forkn = new char *[1024];
		char *tok = strtok(c, " ");
		int count = 0;
		for (i = 0; tok != NULL; i++) {
    		while(tok != NULL){
    			break;
    		}
    		string command = string(tok);
    		//This will check for semicolons
    		if(command.find(';') != string::npos) {
			    command.erase(command.begin() + command.size()-1);
			    commtree.push_back(command);
			    commtree.push_back(";");
		    }
		    else {
		        commtree.push_back(command);
		    }
    		tok = strtok(NULL, " ");
		}
		forkn[count]=NULL;
		
		if(strcmp(forkn[0],"exit")==0){
				run=false;
				break;
			}
			execute(forkn);	
	//	}	 			 
	}
	return 0;
}