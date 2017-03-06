#ifndef TEST_H
#define TEST_H

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

/* class test : public execute {
    public:
        test();
        bool tes(vector<string>);
};
*/
bool tes(vector<string> command){
    struct stat path;
    bool flag;
    int place = 0;
    
    if(command.at(1) == "-e" || command.at(1) == "-f" || command.at(1) == "-d") {
        flag = true;
        place++;
    }
    else {
        flag = false;
    }
    
    if(!flag && command.at(1) != "") {
        perror("The invalid flag given");
        return false;
    }
    
    if(stat((char*)command.at(place).c_str(), &path) == -1) {
        perror("Not accessible");
        return false;
    }
    
    if(((command.at(1) == "-e" || !flag) && path.st_mode) || (command.at(1) == "-f" && S_ISREG(path.st_mode)) || (command.at(1) == "-d" && S_ISDIR(path.st_mode))) {
        cout << "(True)" << endl;
        return true;
    }
    else {
        cout << "(False)" << endl;
        return false;
    }
}

#endif