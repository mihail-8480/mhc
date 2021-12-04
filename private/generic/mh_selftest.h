#ifndef CASYNC_MH_SELFTEST_H
#define CASYNC_MH_SELFTEST_H

#include <mh.h>

#ifndef MH_WINDOWS
#define MH_RESET_COLOR "\033[0m"
#define MH_DEBUG_COLOR "\033[1;33m"
#define MH_ASSERT_FAIL_COLOR "\033[1;31m"
#else
#define MH_RESET_COLOR ""
#define MH_DEBUG_COLOR ""
#define MH_ASSERT_FAIL_COLOR ""
#endif

#ifdef DEBUG

// A function that gets called on a failed assertion.
MH_EXPORT void mh_assert_fail(String assertion, String file, count_t line, String function);

// Confirm that something is true, if not, panic (while debugging).
#define mh_assert(expr) ({\
    if (!(expr)) {        \
    mh_assert_fail(#expr, __FILE__, __LINE__, __FUNCTION__);\
}})


MH_EXPORT void mh_debug_log(String message, Any object, String file, count_t line, String function);
#define mh_debug(__message__, __object__) mh_debug_log(__message__, (Any)(__object__), __FILE__, __LINE__, __FUNCTION__)

#else
// Ignore the assertion.
#define mh_assert(expr) ((void)(0))
#define mh_debug(__message__, __object__) ((void)(0))
#endif


#define mh_wrap_return _ret

#define mh_wrap(func, test_before, test_after, ...) ({\
    mh_assert(test_before);                           \
    var mh_wrap_return = func(__VA_ARGS__);           \
    mh_assert(test_after);                            \
    mh_wrap_return;                                   \
})

#define mh_wrap_void(func, test_before, test_after, ...) ({\
    mh_assert(test_before);                                \
    func(__VA_ARGS__);                                     \
    mh_assert(test_after);                                 \
})


#endif //CASYNC_MH_SELFTEST_H
