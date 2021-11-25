# libmhc

A C convenience library.

## Naming
All code must follow this naming convention: 

| Type | Naming |
| ---- | ------ |
| `struct` | `namespace_snake_case`
| `typedef struct,enum,union` | `namespace_snake_case_t`
| `typedef struct,enum,union *` | `CamelCase`
| `function` | `namespace_snake_case`
| `typedef function` | `namespace_snake_case_func`
| `typedef function *` | `FCamelCase`
| `constant` | `NAMESPACE_UPPER_CASE_UNDERSCORE`

## Public APIs
An addition to the API might happen at any time, a change or removal will happen only on major version changes.

### Module `mhc`
* The code is added directly to `libmhc` without creating an actual module.
* Defines `MH_LIBRARY` to all executables that link to it with `cmake` and for itself.
* Doesn't define `MH_LIBRARY` for the modules it includes.

#### `<mh.h>`

* Includes `<mh_bool.h>`.

| Name | Type | Value | Description |
|------|------| ----- | ----------- |
| `Any` | Typedef | `void *` | A reference to anything.
| `String` | Typedef | `const char *` | A readonly character array.
| `count_t` | Typedef | `unsigned long long` | A big enough number to hold a memory address (hopefully).
| `MH_EXPORT` | Macro (attribute) | Code that marks a function as external and maybe unused. | A function that is a part of the public API.
| `MH_UNUSED` | Macro (attribute) | Code that marks something as maybe unused. | Something that is not being used but needs to be there.
| `mh_never` | Macro (type) | Code that says that a function will never return. | I guarantee that this function will never return.
| `var` | Macro (type) | Use type inference for the variable. | `__auto_type` is too long to type, so we are going to use var instead.
| `MH_NULL` | Macro (value) | `((Any)0)` | A reference to nothing.

* When `MH_LIBRARY` is defined, it has the additional:

| Name | Type | Value | Description |
|------|------| ----- | ----------- |
| `mh_library_version` | Struct | Members that represent the `cmake` project information. | The library version.
| `mh_library_version_t` | Typedef | `struct mh_library_version` | 
| `LibraryVersion` | Typedef | `struct mh_library_version * const` | 
| `LibraryVersion mh_get_version(void)` | Function | Implementation in `source/mh.c`. | Get library version.

#### `<mh_bool.h>`
| Name | Type | Value | Description |
|------|------| ----- | ----------- |
| `mh_bool` | Enum | `MH_TRUE` and `MH_FALSE` as options. | Can be "true" (MH_TRUE) or "false" (MH_FALSE).
| `mh_bool_t` | Typedef | `enum mh_bool`
| `MH_TRUE` | `mh_bool` Value | `1` | = 1
| `MH_FALSE` | `mh_bool` Value | `0` | = 0

* Also declares a valid implementation of `<stdbool.h>` if it's not included.

### Module `generic`

* If `libmhc` is linked to a project using `cmake` and this module is included - `MH_MODULE_GENERIC` is going to be defined.
* Contains platform independent code.

#### `<mh_queue.h>`

* Includes `<mh.h>`.

| Name | Type | Value | Description |
|------|------| ----- | ----------- |
| `mh_queue` | Struct | Anonymous struct. | A datatype that holds elements and returns them first-in-first-out. Our implementation does only one allocation for the queue object.
| `mh_queue_t` | Typedef | `struct mh_queue` |
| `Queue` | Typedef | `struct mh_queue *` |
| `void mh_queue(Queue queue, Any data, Any handle)` | Function | Implementation in `source/generic/mh_queue.c`. | Put something in the queue (the element gets added). The handle is a block of memory that gets used as the container. It needs to be big enough to contain two pointers.
| `Any mh_queue_take(Queue queue)` | Function | Implementation in `source/generic/mh_queue.c`. | Take something from the queue (the element gets removed).
| `Queue mh_queue_new(void)` | Function | Implementation in `source/generic/mh_queue.c` | Create a new queue (must be destroyed with mh_queue_destroy).
| `void mh_queue_destroy(Queue queue)` | Function | Implementation in `source/generic/mh_queue.c` | Destroy a queue.

### Module `linux`

* If `libmhc` is linked to a project using `cmake` and this module is included - `MH_MODULE_LINUX` is going to be defined.
* Contains Linux-dependent code.

