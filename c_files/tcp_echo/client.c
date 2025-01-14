#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>



int connect_host(const char* host, const char* port) { 
    int sock_fd = 0; // client socket file descriptor
    struct addrinfo* addr;
    struct addrinfo hints;
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    int addr_info_res = getaddrinfo(host, port, &hints, &addr);
    if (addr_info_res < 0) { 
        fprintf(stderr, "getaddrifo: %s\n", gai_strerror(addr_info_res));
        exit(EXIT_FAILURE);
    }

    for (struct addrinfo* next = addr; next ; next = next->ai_next) { 
        sock_fd = socket(next->ai_family, next->ai_socktype, 0);
        if(sock_fd < 0) { 
            perror("socket creation failed");
            continue;
        }

        if(connect(sock_fd, next->ai_addr, next->ai_addrlen) == 0) { 
            printf("connected successfully to %s \n", host);
            break;
        } 
    }
    close(sock_fd);
    freeaddrinfo(addr);
}


int main () { 
    connect_host("www.twitter.com", "80");
}