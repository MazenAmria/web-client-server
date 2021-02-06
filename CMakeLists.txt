cmake_minimum_required(VERSION 3.17)
project(web-client-server C)

set(CMAKE_C_STANDARD 11)

add_executable(web_client tcp_client.c common.h)
add_executable(web_server tcp_server.c common.h)