#### `<mh_context.h>`

* Includes `<mh.h>`.

| Name | Type | Value | Description |
|------|------| ----- | ----------- |
| `mh_context` | Struct | Anonymous struct. | The program's state at a certain point in time.
| `mh_context_t` | Typedef | `struct mh_context` 
| `Context` | Typedef | `struct mh_context *`
| `mh_context_stack` | Struct | Anonymous struct. | The stack that gets used by the context when it's running.
| `mh_context_stack_t` | Typedef | `struct mh_context_stack`
| `ContextStack` | Typedef | `struct mh_context_stack *`
| `mh_context_start_func` | Typedef | `(void)(Context context)` | The context's first function.
| `FContextStart` | Typedef | `mh_context_start_func *` | A reference to the context's first function to be called.
| `void mh_context_save(Context context)` | Function | Implementation in `source/linux/mh_context.c` | Save the current program's state to an object.
| `Context mh_context_new(void)` | Function | Implementation in `source/linux/mh_context.c` | Allocate a new empty context.
| `void mh_context_destroy(Context context)` | Function | Implementation in `source/linux/mh_context.c` | Free the memory associated with a context.
| `void mh_context_create(Context context, ContextStack stack, FContextStart func, Context finalize)` | Function | Implementation in `source/linux/mh_context.c` | Create a new context that can be switched to.
| `void mh_context_switch(Context current, Context new)` | Function | Implementation in `source/linux/mh_context.c` | Save the current program state, and switch to the other previously saved program state.
| `void mh_context_restore(Context context)` | Function | Implementation in `source/linux/mh_context.c` | Restore the program's state from an object.
| `ContextStack mh_context_stack_new(count_t size)` | Function | Implementation in `source/linux/mh_context.c` | Create a new stack to be used by a context.
| `void mh_context_stack_destroy(ContextStack stack)` | Function | Implementation in `source/linux/mh_context.c` | Free the memory used by the stack.
| `MH_DEFAULT_STACK_SIZE` | Macro | `4096` | The default stack size.

#### `<mh_task.h>`

* Includes `<mh.h>`.

