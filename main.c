#define MH_MAIN_ASYNC
#include <mh_async.h>
#include <stdio.h>

// todo: implement a utility function for starting tasks
// todo: add a function for resuming an awaited task externally (using an indirect pointer to the task)
// todo: implement interfaces
// todo: make the scheduler not instantly close while it has running tasks
// todo: take the platform specific parts out of <mh_task.h> by using interfaces
// todo: split <mh_task.h> into smaller parts (by making a <mh_sched.h>)
// todo: implement async IO using io_uring

void test(Task self) {
    printf("%s\n",(String) mh_task_get_arguments(self));
    mh_task_reject(self, "Lol");
}

void main_async(Task self) {
    var version = mh_get_version();
    printf("%s/%llu.%llu.%llu.%llu - %s\n",
           version->name,
           version->major,
           version->minor,
           version->patch,
           version->tweak,
           version->description);
    mh_task_start_t start = {
            .arguments = "Test",
            .stack_size = 4096,
            .func = test,
            .scheduler = mh_task_get_scheduler(self)
    };
    var task = mh_task_new(start);
    mh_task_dereference(task);
}