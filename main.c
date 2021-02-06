#include <stdint.h>
#include <string.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <stdarg.h>

#define SERVER_PORT 80
#define MAX_LINE 4096

void raise_err (const char *fmt, ...);

int32_t main (int32_t argc, char ** argv) {
    int32_t socket_fd;
    int32_t send_bytes, n;
    char send_line[MAX_LINE];
    char recv_line[MAX_LINE];
    struct sockaddr_in server_addr;

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

    if (connect(socket_fd, (const struct sockaddr *) &server_addr, sizeof(server_addr)) < 0) {
        raise_err("Connection failed.");
    }

    sprintf(send_line, "GET / HTTP/1.1\r\n\r\n");
    send_bytes = strlen(send_line);

    if (write(socket_fd, send_line, send_bytes) != send_bytes) {
        raise_err("Write error.");
    }

    memset(recv_line, 0, MAX_LINE);

    while ((n = read(socket_fd, recv_line, MAX_LINE)) > 0) {
        printf("%s", recv_line);
    }
    if (n < 0) {
        raise_err("Read error.");
    }

    exit(0);
}

void raise_err(const char *fmt, ...) {
    int32_t err_no;
    va_list ap;

    err_no = errno;

    va_start(ap, fmt);
    vfprintf(stderr, fmt, ap);
    fprintf(stderr, "\n");
    fflush(stderr);

    if (err_no != 0) {
        fprintf(stderr, "(errno = %d) : %s\n", err_no, strerror(err_no));
        fflush(stderr);
    }

    va_end(ap);

    exit(1);
}