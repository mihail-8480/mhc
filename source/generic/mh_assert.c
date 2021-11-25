#include <mh.h>
#include <stdlib.h>
#include <stdio.h>
MH_EXPORT void mh_assert_fail(String assertion, String file, count_t line, String function) {
    fprintf(stderr,"%s:%llu: %s: Assertion `%s` failed.\n", file, line, function, assertion);
    fflush(stderr);
    abort();
}