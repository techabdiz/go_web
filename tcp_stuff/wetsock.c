#include <stdio.h>      // For printf, perror
#include <stdlib.h>     // For exit, malloc, free
#include <string.h>     // For memset, strcpy
#include <sys/types.h>  // For socket-related types
#include <sys/socket.h> // For socket functions
#include <netdb.h>      // For getaddrinfo, addrinfo
#include <unistd.h>     // For close, read, write

int main() {
    const char *hostname = "example.com"; // Server hostname
    const char *port = "80";              // HTTP port

    struct addrinfo hints;        // Hints for `getaddrinfo`
    struct addrinfo *result, *rp; // Linked list of results from `getaddrinfo`
    int sockfd;                   // Socket file descriptor

    // Clear the hints structure
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;     // Allow both IPv4 and IPv6
    hints.ai_socktype = SOCK_STREAM; // TCP socket

    // Get address information
    int status = getaddrinfo(hostname, port, &hints, &result);
    if (status != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
        exit(EXIT_FAILURE);
    }

    // Loop through addresses and try to connect
    for (rp = result; rp != NULL; rp = rp->ai_next) {
        sockfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
        if (sockfd == -1) {
            perror("socket");
            continue;
        }

        if (connect(sockfd, rp->ai_addr, rp->ai_addrlen) != -1) {
            break; // Success
        }

        perror("connect");
        close(sockfd);
    }

    if (rp == NULL) {
        fprintf(stderr, "Could not connect\n");
        freeaddrinfo(result);
        exit(EXIT_FAILURE);
    }

    // Free the address information
    freeaddrinfo(result);

    // Prepare an HTTP GET request
    const char *http_request = 
        "GET / HTTP/1.1\r\n"
        "Host: example.com\r\n"
        "Connection: close\r\n"
        "\r\n";

    // Write the HTTP GET request to the socket
    if (write(sockfd, http_request, strlen(http_request)) == -1) {
        perror("write");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    // Read the server's response
    char buffer[1024];
    ssize_t bytes_read;
    printf("Server response:\n");
    while ((bytes_read = read(sockfd, buffer, sizeof(buffer) - 1)) > 0) {
        buffer[bytes_read] = '\0'; // Null-terminate the string
        printf("%s", buffer);      // Print the server's response
    }

    if (bytes_read == -1) {
        perror("read");
    }

    // Close the socket
    close(sockfd);

    return 0;
}
