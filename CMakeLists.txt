cmake_minimum_required(VERSION 3.16)
project(ex5_shaharha1)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(ex5_shaharha1
        main.cpp
        Movie.cpp
        Movie.h
        User.cpp
        User.h
        RecommendationSystemLoader.cpp
        RecommendationSystemLoader.h
        RecommendationSystem.cpp
        RecommendationSystem.h
        UsersLoader.cpp
        UsersLoader.h)
