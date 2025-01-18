#include <stdio.h>
#include <unistd.h>

int main () { 
    printf("current brk: %p\n", (void*)sbrk(0));
    printf("current brk: %p\n", (void*)sbrk(0));
    printf("current brk: %p\n", (void*)sbrk(0));
    printf("current brk: %p\n", (void*)sbrk(0));
    printf("current brk: %p\n", (void*)sbrk(0));
    printf("current brk: %p\n", (void*)sbrk(0));
    printf("current brk: %p\n", (void*)sbrk(0));
}
