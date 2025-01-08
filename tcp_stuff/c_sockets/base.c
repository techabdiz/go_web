#include <stdio.h>      // For printf, perror
#include <stdlib.h>     // For exit, malloc, free
#include <string.h>     // For memset, strcpy
#include <sys/types.h>  // For socket-related types
#include <sys/socket.h> // For socket functions
#include <netdb.h>      // For getaddrinfo, addrinfo
#include <unistd.h>     // For close, read, write
#include <arpa/inet.h>  // For inet_ntop (convert binary to text)


extern void print_addrinfo(struct addrinfo *result) ;

int main () { 

    struct addrinfo hints;
    struct addrinfo *result, *next;

    const char* host = "example.com";
    const char* port = "80";
    int sock_fd ;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    
    int status = getaddrinfo( host, port, &hints, &result);

    if(status != 0) {
        printf("something went wrong with the getaddrinfo"); 
        return 1; 
    }
    print_addrinfo(result);
    for(next = result; next != NULL; next = result->ai_next) { 
        sock_fd = socket(next->ai_family, next->ai_socktype, next->ai_protocol);
        if(sock_fd == -1) { 
            printf("something went wrong the the socket creation loop");
            continue;
        }

        if(connect(sock_fd, next->ai_addr, next->ai_addrlen) != -1) { 
            printf("connected successfully... !\n");
            //close(sock_fd);
            break;
        }
        printf("looping... looping... looping... \n");
        close(sock_fd);
    }

    const char* tx =
        "GET / HTTP/1.1\r\n"
        "Host: example.com\r\n"
        "Connection: close\r\n"
        "\r\n";


    int wrt = write(sock_fd, tx, strlen(tx));

    printf("written %d bytes to the socket\n", wrt);

    char buf[128];
    ssize_t bytes_read = 0;
    while((bytes_read = read (sock_fd, buf, sizeof(buf)-1)) > 0) { 
        printf("response from server: %s\n", buf);
    }

    close(sock_fd);


}

void print_addrinfo(struct addrinfo *result) {
    struct addrinfo *rp;
    char addr_str[INET6_ADDRSTRLEN]; // Buffer for IPv4/IPv6 address as a string

    printf("Addrinfo results:\n");
    for (rp = result; rp != NULL; rp = rp->ai_next) {
        // Print the address family
        printf("  Family: ");
        if (rp->ai_family == AF_INET) {
            printf("IPv4\n");
        } else if (rp->ai_family == AF_INET6) {
            printf("IPv6\n");
        } else {
            printf("Unknown (%d)\n", rp->ai_family);
        }

        // Print the socket type
        printf("  Socket type: ");
        if (rp->ai_socktype == SOCK_STREAM) {
            printf("Stream (TCP)\n");
        } else if (rp->ai_socktype == SOCK_DGRAM) {
            printf("Datagram (UDP)\n");
        } else {
            printf("Unknown (%d)\n", rp->ai_socktype);
        }

        // Print the protocol
        printf("  Protocol: ");
        if (rp->ai_protocol == IPPROTO_TCP) {
            printf("TCP\n");
        } else if (rp->ai_protocol == IPPROTO_UDP) {
            printf("UDP\n");
        } else {
            printf("Unknown (%d)\n", rp->ai_protocol);
        }

        // Print the address
        void *addr;
        if (rp->ai_family == AF_INET) { // IPv4
            struct sockaddr_in *ipv4 = (struct sockaddr_in *)rp->ai_addr;
            addr = &(ipv4->sin_addr);
        } else if (rp->ai_family == AF_INET6) { // IPv6
            struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *)rp->ai_addr;
            addr = &(ipv6->sin6_addr);
        } else {
            printf("  Address: Unknown family\n");
            continue;
        }

        // Convert the address to a human-readable string
        if (inet_ntop(rp->ai_family, addr, addr_str, sizeof(addr_str)) == NULL) {
            perror("inet_ntop");
            continue;
        }
        printf("  Address: %s\n", addr_str);
        printf(" -------------  --------------------\n");
    }

}