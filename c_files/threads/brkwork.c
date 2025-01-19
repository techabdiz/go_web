#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main () { 


    int* current = (int*)sbrk(0);// current position
    sbrk(4);
    *current = 10;
    printf("current program break: %p -- %d\n", current, *current);
    printf("current program break: %p -- %d\n", current, *current);
    current = sbrk(0)-8;
    printf("current program break: %p -- %d\n", current, *current);
}
