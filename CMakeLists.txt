cmake_minimum_required(VERSION 3.28)
project(SO_server C)

set(CMAKE_C_STANDARD 11)
set(SOURCE_EXE main.c
        user.c user.h
        contact.c contact.h
        server.c server.h
        commands.c commands.h)
add_executable(SO_server ${SOURCE_EXE})