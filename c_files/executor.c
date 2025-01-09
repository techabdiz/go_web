#include <sys/mman.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <unistd.h>

int main () { 
    
    int fd = open("./token.txt", O_RDONLY);
    if(fd < 0) {
        perror("failed to open file");
        exit(1); 
    }

    struct stat buf;
    if ( fstat(fd, &buf) < 0 ) {
         perror("failed to fstat");
        exit(1); 
    } 

    void* file = mmap(NULL, buf.st_size, PROT_READ|PROT_WRITE, MAP_PRIVATE, fd, 0);
    printf("mapped file at address: %p\n", file);

    for (int i = 0; i < buf.st_size; i++ ) {
        if(i > 6) { 
            *((char*)(file + i)) = 'a';
        } 
        printf("%c", *(((char*)file)+i ));
    }
    msync(file, buf.st_size, MS_SYNC);
    munmap(file, buf.st_size);
    close(fd);

}