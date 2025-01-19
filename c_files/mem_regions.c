#include <stdio.h>


int initzed = 10;
int uninit;
int initzed_2 = 10;
int initzed_3 = 10;
int uninit_2;


void print_regions () { 
    int local = 10;
    int local_2 = 10;
    printf("initialized seg address:   %p\n", &initzed);
    printf("initialized seg address:   %p\n", &initzed_2);
    printf("initialized seg address:   %p\n", &initzed_3);
    printf("uninitialized seg address: %p\n", &uninit);
    printf("uninitialized seg address: %p\n", &uninit_2);
    printf("local seg address: %p\n", &local);
    printf("local seg address: %p\n", &local_2);
    printf("Code segment: %p\n", &print_regions);
    int* code_region = (int*)&print_regions;
    *(code_region) = 11111;
    
}

int main () { 
    print_regions();
    return 0;
}