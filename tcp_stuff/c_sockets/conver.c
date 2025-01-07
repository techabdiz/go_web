#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>  // For inet_ntop (convert binary to text)

int main ()  {
    struct sockaddr_in sa;
    inet_pton(AF_INET, "192.168.0.2::8080", &(sa.sin_addr));

    printf("port: %d\n", &(sa.sin_addr).s_addr);


}