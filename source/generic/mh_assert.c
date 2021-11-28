#include <mh.h>
#include <stdlib.h>
#include <stdio.h>

#define MH_RESET_COLOR "\033[0m"
#define MH_DEBUG_COLOR "\033[1;33m"
#define MH_ASSERT_FAIL_COLOR "\033[1;31m"

void mh_assert_fail(String assertion, String file, count_t line, String function) {
    fprintf(stderr,MH_ASSERT_FAIL_COLOR "mh_assert_fail(): %s:%llu: %s(): Assertion `%s` failed.\n" MH_RESET_COLOR, file, line, function, assertion);
    fflush(stderr);
    abort();
}

void mh_debug_log(String message, Any object, String file, count_t line, String function) {
    fprintf(stdout, MH_DEBUG_COLOR "mh_debug(0x%llx): %s:%llu: %s(): %s\n" MH_RESET_COLOR, (count_t)object, file, line, function, message);
    fflush(stdout);
}
