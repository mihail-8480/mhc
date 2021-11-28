// todo: split current tasks into ValueTask, FunctionTask, ExternalTask
// todo: add a function for resuming an awaited task externally (using an indirect pointer to the task)
// todo: implement interfaces
// todo: make the scheduler not instantly close while it has running tasks
// todo: take the platform specific parts out of <mh_task.h> by using interfaces
// todo: split <mh_task.h> into smaller parts (by making a <mh_sched.h>)
// todo: implement async IO using io_uring
