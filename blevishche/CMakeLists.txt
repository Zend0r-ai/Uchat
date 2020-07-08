cmake_minimum_required(VERSION 3.16)
project(Uchat)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)
include_directories(client)
include_directories(client/inc)
include_directories(client/src)
include_directories(server)
include_directories(server/inc)
include_directories(server/src)

add_executable(Uchat
        client/inc/client.h
        client/a.out
        client/Makefile
        server/inc/server.h
        server/src/main.c
        server/Makefile
        server/uchat_server)
