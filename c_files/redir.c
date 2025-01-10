#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/wait.h>

extern char **environ;

const int FD_STDOUT = 1; // file descriptor for standard output

int redirect (const char* prog, const char* f_out, const char* args[]) { 

    pid_t pid = fork();
    if(pid < 0) { 
        // fork error branch [still in parent]
        perror("fork failed"); 
        return 0;
    } else if ( pid ==  0) { 
        // child branch [process]
        int fd = open(f_out, O_WRONLY|O_CREAT|O_TRUNC, S_IWUSR|S_IRUSR |S_IRGRP|S_IWGRP |S_IROTH);
        if(fd < 0 ) { 
            perror("failed to open file for redirection");
            exit(1);
        }

        if (dup2(fd, FD_STDOUT) < 0) { // now std out is pointing to the opened file (f_out) 
            perror("redirection failed, unable to point stdout to outfile");
            exit(1);
        }

        execve(prog, (char* const*) args, environ);
        perror("exec failed"); 
        exit(1);
    }else { 
        // parent branch [process]
        int status = 0;
        pid_t cpid = wait(&status);
        if(cpid < 0) { 
            perror("failed to wait for child");
            return 0;
        }
        printf("child process %d completed execution with\n", cpid);

        return WIFEXITED(status) && WEXITSTATUS(status) == 0;
    }   
}

int main(void){

    const char* args[] = { "ls", "-lrt", "/",  NULL };

    int res = redirect("/usr/bin/ls", "tokens.txt",  args);
    if(res)  {
        printf("success\n");
        return 0;
    }else { 
        printf("failed\n");
        return 1;
    }
}