#ifndef CASYNC_MH_CONTEXT_H
#define CASYNC_MH_CONTEXT_H
#include "mh.h"

// The program's state at a certain point in time.
typedef struct mh_context mh_context_t, *Context;

// The stack that gets used by the context when it's running.
typedef struct mh_context_stack mh_context_stack_t, *ContextStack;

// The context's first function.
typedef void mh_context_start_func(Context context);

// A reference to the context's first function to be called.
typedef mh_context_start_func *FContextStart;

// Save the current program's state to an object.
MH_EXPORT void mh_context_save(Context context);

// Allocate a new empty context.
MH_EXPORT Context mh_context_new(void);

// Free the memory associated with a context.
MH_EXPORT void mh_context_destroy(Context context);

// Create a new context that can be switched to.
MH_EXPORT void mh_context_create(Context context, ContextStack stack, FContextStart func, Context finalize);

// Save the current program state, and switch to the other previously saved program state.
MH_EXPORT void mh_context_switch(Context current, Context new);

// Restore the program's state from an object.
MH_EXPORT void mh_context_restore(Context context);

// Create a new stack to be used by a context.
MH_EXPORT ContextStack mh_context_stack_new(count_t size);

// Free the memory used by the stack.
MH_EXPORT void mh_context_stack_destroy(ContextStack stack);

// The default stack size.
#define MH_DEFAULT_STACK_SIZE 4096

#endif //CASYNC_MH_CONTEXT_H
