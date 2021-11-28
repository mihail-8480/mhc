#ifndef CASYNC_MH_ALLOC_H
#define CASYNC_MH_ALLOC_H

#include MH_SELFTEST_IMPL
#include <stdlib.h>

#define mh_calloc(...) mh_wrap(calloc, MH_TRUE, mh_wrap_return != MH_NULL, __VA_ARGS__)
#define mh_free(...) mh_wrap_void(free, __VA_ARGS__ != MH_NULL, MH_TRUE, __VA_ARGS__)
#define mh_malloc(...) mh_wrap(malloc, MH_TRUE, mh_wrap_return != MH_NULL, __VA_ARGS__)

#endif //CASYNC_MH_ALLOC_H
