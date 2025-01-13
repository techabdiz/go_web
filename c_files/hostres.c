#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h> 

const int HOST_LEN = 24;
const int PORT_LEN = 24;


void print_ips(const char* host, const char* port) { 
    struct addrinfo hints;
    struct addrinfo* info, *next;
    memset(&hints, 0, sizeof(hints));
    int res = getaddrinfo(host, port, &hints, &info);
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    if (res > 0) { 
        printf("ok... addrinfo failed");
    }

    char* look_host = (char*) malloc(HOST_LEN);
    char* look_port = (char*) malloc(PORT_LEN);
    for ( next = info; next != NULL; next = next->ai_next ) { 
        int addr = getnameinfo(next->ai_addr, next->ai_addrlen ,  look_host, HOST_LEN, look_port, PORT_LEN, NI_NUMERICHOST | NI_NUMERICSERV);
        printf("%s:%s\n", look_host, look_port);
    }

    free(look_host);
    free(look_port);
    freeaddrinfo(info);
}


int main (int argc, char** argv)  { 
    if(argc < 3) { 
        printf("usage: hostres <hostname> <port>\n");
        exit(1);
    }    

    print_ips(argv[1], argv[2]);
}