#include <linux/mh_task.t.h>
#include <generic/mh_queue.t.h>
#include <generic/mh_alloc.h>

void mh_task_set_result(Task task, mh_task_result_status_t status, Any value) {
    mh_assert(task != MH_NULL);
    task->result.status = status;
    task->result.value = value;
}

void mh_task_complete(Task task, mh_bool_t success) {
    mh_assert(task != MH_NULL);
    task->status = success ? MH_TASK_COMPLETE : MH_TASK_REJECTED;
    mh_context_switch(&task->context, &task->start.scheduler->context);
}

mh_never mh_task_sched_finalizer(TaskSchedulerFinalizerContext context) {
    mh_assert(context != MH_NULL);
    TaskScheduler scheduler = context->parent;
    mh_assert(scheduler != MH_NULL);
    while(MH_TRUE) {
        Task task = scheduler->current_task;
        mh_assert(task != MH_NULL);
        mh_task_set_result(task, MH_TASK_RESULT_COMPLETE, MH_NULL);
        task->status = MH_TASK_COMPLETE;
        mh_task_complete(task, MH_TRUE);
    }
}

TaskScheduler mh_task_sched_new(void) {
    TaskScheduler sched = mh_calloc(1, sizeof(mh_task_sched_t));
    mh_assert(sched != MH_NULL);

    sched->task_queue = mh_queue_new();
    sched->finalizer.parent = sched;
    sched->finalizer.stack = mh_context_stack_new(MH_DEFAULT_STACK_SIZE);

    mh_context_create(&sched->finalizer.context, sched->finalizer.stack, (FContextStart) mh_task_sched_finalizer, &sched->context);
    return sched;
}

void mh_task_sched_destroy(TaskScheduler sched) {
    mh_assert(sched != MH_NULL);
    mh_queue_destroy(sched->task_queue);
    mh_context_stack_destroy(sched->finalizer.stack);
    mh_free(sched);
}

void mh_task_dereference(Task task) {
    task->references--;
    if (task->references <= 0) {
        mh_context_stack_destroy(task->stack);
        mh_free(task);
    }
}

void mh_task_yield(Task task) {
    mh_context_switch(&task->context, &task->start.scheduler->context);
}

Any mh_task_await(Task task, Task next) {
    mh_task_reference(next);
    if (next->status != MH_TASK_COMPLETE && next->status != MH_TASK_REJECTED) {
        task->awaiter.next = next->finalizer;
        next->finalizer = &task->awaiter;
        task->status = MH_TASK_AWAITED;
        mh_task_yield(task);
    }
    Any value = 0;
    var status = mh_task_get_value(next, &value);
    mh_assert(status == MH_TASK_VALUE_NO_ERROR || status == MH_TASK_VALUE_IS_REJECTED);
    if (status == MH_TASK_VALUE_IS_REJECTED) {
        mh_task_reject(task, value); // add a way to indicate that it was rejected
    }
    mh_task_dereference(next);
    return value;
}

Task mh_task_new(mh_task_start_t start) {
    var stack = mh_context_stack_new(start.stack_size);
    Task task = mh_calloc(1,sizeof(mh_task_t));
    task->status = MH_TASK_PENDING;
    task->references = 2;
    task->start = start;
    task->finalizer = MH_NULL;
    task->awaiter.next = MH_NULL;
    task->awaiter.should_free = MH_FALSE;
    task->awaiter.data = task;
    task->awaiter.data_is_task = MH_TRUE;
    task->awaiter.func = MH_NULL;
    task->stack = stack;
    task->result.status = MH_TASK_RESULT_PENDING;
    task->result.value = MH_NULL;
    task->queue_handle.next = MH_NULL;
    task->queue_handle.data = MH_NULL;
    mh_context_create(&task->context, stack, (FContextStart) start.func, &start.scheduler->finalizer.context);
    mh_queue(start.scheduler->task_queue, task, &task->queue_handle);
    return task;
}

void mh_task_reference(Task task) {
    mh_assert(task != MH_NULL);
    task->references++;
}

void mh_task_sched_run(TaskScheduler scheduler) {
#define c_task (scheduler->current_task)
    while((c_task = mh_queue_take(scheduler->task_queue)) != MH_NULL) {
        mh_context_switch(&scheduler->context, &c_task->context);
        if (c_task->status == MH_TASK_AWAITED) {
            continue;
        }
        if (c_task->status != MH_TASK_COMPLETE && c_task->status != MH_TASK_REJECTED) {
            mh_queue(scheduler->task_queue, c_task, &c_task->queue_handle);
        } else {
            while (c_task->finalizer != MH_NULL) {
                if (c_task->finalizer->data_is_task) {
                    mh_queue(scheduler->task_queue, c_task->finalizer->data, &c_task->queue_handle);
                    ((Task)c_task->finalizer->data)->status = MH_TASK_PENDING;
                }
                if (c_task->finalizer->func != MH_NULL) {
                    c_task->finalizer->func(c_task, c_task->finalizer->data);
                }
                var next = c_task->finalizer->next;
                if (c_task->finalizer->should_free) {
                    mh_free(c_task->finalizer);
                }
                c_task->finalizer = next;
            }
            mh_task_dereference(c_task);
        }
    }
#undef c_task
}

void mh_task_resolve(Task task, Any value) {
    mh_assert(task != MH_NULL);
    mh_task_set_result(task, MH_TASK_RESULT_COMPLETE, value);
    mh_task_complete(task, MH_TRUE);
}

void mh_task_reject(Task task, Any value) {
    mh_assert(task != MH_NULL);
    mh_task_set_result(task, MH_TASK_RESULT_COMPLETE, value);
    mh_task_complete(task, MH_FALSE);
}

mh_task_value_error_t mh_task_get_value(Task task, Any *value) {
    mh_assert(task != MH_NULL);
    mh_assert(value != MH_NULL);
    if (task->status == MH_TASK_REJECTED) {
        *value = task->result.value;
        return MH_TASK_VALUE_IS_REJECTED;
    }
    switch (task->result.status) {
        case MH_TASK_RESULT_COMPLETE:
            *value = task->result.value;
            return MH_TASK_VALUE_NO_ERROR;

        case MH_TASK_RESULT_PENDING:
            return MH_TASK_VALUE_IS_PENDING;
        /*case MH_TASK_RESULT_PARTIAL:
            return MH_TASK_VALUE_IS_PARTIAL;*/
        default:
            return MH_TASK_VALUE_UNKNOWN_ERROR;
    }
}

Any mh_task_get_arguments(Task task) {
    mh_assert(task != MH_NULL);
    return task->start.arguments;
}

TaskScheduler mh_task_get_scheduler(Task task) {
    mh_assert(task != MH_NULL);
    return task->start.scheduler;
}

void mh_task_free(MH_UNUSED Task task, Any data) {
    mh_free(data);
}

Any mh_task_allocate(Task task, count_t size) {
    mh_assert(task != MH_NULL);
    Any block = mh_calloc(1, size);
    TaskFinalizer finalizer = mh_calloc(1, sizeof(mh_task_finalizer_t));
    finalizer->should_free = MH_TRUE;
    finalizer->next = task->finalizer;
    finalizer->data = block;
    finalizer->data_is_task = MH_FALSE;
    finalizer->func = mh_task_free;
    task->finalizer = finalizer;
    return block;
}