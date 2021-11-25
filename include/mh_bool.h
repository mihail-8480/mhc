#ifndef CASYNC_MH_BOOL_H
#define CASYNC_MH_BOOL_H

// Can be "true" (MH_TRUE) or "false" (MH_FALSE).
typedef enum mh_bool {
    // = 0
    MH_FALSE = 0,
    // = 1
    MH_TRUE = 1
} mh_bool_t;

#ifndef __bool_true_false_are_defined
#define __bool_true_false_are_defined 1 // NOLINT(bugprone-reserved-identifier)
// <stdbool.h> stuff
#define bool mh_bool_t
#define true MH_TRUE
#define false MH_FALSE
#endif

#endif //CASYNC_MH_BOOL_H
