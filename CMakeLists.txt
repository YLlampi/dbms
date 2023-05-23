cmake_minimum_required(VERSION 3.25)
project(dbms)

set(CMAKE_CXX_STANDARD 17)

add_executable(dbms main.cpp TextTable.h Disco.h)
