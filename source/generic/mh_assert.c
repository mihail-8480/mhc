#include MH_SELFTEST_IMPL
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void mh_assert_fail(String assertion, String file, count_t line, String function) {
    fprintf(stderr,MH_ASSERT_FAIL_COLOR "mh_assert_fail(): %s:%llu: %s(): Assertion `%s` failed.\n" MH_RESET_COLOR, file, line, function, assertion);
    fflush(stderr);
    abort();
}

void mh_debug_log(String message, Any object, String file, count_t line, String function) {
#define TRUE_STR "1"
    var no_spam = getenv("MH_NO_SPAM");
    if (no_spam == MH_NULL || strncmp(no_spam, TRUE_STR, sizeof TRUE_STR-1) != 0) {
        fprintf(stdout, MH_DEBUG_COLOR "mh_debug(0x%llx): %s:%llu: %s(): %s\n" MH_RESET_COLOR, (count_t) object, file,
                line, function, message);
        fflush(stdout);
    }
#undef TRUE_STR
}