| Name | Type | Value | Description |
|------|------| ----- | ----------- |
| `mh_task` | Struct | Anonymous struct. | A task is a function that could be executed at any point in time in the future.
| `mh_task_t` | Typedef | `struct mh_task` 
| `Task` | Typedef | `struct mh_task *` 
| `mh_task_sched` | Struct | Anonymous struct. | The task scheduler is the part of the program that executes tasks.
| `mh_task_sched_t` | Typedef | `struct mh_task_sched` 
| `TaskScheduler` | Typedef | `struct mh_task_sched *` 
| `task_start_func` | Typedef | `(void)(Task task)` | The task start function signature.
| `FTaskStart` | Typedef | `task_start_func` | The task start function.
| `mh_task_status` | Enum | `MH_TASK_PENDING`, `MH_TASK_COMPLETE`, `MH_TASK_REJECTED` or `MH_TASK_AWAITED` | The task's status.
| `mh_task_status_t` | Typedef | `enum mh_task_status` | 
| `MH_TASK_PENDING` | `mh_task_status` Value | `0` | The task is incomplete.
| `MH_TASK_COMPLETE` | `mh_task_status` Value | `1` | The task has been completed successfully.
| `MH_TASK_REJECTED` | `mh_task_status` Value | `2` | The task has failed.
| `MH_TASK_AWAITED` | `mh_task_status` Value | `3` | The task currently waits for another task to complete.
| `mh_task_result_status` | Enum | `MH_TASK_RESULT_PENDING` or `MH_TASK_RESULT_COMPLETE` | The task's result status.
| `mh_task_result_status_t` | Typedef | `enum mh_task_result_status`
| `MH_TASK_RESULT_PENDING` | `mh_task_result_status` Value | `0` | The result isn't there yet.
| `MH_TASK_RESULT_COMPLETE` | `mh_task_result_status` Value | `2` | The task's result is available.
| `mh_task_value_error` | Enum | `MH_TASK_VALUE_UNKNOWN_ERROR`, `MH_TASK_VALUE_NO_ERROR`, `MH_TASK_VALUE_IS_REJECTED`, `MH_TASK_VALUE_IS_PENDING` | The error codes that get returned by mh_task_get_value().
| `mh_task_value_error_t` | Typedef | `enum mh_task_value_error`
| `MH_TASK_VALUE_UNKNOWN_ERROR` | `mh_task_value_error` Value | `-1` | We don't understand what happened.
| `MH_TASK_VALUE_NO_ERROR` | `mh_task_value_error` Value | `0` | There isn't an error, the value has been set.
| `MH_TASK_VALUE_IS_REJECTED` | `mh_task_value_error` Value | `1` | The task failed, the value has been set to the error message.
| `MH_TASK_VALUE_IS_PENDING` |  `mh_task_value_error` Value | `3` | The value isn't there yet, you should probably call mh_task_await() instead.
| `mh_task_start` | Struct | Information that is used to start a task. | A structure that represents how the task should behave.
| `mh_task_start_t` | Typedef | `struct mh_task_start` 
| `void mh_task_yield(Task task)` | Function | Implementation in `source/linux/mh_task.c`. | (Potentially) Give control to another task.
| `TaskScheduler mh_task_sched_new(void)` | Function | Implementation in `source/linux/mh_task.c`. | Create a new task scheduler.
| `void mh_task_sched_destroy(TaskScheduler sched)` | Function | Implementation in `source/linux/mh_task.c`. | Destroy a task scheduler.
| `void mh_task_dereference(Task task)` | Function | Implementation in `source/linux/mh_task.c`. | Dereference a task (this might free the resources used by the task).
| `Task mh_task_new(mh_task_start_t start)` | Function | Implementation in `source/linux/mh_task.c`. | Create a new task. Its reference count is set to 2. It gets decremented once by the scheduler when it finishes or fails. It should get decremented once by the caller by calling mh_task_dereference().
| `void mh_task_reference(Task task)` | Function | Implementation in `source/linux/mh_task.c`. | Reference a task (prevent the task's resources from being freed.)
| `Any mh_task_allocate(Task task, count_t size)` | Function | Implementation in `source/linux/mh_task.c`. | Allocate memory for a task, that gets automatically freed when the task completes.
| `void mh_task_sched_run(TaskScheduler scheduler)` | Function | Implementation in `source/linux/mh_task.c`. | Run the scheduler on this thread, this function might not return.
| `void mh_task_resolve(Task task, Any value)` | Function | Implementation in `source/linux/mh_task.c`. | This sets the complete status of the task to MH_TASK_COMPLETE, and sets the return value. This function gives control to another task and permanently finishes the current one.
| `void mh_task_reject(Task task, Any value)` | Function | Implementation in `source/linux/mh_task.c`. | This sets the complete status of the task to MH_TASK_REJECTED, and sets the return value. This function gives control to another task and permanently finishes the current one.
| `mh_task_value_error_t mh_task_get_value(Task task, Any *value)` | Function | Implementation in `source/linux/mh_task.c`. | Get the return value of a task (see mh_task_await() for a better way).
| `Any mh_task_get_arguments(Task task)` | Function | Implementation in `source/linux/mh_task.c`. | Get the arguments passed to the task.
| `TaskScheduler mh_task_get_scheduler(Task task)` | Function | Implementation in `source/linux/mh_task.c`. | Get the task's scheduler.
| `Any mh_task_await(Task task, Task next)` | Function | Implementation in `source/linux/mh_task.c`. | "Wait" for another task's completion and return it's return value.  This function gives up control and removes the task from the queue until the other task completes.

#### `<mh_async.h>`

* Includes `<mh_task.h>` and `<mh_context.h>`.
* If `MH_MAIN_ASYNC` isn't defined, the "include" doesn't do anything.
* If `MH_MAIN_STACK_SIZE` isn't defined it's set to `MH_DEFAULT_STACK_SIZE`.

| Name | Type | Value | Description |
|------|------| ----- | ----------- |
| `main_args` | Struct | `argc` and `argv`. | Should be obtained using mh_task_get_arguments().
| `main_args_t` | Typedef | `struct main_args`
| `MainArgs` | Typedef | `struct main_args *`
| `void main_async(Task self)` | Function | Should be implemented by the user. | The new entry point of the program.
| `int main(int argc, String argv[])` | Function | Starts a new scheduler and creates a task based on `main_async` with a stack size of `MH_MAIN_STACK_SIZE`. | The entry point of the program.