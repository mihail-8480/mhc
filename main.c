#define MH_MAIN_ASYNC
#include <mh_async.h>
#include <stdio.h>

void test(Task self) {
    mh_task_yield(self);
}

void main_async(Task self) {
    mh_task_dereference(mh_task_new((mh_task_start_t) {
        .scheduler = mh_task_get_scheduler(self),
        .func = test,
        .stack_size = MH_DEFAULT_STACK_SIZE,
        .arguments = NULL
    }));

    mh_task_yield(self);
}