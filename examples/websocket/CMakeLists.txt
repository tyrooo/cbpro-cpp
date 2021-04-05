cmake_minimum_required(VERSION 3.15)
project(websocket_ex)
set(CMAKE_CXX_STANDARD 17)

find_package(cbpro++ REQUIRED)

add_executable(websocket_ex main.cpp)
