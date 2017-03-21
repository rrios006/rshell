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
#include <stack>

#include <cstdio>
#include <cstring>
#include <iostream>
#include <iomanip>
#include <map>
#include <vector>
#include <algorithm>
#include "test.h"
#include "execute.h"

using namespace std;

bool run(vector<string> c, char** f) {
    bool testck = false;
    bool parth = false;
    bool passed;
    int loop;
    unsigned int i = 0;
    while(i < c.size()) {
    	// exit function
    	if(c.at(i) == "exit") {
    		cout << "Exiting" << endl;
    		return false;
		exit(0);
    	}
    	// test function
    	if(c.at(i) == "test" || c.at(i) == "[") {
    		i++;
    		vector<string> testcom;
    		if(c.at(i).find('-') != string::npos) {
    			testcom.push_back(c.at(i));
    			i++;
    		}
    		else{
    			testcom.push_back("");
    		}
    		testcom.push_back(c.at(i));
    		i++;
    		passed = tes(testcom);
    		if(i >= c.size()) {
    			break;
    		}
    		else if(c.at(i) == "]") {
    			i++;
    			if(i >= c.size()) {
    				break;
    			}
    		}
    		testck = true;
    		loop = 0;
    	}
    	//in case of semicolon, just act as if new array
    	else if(c.at(i) == ";") {
    		if(!testck){
	    		if(!parth) {
    				f[loop] = 0;
    				passed = exec(f);
    			}
    			else{
    				parth = false;
    			}
    		}
    		else{
    			testck = false;
    		}
    		loop = 0;
    		i++;
    	}
    	
    	//case of connector "&&", will only run if last command ran
    	else if(c.at(i) == "&&") {
    		if(!testck){
	    		if(!parth) {
    				f[loop] = 0;
    				passed = exec(f);
    			}
    			else{
    				parth = false;
    			}
    		}
    		else{
    			testck = false;
    		}
    		
    		if(!passed) {
    			return false;
			break;
    		}
    		else{
    			loop = 0;
    			i++;
    		}
    	}
    	//case of connector "||", will only run if last command didn't run
    	else if(c.at(i)=="||") {
    		if(!testck){
	    		if(!parth) {
    				f[loop] = 0;
    				passed = exec(f);
    			}
    			else{
    				parth = false;
    			}
    		}
    		else{
    			testck = false;
    		}
    		
    		if(!passed) {
    			loop = 0;
    			i++;
    		}
    		else{
    			while(i < c.size()) {
    				if(c.at(i) == "&&" || c.at(i) == ";") {
    					loop = 0;
    					i++;
    					break;
    				}
    				else if(c.at(i) == "(") {
    					while(c.at(i) != ")" && i < c.size()) {
    						i++;
    					}
    					i++;
    					break;
    				}
    				i++;
    			}
    			//check if end of array
    			if(i >= c.size()) {
   				break;
    			}
		}
    	}
    		//check for parathesis
	else if(c.at(i) == "(") {
		vector<string> parenthesis;
		parth = true;
		++i;
		while(c.at(i) != ")"){
			parenthesis.push_back(c.at(i));
			i++;
			if(i == c.size()-1) {
				perror("No end parathesis");
				exit(1);
			}
		}
		passed = run(parenthesis, f);
		i++;
		loop = 0;
	}
	//check for redirection and makes subvector
	else if(i!=c.size()-1){
		if(c.at(i+1) == "<" || c.at(i+1) == ">" || c.at(i+1) == ">>" || c.at(i+1) == "||"){
			vector<string> piper;
			while(i != c.size()) {
				piper.push_back(c.at(i));
				i++;
				if(!(c.at(i) == "<" || c.at(i) == ">" || c.at(i) == ">>" || c.at(i) == "|") && !(c.at(i-1) == "<" || c.at(i-1) == ">" || c.at(i-1) == ">>" || c.at(i-1) == "|") ){
					break;
				}
			}
			passed = redirect(piper);
			loop = 0;
		}
	}	
	
	else {
			if(!testck){
				f[loop] = (char*)c.at(i).c_str();
				loop++;
				f[loop] = 0;
				i++;
				if(i >= c.size()) {
					passed = exec(f);
					break;
				}
			}
			else{
				testck = false;
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
		    //this will check for parathesises
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
		    //for test function brackets
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
		char **forkn = new char *[1024];
		
		pro=run(commtree,forkn);
	}
	return 0;
}
