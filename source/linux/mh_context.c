#include <linux/mh_context.t.h>
#include <generic/mh_alloc.h>
#if DEBUG
#include <valgrind/valgrind.h>
#endif
void mh_context_stack_destroy(ContextStack stack) {
#if DEBUG
    VALGRIND_STACK_DEREGISTER(stack->stack);
#endif
    mh_assert(stack != MH_NULL);
    mh_free(stack);
    mh_debug("Stack was destroyed.", stack);
}

void mh_context_save(Context context) {
    mh_assert(context != MH_NULL);
    mh_debug("Context will be saved.", context);
    getcontext(&context->context);
}

Context mh_context_new(void) {
    Context context = mh_calloc(1, sizeof(mh_context_t));
    mh_debug("Context was allocated.", context);
    return context;
}

void mh_context_destroy(Context context) {
    mh_assert(context != MH_NULL);
    mh_free(context);
    mh_debug("Context was destroyed.", context);
}

void mh_context_create(Context context, ContextStack stack, FContextStart func, Context finalize) {
    mh_assert(context != MH_NULL);
    mh_assert(stack != MH_NULL);
    mh_assert(func != MH_NULL);
    mh_context_save(context);
    context->context.uc_stack.ss_size = stack->size;
    context->context.uc_stack.ss_sp = stack->stack;
    if (finalize != MH_NULL) {
        context->context.uc_link = &finalize->context;
    }
    makecontext(&context->context, (void (*)(void)) func, 1, context);
    mh_debug("Context was created.", context);
}

void mh_context_switch(Context current, Context new) {
    mh_assert(current != MH_NULL);
    mh_assert(new != MH_NULL);
    mh_debug("About to swap to an other context.", new);
    swapcontext(&current->context, &new->context);
}

void mh_context_restore(Context context) {
    mh_assert(context != MH_NULL);
    mh_debug("About to to restore a previously saved context.", context);
    setcontext(&context->context);
}

ContextStack mh_context_stack_new(count_t size) {
    ContextStack stack = mh_malloc((sizeof(mh_context_stack_t) - MH_DEFAULT_STACK_SIZE) + size);
#if DEBUG
    VALGRIND_STACK_REGISTER(stack->stack, stack->stack + size);
#endif
    stack->size = size;
    mh_debug("Created stack.", stack);
    return stack;
}


