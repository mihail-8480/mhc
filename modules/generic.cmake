cmake_minimum_required(VERSION 3.20)
add_library(mhc_generic STATIC private/generic/mh_alloc.h private/generic/mh_queue.t.h private/generic/mh_selftest.h source/generic/mh_queue.c source/generic/mh_assert.c)
target_include_directories(mhc_generic PUBLIC include/)
target_include_directories(mhc_generic PRIVATE private/)
