cmake_minimum_required(VERSION 3.27)
project(Board_games_framework)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(TicTacToe enhanced_framework.h TTT3x3.cpp)
