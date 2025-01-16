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
extern void free_tokens(token * tk) ;

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
    char* path = getenv("PATH");
    if(path != NULL) { 
        char* buf = malloc(1024);
        strcpy(buf, path);
        return split_by_delim(buf, ':');
    }
    /*while(environ[i]) {
        if(strncmp("PATH", environ[i], 4) == 0) { 
            char* buf = malloc(1024);
            strcpy(buf, environ[i]+5);
            return split_by_delim(buf, ':');
        }
        i++;
    }*/
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
        if (c->t != NULL) {  // i've got a bad feeling about this... should be in extreme sync with token_size
            if (c->next == NULL)  { 
                *(c->t+(c->length-1)) = '\0';
            }
            args[index] = c->t;
            index++;
        }
    }

    if(*args[0] == '\0') { // if blank command , look for next
        return 0;
    }else if (strcmp(args[0], "exit") == 0)  { 
        goto exit_clean_up_start;
        exit(0);
    }else if (strcmp(args[0], "cd") == 0)  { 
        if(args[1] != NULL) { 
            if(strcmp(args[1], "-") == 0) { 
                chdir(getenv("OLDPWD")); // this is buggy
            } else if(strcmp(args[1], "~") == 0) { 
                chdir(getenv("HOME"));
            }else if(strncmp(args[1], "~/", 2) == 0) { 
                char path[1024];
                strcpy(path, getenv("HOME"));
                strcat(path, args[1]+1);
                chdir(path);
            }else { 
                chdir(args[1]);
            }            
        } else { 
            chdir(getenv("HOME"));
        }
        return 0;
    }

    int pid = fork();
    

    if ( pid == 0 ) { 
        int executed = 0;
        for(token* c = search_dirs; c != NULL; c = c->next) { // execvp does this automatically from path variable
            char* comm_path = malloc(1024);
            
            if( strncmp(".", t->t, 1) == 0 || strncmp("/", t->t, 1) == 0) { 
                strcpy(comm_path, t->t);
            }else { 
                strcpy(comm_path, c->t);
                strcat(comm_path, "/");
                strcat(comm_path, t->t);
            }
            
            
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
    wait(NULL); // will be trouble for bg processes
    free_tokens(t);
    free_tokens(search_dirs);
    goto exit_clean_up_done;
    exit_clean_up_start: //cleanup and exit (only done exits on intentionals)
        free(command);
        exit(0);
    exit_clean_up_done:
    return 0;
}

int token_size(token* ptr) { 
    int size = 0;
    for(token* c = ptr; c != NULL; c = c->next) { 
        if (c->t != NULL) { 
           size++;
        }
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


void free_tokens(token * tk) {  //freeing a singly linked list
    if(tk == NULL) { 
        return;
    }
    free_tokens(tk->next);
    free(tk);
}



int get_next_input(char* command) { 
    char cwd[1024];

    if (getcwd(cwd, sizeof(cwd)) != NULL) {
    } else {
        perror("[getcwd() error]");
    }
    fprintf(stdout, "[");
    fprintf(stdout, getenv("USER"));
    fprintf(stdout, "@");
    fprintf(stdout, getenv("LOGNAME"));
    fprintf(stdout, " ");
    fprintf(stdout, cwd);
    fprintf(stdout, "]$ ");
    fflush(stdout);

    int total_read = 0;
    while(1) {
        char ch  = '\0';
        int rd_size = read(0, &ch, 1);
        if(ch == '\t') {
            // implement backspace
            printf("\r\b \r\b");
            fflush(stdout);
            break;
        }
        *(command+total_read) = ch;
        if(ch == '\n') {
            break;
        }
        total_read++;
    }
    return total_read;
}