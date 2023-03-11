cmake_minimum_required(VERSION 3.22)
project(Clex)

set(CMAKE_CXX_STANDARD 14)

add_executable(Clex
        src/main.cpp src/trie.h src/token.h src/token.cpp)
