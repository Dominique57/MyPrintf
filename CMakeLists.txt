cmake_minimum_required(VERSION 3.10)
project(printf C)

set(CMAKE_C_STANDARD 99)

add_executable(printf main.c printf.c toString.c my_parser.c)