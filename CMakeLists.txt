cmake_minimum_required(VERSION 3.15)
project(connect4_monte_carlo C)

set(CMAKE_C_STANDARD 11)

add_executable(connect4_monte_carlo main.c game.c game.h players.h tournament.h players.c tournament.c)