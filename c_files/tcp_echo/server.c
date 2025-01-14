#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

extern void print_socket_payload(int sock_fd);

void start_server (const char* host, const char* port) { 
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

    int sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(sock_fd < 0 ){ 
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    printf("socket created successfully... \n");

    int bind_res = bind(sock_fd, addr->ai_addr, addr->ai_addrlen);

    if(bind_res < 0) { 
        perror("socket bind failed");
        exit(EXIT_FAILURE);
    }

    printf("bind successful.. \n");
    
    int listen_res = listen(sock_fd, 8);
    if(bind_res < 0) { 
        perror("socket listen failed");
        exit(EXIT_FAILURE);
    }
    printf("successfully marked socket as listener\n");
    
    int new_fd = accept(sock_fd, addr->ai_addr, addr->ai_addrlen);
    printf("servicing the request\n");

    print_socket_payload(new_fd);

    const char* req = "GET /rfc/rfc793.txt HTTP/1.1\r\n\r\n";
    
    int write_response = write(sock_fd, req, strlen(req));

    if(write_response < 0 ) { 
        perror("writing to socket failed");
        exit(EXIT_FAILURE);
    }
    close(new_fd);
    close(sock_fd);
}



int main () { 
    start_server("localhost", "8080");
}

void print_socket_payload(int sock_fd) { 
    int buf_size = 1025;

    char buffer[buf_size];
    ssize_t bytes_read;
    printf("Server response:\n");
    while ((bytes_read = read(sock_fd, buffer, buf_size - 1)) > 0) {
        if (bytes_read >= buf_size-1) { 
            buffer[buf_size] = '\0';
        }else { 
            buffer[bytes_read] = '\0';
        }
        printf("%s", buffer);      // Print the server's response
    }
}