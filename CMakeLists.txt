cmake_minimum_required(VERSION 3.20)
project(
        mhc
        LANGUAGES C
        VERSION 0.0.0.1
        DESCRIPTION "A C convenience library."
)

include(modules/mhc.cmake)

add_mhc_module("generic")
add_mhc_module("linux")

add_executable(casync main.c todo.c)
target_link_libraries(casync PRIVATE mhc)