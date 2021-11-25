#ifndef CASYNC_MH_CONTEXT_T_H
#define CASYNC_MH_CONTEXT_T_H
#include <mh.h>
#include <mh_context.h>
#include <ucontext.h>

struct mh_context_stack {
    count_t size;
    char stack[MH_DEFAULT_STACK_SIZE];
};

struct mh_context {
    ucontext_t context;
};

#endif //CASYNC_MH_CONTEXT_T_H
