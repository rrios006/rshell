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
#include "test.h"

using namespace std;

bool run(vector<string> c, char** f) {
    bool test = false;
    bool parth = false;
    bool passed;
    int loop;
    execute e;
    unsigned int i = 0;
    while(i < c.size()) {
    	if(c.at(i) == "exit") {
    		cout << "Exiting" << endl;
    		exit(0);
    	}
    	if(c.at(i) == "test" || c.at(i) == "[") {
    		test t;
    		i++;
    		vector<string> testcom;
    		testcom.push_back(c.at(i));
    		i++;
    		if(c.at(i).find('-') != string::npos) {
    			testcom.push_back(c.at(i));
    			i++;
    		}
    		else{
    			testcom.push_back("");
    		}
    		passed = t.test(testcom);
    		if(i >= c.size()) {
    			break;
    		}
    		else if(c.at(i) == "]") {
    			i++;
    			if(i >= c.size()) {
    				break;
    			}
    		}
    		test = true;
    		loop = 0;
    	}
    	//in case of semicolon, just act as if new array
    	else if(c.at(i) == ";") {
    		if(!test){
	    		if(!parth) {
    				f[loop] = 0;
    				passed = e.execute(f);
    			}
    			else{
    				parth = false;
    			}
    		}
    		else{
    			test = false;
    		}
    		loop = 0;
    		i++;
    	}
    	//case of connector "&&", will only run if last command ran
    	if(c.at(i) == "&&") {
    		if(!test){
	    		if(!parth) {
    				f[loop] = 0;
    				passed = e.execute(f);
    			}
    			else{
    				parth = false;
    			}
    		}
    		else{
    			test = false;
    		}
    		
    		if(!passed) {
    			break;
    		}
    		else{
    			loop = 0;
    			i++;
    		}
    	}
    	//case of connector "||", will only run if last command didn't run
    	else if(v.at(i)=="||") {
    		if(!test){
	    		if(!parth) {
    				f[loop] = 0;
    				passed = e.execute(f);
    			}
    			else{
    				parth = false;
    			}
    		}
    		else{
    			test = false;
    		}
    		
    		if(!passed) {
    			loop = 0;
    			i++;
    		}
    		else{
    			while(i < v.size()) {
    				if(v.at(i) == "&&" || v.at(i) == ";") {
    					loop = 0;
    					i++;
    					break;
    				}
    				else if(v.at(i) == "(") {
    					while(v.at(i) != ")" && i < v.size()) {
    						i++;
    					}
    					i++;
    					break;
    				}
    				i++;
    			}
    			//check if end of array
    			if(i >= v.size()) {
    				break;
    			}
    		}
    	}
		else if(v.at(i) == "(") {
			vector<string> parenthesis;
			parth = true;
			++i;
			while(v.at(i) != ")"){
				parenthesis.push_back(v.at(i));
				i++;
			}
			passed = run(parenthesis, f);
			i++;
			loop = 0;
		}
		else {
			if(!test){
				f[loop] = (char*)v.at(i).c_str();
				loop++;
				f[loop] = 0;
				i++;
				if(i >= v.size()) {
					passed = e.execute(f);
					break;
				}
			}
			else{
				test = false;
				break;
			}
		}
    }
    
    return passed;
}

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
	bool pro = true;
	string input;
	while(pro){
  		//output user and login
		cout << log << '@' << hostname << " $ ";
	
		//will get every input before a newline
		getline(cin,input);
		
		//check for comments and deletes anything after '#'
		if(input.find('#') != string::npos){
            input.erase(input.find('#'), input.size());
        }
		
		//takes input and makes vector of tokens in proper order for running
		//parsing
		char c[1024];
		strncpy(c, input.c_str(), sizeof(c));
		c[sizeof(c)-1]=0;
		vector <string> commtree;
		char *tok = strtok(c, " ");
		for (unsigned i = 0; tok != NULL; i++) {
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
		    else if(command.find(')') != string::npos) {
		    	if(command.size() == 1) {
		    		commtree.push_back(command);
		    	}
		    	else{
		    		command.erase(command.begin() + command.size()-1);
			    	commtree.push_back(command);
			    	commtree.push_back(")");
		    	}
		    }
		    else if(command.find('(') != string::npos) {
		    	if(command.size() == 1) {
		    		commtree.push_back(command);
		    	}
		    	else{
		    		command.erase(command.begin());
			    	commtree.push_back("(");
			    	commtree.push_back(command);
		    	}
		    }
		    else if(command.find(']') != string::npos) {
		    	if(command.size() == 1) {
		    		commtree.push_back(command);
		    	}
		    	else{
		    		command.erase(command.begin() + command.size()-1);
			    	commtree.push_back(command);
			    	commtree.push_back("]");
		    	}
		    }
		    else if(command.find('[') != string::npos) {
		    	if(command.size() == 1) {
		    		commtree.push_back(command);
		    	}
		    	else{
		    		command.erase(command.begin());
			    	commtree.push_back("[");
			    	commtree.push_back(command);
		    	}
		    }
		    else {
		        commtree.push_back(command);
		    }
    		tok = strtok(NULL, " ");
		}
		if(commtree.at(0) == "exit") {
            exit(0);
        }
		char *forkn[commtree.size() + 1];
		
		run(commtree,forkn);
	}
	return 0;
}