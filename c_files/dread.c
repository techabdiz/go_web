#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>


int main () { 
    const char* dir_name = "/home/deadspider/source/golang/web";

    DIR* dp = opendir(dir_name);
    struct dirent* cpoint;
    while ( (cpoint = readdir(dp)) != NULL) { 
        printf("%s\n", cpoint->d_name);
       
    }
    closedir(dp);
}