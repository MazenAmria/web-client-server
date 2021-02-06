//
// Created by Mazen Amria on 06/02/2021.
//

#include "common.h"

int32_t main(int32_t argc, char **argv) {
    int32_t socket_fd;
    int32_t send_bytes, n;
    char send_line[MAX_LINE];
    char recv_line[MAX_LINE];
    struct sockaddr_in server_addr;
    FILE *recv_header = fopen("recv_header", "w");
    FILE *recv_content = fopen("recv_content", "w");

    if (argc != 2) {
        raise_err("usage: %s <server address>", argv[0]);
    }

    if ((socket_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        raise_err("Error at socket creation.");
    }

    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);

    if (inet_pton(AF_INET, argv[1], &(server_addr.sin_addr)) <= 0) {
        raise_err("inet_pton error at %s", argv[1]);
    }

    if (connect(socket_fd, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0) {
        raise_err("Connection failed.");
    }

    sprintf(send_line, "GET / HTTP/1.1\r\n\r\n");
    send_bytes = strlen(send_line);

    if (send(socket_fd, send_line, send_bytes, 0) != send_bytes) {
        raise_err("Write error.");
    }

    while (1) {
        while (read_line(socket_fd, recv_line)) {
            fprintf(recv_header, "%s", recv_line);
        }
        if (!strcmp(recv_line, "\r\n")) {
            break;
        }
        fprintf(recv_header, "%s", recv_line);
    }

    fclose(recv_header);

    // Just for Transfer-Encoding: chunked

    while (1) {
        read_line(socket_fd, recv_line);
        n = strtol(recv_line, NULL, 16);
        if (n > 0) {
            while (read_nbytes(socket_fd, recv_line, &n)) {
                fprintf(recv_content, "%s", recv_line);
            }
            fprintf(recv_content, "%s", recv_line);
            // read until CRLF and discard
            while (read_line(socket_fd, recv_line));
        } else if (n < 0) {
            raise_err("Read error.");
        } else {
            // read a line and discard it
            while (read_line(socket_fd, recv_line));
            break;
        }
    }

    fclose(recv_content);

    exit(0);
}
