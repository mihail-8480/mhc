cmake_minimum_required(VERSION 3.20)
project(
        mhc
        LANGUAGES C
        VERSION 0.0.0.1
        DESCRIPTION "A C convenience library."
)

include(base.cmake)
include(mhc.cmake)

mhc_enable(generic)
mhc_enable(ucontext)
mhc_enable(tasks)

add_executable(casync main.c todo.c)
target_link_libraries(casync PRIVATE mhc)