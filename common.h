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

#define SERVER_PORT 80
#define MAX_LINE 4096

void raise_err (const char *fmt, ...);

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

#endif //WEB_SERVER_COMMON_H
