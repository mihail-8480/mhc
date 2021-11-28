#ifndef CASYNC_MH_TASK_T_H
#define CASYNC_MH_TASK_T_H
#include <mh_task.h>
#include "mh_context.t.h"
#include "generic/mh_queue.t.h"

typedef void mh_task_finalizer_func(Task task, Any data);
typedef struct mh_task_finalizer mh_task_finalizer_t, *TaskFinalizer;

struct mh_task_finalizer {
    mh_task_finalizer_func *func;
    Any data;
    mh_bool_t data_is_task;
    mh_bool_t should_free;
    struct mh_task_finalizer *next;
};

struct mh_task {
    mh_context_t context;
    mh_task_status_t status;
    ContextStack stack;
    struct {
        mh_task_result_status_t status;
        Any value;
    } result;
    mh_task_start_t start;
    mh_task_finalizer_t awaiter;
    mh_task_finalizer_t *finalizer;
    _Atomic(count_t) references;
};

typedef struct mh_task_sched_finalizer_context {
    mh_context_t context;
    ContextStack stack;
    TaskScheduler parent;
} mh_task_sched_finalizer_context_t, *TaskSchedulerFinalizerContext;

struct mh_task_sched {
    mh_context_t context;
    Queue task_queue;
    Task current_task;
    mh_task_sched_finalizer_context_t finalizer;
};

#endif //CASYNC_MH_TASK_T_H
