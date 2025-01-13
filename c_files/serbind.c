#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <string.h>

const int BACK_SIZE = 10;

int main () { 

    struct addrinfo hints;
    struct addrinfo *info;

    int res = getaddrinfo("localhost", "9090", &hints, &info);
    if(res < 0) {
        printf("getaddrinfo failed"); 
        exit(1);
    }
    int sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if ( sock_fd < 0 ){ 
        perror("socket creation failed");
        exit(1);
    } 

    int bnd =  bind(sock_fd, info->ai_addr, info->ai_addrlen); 

    if( bnd < 0 ) {    
        perror("bind failed");
        exit(1);
    }

    int lres = listen(sock_fd, BACK_SIZE);

    if( lres < 0 ) { 
        perror("listen failed");
        exit(1);
    }

    while (1) { 
        struct sockaddr addr;
        memset(&addr, 0, info->ai_addrlen);
        int nfd = accept(sock_fd, &addr, info->ai_addrlen);
        printaddr(addr, info->ai_addrlen );
        close(nfd);
    }

    close(sock_fd);
    return 0;
}

void printaddr(struct sockaddr* addr, int leng)   {
    printf("i came here\n");
    char* host = malloc(24);
    int res = getnameinfo(addr,leng ,host, 24, NULL, 0, 0);
    if (res == 0) { 
        printf("connection from host: %s\n", host);
        return;
    }
    free(host);
    printf("unable to get host info");
}