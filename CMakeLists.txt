cmake_minimum_required(VERSION 3.20)
project(
        mhc
        LANGUAGES C
        VERSION 0.0.0.1
        DESCRIPTION "A C convenience library."
)


set(CMAKE_C_FLAGS "-Wall -Werror")
set(CMAKE_C_FLAGS_DEBUG "-DDEBUG -ggdb3")
set(CMAKE_C_FLAGS_RELEASE "-DRELEASE -O3")
set(CMAKE_C_STANDARD 11)

add_library(mhc_base INTERFACE)
target_compile_definitions(mhc_base INTERFACE "MH_PROJECT_NAME=${CMAKE_PROJECT_NAME}")
target_compile_definitions(mhc_base INTERFACE "MH_PROJECT_VERSION_MAJOR=${CMAKE_PROJECT_VERSION_MAJOR}")
target_compile_definitions(mhc_base INTERFACE "MH_PROJECT_VERSION_MINOR=${CMAKE_PROJECT_VERSION_MINOR}")
target_compile_definitions(mhc_base INTERFACE "MH_PROJECT_VERSION_PATCH=${CMAKE_PROJECT_VERSION_PATCH}")
target_compile_definitions(mhc_base INTERFACE "MH_PROJECT_VERSION_TWEAK=${CMAKE_PROJECT_VERSION_TWEAK}")
target_compile_definitions(mhc_base INTERFACE "MH_PROJECT_DESCRIPTION=${CMAKE_PROJECT_DESCRIPTION}")

add_library(mhc SHARED source/mh.c include/mh.h include/mh_async.h include/mh_bool.h include/mh_context.h include/mh_queue.h include/mh_task.h)
target_compile_definitions(mhc PUBLIC "MH_LIBRARY")
target_compile_options(mhc PUBLIC "-fPIC")
target_link_libraries(mhc PRIVATE mhc_base)

function(mhc_feature name impl)
    string(TOUPPER ${name} name_upper)
    target_compile_definitions(mhc_base INTERFACE "MH_${name_upper}_IMPL=\"${impl}\"")
endfunction()

function(mhc_generic)
    add_library(mhc_generic STATIC private/generic/mh_alloc.h private/generic/mh_queue.t.h private/generic/mh_selftest.h source/generic/mh_queue.c source/generic/mh_assert.c)
    target_compile_options(mhc_generic PUBLIC "-fPIC")
    target_link_libraries(mhc_generic PRIVATE mhc_base)
    target_link_libraries(mhc_base INTERFACE mhc_generic)
    target_include_directories(mhc_generic PUBLIC include/)
    target_include_directories(mhc_generic PRIVATE private/)
    target_link_libraries(mhc PUBLIC mhc_generic)
    target_compile_definitions(mhc_base INTERFACE "MH_MODULE_GENERIC")
endfunction()

function(mhc_ucontext)
    add_library(mhc_ucontext STATIC private/ucontext/mh_context.t.h source/ucontext/mh_context.c)
    target_compile_options(mhc_ucontext PUBLIC "-fPIC")
    target_link_libraries(mhc_ucontext PRIVATE mhc_base)
    target_link_libraries(mhc_base INTERFACE mhc_ucontext)
    target_include_directories(mhc_ucontext PUBLIC include/)
    target_include_directories(mhc_ucontext PRIVATE private/)
    target_compile_definitions(mhc_base INTERFACE "MH_MODULE_UCONTEXT")
    mhc_feature("context" "ucontext/mh_context.t.h")
endfunction()

function(mhc_tasks)
    add_library(mhc_tasks STATIC private/tasks/mh_task.t.h source/tasks/mh_task.c)
    target_compile_options(mhc_tasks PUBLIC "-fPIC")
    target_link_libraries(mhc_tasks PRIVATE mhc_base)
    target_link_libraries(mhc_base INTERFACE mhc_tasks)
    target_include_directories(mhc_tasks PUBLIC include/)
    target_include_directories(mhc_tasks PRIVATE private/)
    target_compile_definitions(mhc_base INTERFACE "MH_MODULE_TASKS")
    mhc_feature("tasks" "tasks/mh_task.t.h")
endfunction()

mhc_generic()
mhc_tasks()
mhc_ucontext()

add_executable(casync main.c todo.c)
target_link_libraries(casync PRIVATE mhc)