#ifndef CASYNC_MH_TASK_H
#define CASYNC_MH_TASK_H

#include "mh_context.h"

// A task is a function that could be executed at any point in time in the future.
typedef struct mh_task mh_task_t, *Task;

// The task scheduler is the part of the program that executes tasks.
typedef struct mh_task_sched mh_task_sched_t, *TaskScheduler;

// The task start function signature.
typedef void task_start_func(Task task);

// The task start function.
typedef task_start_func *FTaskStart;

// The task's status.
typedef enum mh_task_status {
    // The task is incomplete.
    MH_TASK_PENDING = 0,
    // The task has been completed successfully.
    MH_TASK_COMPLETE = 1,
    // The task has failed.
    MH_TASK_REJECTED = 2,
    // The task currently waits for another task to complete.
    MH_TASK_AWAITED = 3
} mh_task_status_t;

// The task's result status.
typedef enum mh_task_result_status {
    // The result isn't there yet.
    MH_TASK_RESULT_PENDING = 0,
    // The task's result is available.
    MH_TASK_RESULT_COMPLETE = 2
} mh_task_result_status_t;

// A structure that represents how the task should behave.
typedef struct mh_task_start {
    // The function that does the work for the task.
    FTaskStart func;
    // The arguments that should be passed to the task.
    Any arguments;
    // The scheduler that will execute the task.
    TaskScheduler scheduler;
    // The stack size of the task.
    count_t stack_size;
} mh_task_start_t;

// The error codes that get returned by mh_task_get_value().
typedef enum mh_task_value_error {
    // We don't understand what happened.
    MH_TASK_VALUE_UNKNOWN_ERROR = -1,
    // There isn't an error, the value has been set.
    MH_TASK_VALUE_NO_ERROR = 0,
    // The task failed, the value has been set to the error message.
    MH_TASK_VALUE_IS_REJECTED = 1,
    // The value isn't there yet, you should probably call mh_task_await() instead.
    MH_TASK_VALUE_IS_PENDING = 3
} mh_task_value_error_t;

// (Potentially) Give control to another task.
MH_EXPORT void mh_task_yield(Task task);

// Create a new task scheduler.
MH_EXPORT TaskScheduler mh_task_sched_new(void);

// Destroy a task scheduler.
MH_EXPORT void mh_task_sched_destroy(TaskScheduler sched);

// Dereference a task (this might free the resources used by the task.)
MH_EXPORT void mh_task_dereference(Task task);

// Create a new task.
// Its reference count is set to 2.
// It gets decremented once by the scheduler when it finishes or fails.
// It should get decremented once by the caller by calling mh_task_dereference().
MH_EXPORT Task mh_task_new(mh_task_start_t start);

// Reference a task (prevent the task's resources from being freed.)
MH_EXPORT void mh_task_reference(Task task);

// Allocate memory for a task, that gets automatically freed when the task completes.
MH_EXPORT Any mh_task_allocate(Task task, count_t size);

// Run the scheduler on this thread, this function might not return.
MH_EXPORT void mh_task_sched_run(TaskScheduler scheduler);

// This sets the complete status of the task to MH_TASK_COMPLETE, and sets the return value.
// This function gives control to another task and permanently finishes the current one.
MH_EXPORT void mh_task_resolve(Task task, Any value);

// This sets the complete status of the task to MH_TASK_REJECTED, and sets the return value.
// This function gives control to another task and permanently finishes the current one.
MH_EXPORT void mh_task_reject(Task task, Any value);

// Get the return value of a task (see mh_task_await() for a better way).
MH_EXPORT mh_task_value_error_t mh_task_get_value(Task task, Any *value);

// Get the arguments passed to the task.
MH_EXPORT Any mh_task_get_arguments(Task task);

// Get the task's scheduler.
MH_EXPORT TaskScheduler mh_task_get_scheduler(Task task);

// "Wait" for another task's completion and return it's return value.
// This function gives up control and removes the task from the queue until the other task completes.
MH_EXPORT Any mh_task_await(Task task, Task next);

#endif //CASYNC_MH_TASK_H
