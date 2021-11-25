cmake_minimum_required(VERSION 3.20)
add_library(mhc_linux STATIC private/linux/mh_context.t.h private/linux/mh_task.t.h source/linux/mh_context.c source/linux/mh_task.c)
target_link_libraries(mhc_linux PUBLIC mhc_generic)
target_include_directories(mhc_linux PRIVATE private/)
