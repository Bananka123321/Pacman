cmake_minimum_required(VERSION 3.31.5)
project(Pacman VERSION 0.1.0 LANGUAGES C CXX)

add_executable(Pacman main.cpp src/Controller.cpp src/Game.cpp src/Maps.cpp)