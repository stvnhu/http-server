
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

int main(int argc, char *argv[]) {

    int port = 8080;

    if (argc > 1) {
        int new_port = atoi(argv[1]);
        if (new_port != 0) {
            port = new_port;
        }
    }

    int server_fd;
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket()");
        return 1;
    }

    const int opt = 1;
    if ((setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt,
                    sizeof(opt))) == -1) {
        perror("setsockopt()");
        return 1;
    }

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = INADDR_ANY;

    if ((bind(server_fd, (const struct sockaddr *)&addr, sizeof(addr))) == -1) {
        perror("bind()");
        return 1;
    }

    if ((listen(server_fd, SOMAXCONN)) == -1) {
        perror("listen()");
        return 1;
    }

    printf("Listening for connections on port %i...\n\n", port);

    for (;;) {

        int client_fd;
        if ((client_fd = accept(server_fd, NULL, NULL)) == -1) {
            perror("accept()");
            continue;
        }

        printf("Connection accepted\n");

        char recv_buffer[2048];
        ssize_t nbytes = 0;
        ssize_t total = 0;

        for (;;) {
            nbytes = recv(client_fd, &recv_buffer[total],
                          sizeof(recv_buffer) - total, 0);

            switch (nbytes) {
            case -1:
                perror("recv()");
            case 0:
                break;
            }

            total += nbytes;

            if (strstr(recv_buffer, "\r\n\r\n") != NULL) {
                printf("%s\n", recv_buffer);
                char *send_buffer = "HTTP/1.1 200 OK\r\n"
                                    "Content-Type: text/plain\r\n"
                                    "Content-Length: 6\r\n"
                                    "Connection: close\r\n\r\n"
                                    "Hello!";
                send(client_fd, send_buffer, sizeof(char) * strlen(send_buffer),
                     0);
                break;
            }
        }
        close(client_fd);
    }

    return 0;
}
