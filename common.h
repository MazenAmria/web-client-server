//
// Created by Mazen Amria on 06/02/2021.
//

#ifndef WEB_SERVER_COMMON_H
#define WEB_SERVER_COMMON_H

#include <stdint.h>
#include <string.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <stdarg.h>
#include <inttypes.h>

#define SERVER_PORT 80
#define MAX_LINE 4096

void raise_err (const char *fmt, ...);
int8_t read_line (int32_t fd, char * buffer);

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

int8_t read_line (int32_t fd, char * buffer) {
    int32_t ptr = 0, n;
    memset(buffer, 0, MAX_LINE);
    while (ptr < MAX_LINE) {
        n = read(fd, buffer + ptr, 1);
        if (n < 0) {
            raise_err("Read error.");
        }
        ptr++;
        if (n == 0) return 0;
        if (ptr >= 2 && !strcmp(buffer + ptr - 2, "\r\n")) {
            return 0;
        }
    }
    return -1;
}

int8_t read_nbytes (int32_t fd, char * buffer, int32_t * n) {
    int32_t _n;
    memset(buffer, 0, MAX_LINE);

    _n = read(fd, buffer, (MAX_LINE > *n) ? *n : MAX_LINE);

    if (_n < 0) {
        raise_err("Read error.");
    }

    if (_n == *n) {
        *n = 0;
        return 0;
    }

    *n -= _n;
    return -1;
}

#endif //WEB_SERVER_COMMON_H
