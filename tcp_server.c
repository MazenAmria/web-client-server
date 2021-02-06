//
// Created by Mazen Amria on 06/02/2021.
//

#include "common.h"

int32_t main (int32_t argc, char ** argv) {
    int32_t listener_fd;
    int32_t connection_fd;
    int32_t n;
    char send_line[MAX_LINE];
    char recv_line[MAX_LINE];
    struct sockaddr_in server_addr;
    FILE *recv_request = fopen("recv_request", "w");

    if ((listener_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        raise_err("Error at socket creation.");
    }

    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if(bind(listener_fd, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0) {
        raise_err("Binding error");
    }

    if(listen(listener_fd, 10) < 0) {
        raise_err("Listening error");
    }

    for ( ; ; ) {
        struct sockaddr_in addr;
        socklen_t addr_len;

        printf("Listening for requests on port: %d\n", SERVER_PORT);
        fflush(stdout);
        connection_fd = accept(listener_fd, (struct sockaddr *) NULL, NULL);

        memset(recv_line, 0, MAX_LINE);

        while ((n = read(connection_fd, recv_line, MAX_LINE)) > 0) {
            fprintf(recv_request,"%s", recv_line);
            if (strcmp(recv_line + n - 4, "\r\n\r\n") == 0) {
                break;
            }
            memset(recv_line, 0, MAX_LINE);
        }

        fclose(recv_request);

        if (n < 0) {
            raise_err("Read error.");
        }

        snprintf(send_line, MAX_LINE, "HTTP/1.1 200 OK\r\n\r\nHello.");
    }
}
