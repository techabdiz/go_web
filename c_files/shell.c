#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define MAX_COMMAND_SIZE 1024
int get_next_input(char* command) ;
int execute_command(char* command);
int split_by_delim (char *command, char delim, char*** result) ;

int main () { 
    printf("ManGo Software INC.\n\n");
    char* command = malloc(MAX_COMMAND_SIZE);
    
    while(1) { 
        get_next_input(command);
        printf("executing: %s\n", command);

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


int execute_command(char* command) { 
    
    int token_count = 1;
    for ( int i = 0; *(command+i) != '\0' ; i++) { 
        if(*(command+i) == ' ') { 
            token_count++;
        }
    }
    char* command_tokens[token_count];
    command_tokens[0] = command;
    int issued_tokens = 1;

    if(token_count > 1) {
        for ( int i = 0; *(command+i) != '\0' ; i++) { 
            if(*(command+i) == ' ') { 
                *(command+i) = '\0';
                command_tokens[issued_tokens++] = command+(i+1);
            }
            char** result;
            int token_count = split_by_delim(command, ' ', &result);
            for(int i =0; i < token_count-1; i++)  {
                printf("token: %s\n", (*(result)+i) );
            }
        }
    }
}

int split_by_delim (char *command, char delim, char*** result) { 
    int token_count = 1;
    for ( int i = 0; *(command+i) != '\0' ; i++) { 
        if(*(command+i) == delim) { 
            token_count++;
        }
    }
    char* command_tokens[token_count];
    command_tokens[0] = command;
    int issued_tokens = 1;

    if(token_count > 1) {
        for ( int i = 0; *(command+i) != '\0' ; i++) { 
            if(*(command+i) == delim) { 
                *(command+i) = '\0';
                command_tokens[issued_tokens++] = command+(i+1);
            }
        }
    }
    *(result) = command_tokens;
    return issued_tokens;
}


int get_next_input(char* command) { 
    fprintf(stdout, "shell>");
    fflush(stdout);
    int rd_size = read(0, command, MAX_COMMAND_SIZE);
    return rd_size;
}