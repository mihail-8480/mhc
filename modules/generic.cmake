cmake_minimum_required(VERSION 3.20)
include(mhc.cmake)
add_library(mhc_generic STATIC private/generic/mh_alloc.h private/generic/mh_queue.t.h private/generic/mh_selftest.h source/generic/mh_queue.c source/generic/mh_assert.c)
mhc_module_declare(generic)
