cmake_minimum_required(VERSION 3.20)
include(mhc.cmake)
add_library(mhc_ucontext STATIC private/ucontext/mh_context.t.h source/ucontext/mh_context.c)
mhc_module_declare(ucontext)
mhc_feature(context ucontext/mh_context.t.h)