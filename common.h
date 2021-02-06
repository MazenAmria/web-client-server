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

void raise_err(const char *fmt, ...);

int8_t read_line(int32_t fd, char *buffer);

int8_t read_nbytes(int32_t fd, char *buffer, int32_t *n);

#endif //WEB_SERVER_COMMON_H
