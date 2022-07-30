#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>

int main(void) {
    int net_socket = socket(AF_INET, SOCK_DGRAM, 0);
    
    if (net_socket < 0) {
        perror("socket error"); // permission denied indicator
    } else {
        printf("socket created");
    }

    // creating the server struct
    struct sockaddr_in sv_addr;
    sv_addr.sin_family = AF_INET;
    sv_addr.sin_port = htons(12345); // fixed
    sv_addr.sin_addr.s_addr = inet_addr("0.0.0.0");
    //inet_aton("0.0.0.0", &sv_addr.sin_addr);
    //inet_pton(AF_INET, "0.0.0.0", &sv_addr.sin_addr);
    //advantage of pton: convert IP address to 128bit
    
    int len = sizeof(struct sockaddr_in);
    int br = bind(net_socket, (struct sockaddr *)&sv_addr, len);
    // bind() function doesn't takes value result argument

    if (br < 0) {
        perror("bind error");
    } else {
        printf("bind completed\n");

        int len = sizeof(struct sockaddr_in);
        getsockname(net_socket, (struct sockaddr *)&sv_addr, &len); // provides the local address
        printf("local ip: %s\n", inet_ntoa(sv_addr.sin_addr));
        printf("local port: %d\n", inet_ntohs(sv_addr.sin_port));
    }

    char buffer[200];
    int n;
    while (1) {
        n = read(net_socket, buffer, 200);
        if (n < 0) {
            perror("read failed");
        } else {
            buffer[n] = '\0';
            n = write(net_socket, buffer, strlen(buffer));
            //onsuccessful execution of write() function: characters need to send

            if (n < 0) {
                perror("write failed");
            }
            close(net_socket);
            return 0;
        }
    }
}