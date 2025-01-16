#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

typedef struct token { 
    int length;
    char * t;
    struct token* next;
}token;


#define MAX_COMMAND_SIZE 1024
int get_next_input(char* command) ;
int execute_command(char* command);
token* split_by_delim (char *command, char delim);

extern char **environ;
extern int token_size(token* ptr);

int main () { 
    printf("ManGo Software INC.\n\n");
    char* command = malloc(MAX_COMMAND_SIZE);
    
    while(1) { 
        get_next_input(command);

        if (strcmp("exit", command)  == 0) {
            printf("ok... i see exit\n");
            /// even pn "i exit... it will quit... so fix it "
            break;
        }
        execute_command(command);
        memset(command, 0, MAX_COMMAND_SIZE);
    }


    free(command);
    printf("good bye\n");
}


token* getpath() { 
    int i = 0;
    while(environ[i]) {
        if(strncmp("PATH", environ[i], 4) == 0) { 
            char* buf = malloc(1024);
            strcpy(buf, environ[i]+5);
            return split_by_delim(buf, ':');
        }
        i++;
    }
    return NULL;
}


int execute_command(char* command) { 
    token* search_dirs = getpath();
    token* t = split_by_delim(command, ' ');
    int arg_size = token_size(t);
    char* args[arg_size+1];
    args[arg_size]=NULL;
    int index = 0;
    for(token* c = t; c != NULL; c = c->next) {
        if (c->next == NULL)  { 
            *(c->t+(c->length-1)) = '\0';
        }
        args[index] = c->t;
        index++;
    }
    int pid = fork();
    

    if ( pid == 0 ) { 
        int executed = 0;
        for(token* c = search_dirs; c != NULL; c = c->next) { 
            char* comm_path = malloc(1024);
            strcpy(comm_path, c->t);
            strcat(comm_path, "/");
            strcat(comm_path, t->t);
            if(execve(comm_path, args, environ) != -1) {
                executed = 1;
                break;
            }else { 
            }
            free(comm_path);
        }
        printf("command %s not found\n", t->t);        
        exit(EXIT_SUCCESS); // set proper exit status
    }

    wait(NULL);
    return 0;
}

int token_size(token* ptr) { 
    int size = 0;
    for(token* c = ptr; c != NULL; c = c->next) { 
        size++;
    }
    return size;
}

// create function for freeing this linked list
token* split_by_delim (char *command, char delim) { 
    token* start = malloc(sizeof(token));
    token* current = start;
    current->t = command;
    int laslen = 0;
    for ( int i = 0; *(command+i) != '\0' ; i++) { 
        laslen++;
        if(*(command+i) == delim) { 
            *(command+i) = '\0';
            current->length = laslen;
            current->next = malloc(sizeof(token));
            current = current->next;
            current->t = (command+i)+1;
            laslen = 0;
        }
    }
    current->length = laslen;
    current->next = NULL;
    return start;
}


int get_next_input(char* command) { 
    fprintf(stdout, "shell>");
    fflush(stdout);
    int rd_size = read(0, command, MAX_COMMAND_SIZE);
    return rd_size;
}