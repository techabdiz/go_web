#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>


extern void fork_basic(void);
extern void fork_parent_looper(void);
extern void fork_surviving_child(void);
extern void fork_wait_basic(void);
extern void fork_wait_manual(void);

int main () { 
    fork_wait_basic();
}

void fork_wait_manual(void) { 
    int pid = fork () ;
    if(pid != 0 ) { 
        printf("parent code %d\n", getpid());
        int comp = wait(NULL);
        printf("parent code %d exiting when %d completed\n", getpid(), comp);
        exit(0); 
    }
    sleep(2);
    printf("child %d completes its execution\n", getpid());

}

void fork_wait_basic(void) { 
    int NUM_WORKERS = 10;
    int mother_pid = getpid(); // where it all started (ppid)
    for (int i = 0; i < NUM_WORKERS; i ++ ) { 
        int pid = 0;
        if(getpid() == mother_pid) { 
            pid = fork();
            continue;
        }
        
        /** 
         * whatever i write between will be executed by NUM_WORKER worker processors 
         * 
         * -------------- START ------------------------------------------------------
         */
        sleep(1+i); // sleep for variable time 
        printf("child %d completes its execution\n", getpid());


         /** 
         * -------------- END ------------------------------------------------------
        * 
         * whatever i write between will be executed by NUM_WORKER worker processors 
         * 
         */
        if(pid == 0) {
            // child doesnt need to loop 
            break;
        }
    }

    if( getpid() == mother_pid ) { 
        printf("parent code %d\n", getpid());
        for ( int i = 0; i < NUM_WORKERS; i ++ ) { 
            // synchronously waiting for each child termination
            int comp = wait(NULL); 
            printf("parent code %d exiting when %d completed\n", getpid(), comp); 
        }
        exit(0); 
    }
}


void fork_surviving_child(void) { 
    int pid = fork();

    if(pid == 0) {
        printf("I am the child: %d\n", getpid()); 
        while(1); /* for investigation in /proc directory */
    }
    
    printf("Terminating parent: %d\n", getpid());
    exit(0);
}



void fork_parent_looper(void) { 
    int pid = fork();

    if(pid == 0) {
        printf("I am the child: %d\n", getpid()); 
        exit(0);
    }
    
    printf("parent continues with the program: %d\n", getpid());
    while(1); /* for investigation in /proc directory */
}

void fork_basic(void) { 
    int pid = fork();

    if(pid == 0) {
        printf("I am the child ... \n"); 
        exit(0);
    }
    
    printf("parent continues with the program ... \n");
}

void fork_levels(void) { 
    printf("L0\n");
    fork();
    printf("L1\n");
    fork();
    printf("BYE\n");
}
