#ifndef CASYNC_MH_ASYNC_H
#define CASYNC_MH_ASYNC_H

#include "mh_task.h"

// Create a scheduler that calls the async_main declaration if MH_MAIN_ASYNC is defined, it will define a main() function.
#ifdef MH_MAIN_ASYNC

// Set the default main stack size to MH_DEFAULT_STACK_SIZE if it's not set.
#ifndef MH_MAIN_STACK_SIZE
#define MH_MAIN_STACK_SIZE MH_DEFAULT_STACK_SIZE
#endif

MH_EXPORT void main_async(Task self);

typedef struct main_args {
    int argc;
    String *argv;
} *MH_UNUSED MainArgs, main_args_t;

// The entry point of the program.
int main(int argc, String argv[]) {
    // Create the arguments object.
    main_args_t args = {
      .argc = argc,
      .argv = argv
    };

    // Create the task scheduler.
    TaskScheduler scheduler = mh_task_sched_new();

    // Create the main task.
    var task = mh_task_new((mh_task_start_t) {
            .scheduler = scheduler,
            .stack_size = MH_MAIN_STACK_SIZE,
            .arguments = &args,
            .func = main_async,
    });

    // Run the scheduler.
    mh_task_sched_run(scheduler);

    // Cleanup.
    mh_task_sched_destroy(scheduler);

    // Get the return value (if there is one) and error code.
    Any value = 0;
    int code = mh_task_get_value(task, &value);

    // Dereference (hopefully also free) the main task.
    mh_task_dereference(task);

    // Return the error code.
    return code;
}

#endif

#endif //CASYNC_MH_ASYNC_H
