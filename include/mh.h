#ifndef CASYNC_MH_H
#define CASYNC_MH_H

// A reference to anything.
typedef void *Any;

// A readonly character array.
typedef const char *String;

// A big enough number to hold a memory address (hopefully).
typedef unsigned long long count_t;

// A function that is a part of the public API.
#define MH_EXPORT __attribute__((unused)) extern

// Something that is not being used but needs to be there.
#define MH_UNUSED __attribute__((unused))

// I guarantee that this function will never return.
#define mh_never __attribute__((noreturn)) void

// __auto_type is too long to type, so we are going to use var instead.
#define var __auto_type

// A reference to nothing.
#define MH_NULL ((Any)0)

#include "mh_bool.h"

#ifdef MH_LIBRARY
// The library version.
typedef struct mh_library_version {
    String name;
    count_t major;
    count_t minor;
    count_t patch;
    count_t tweak;
    String description;
} mh_library_version_t, * const LibraryVersion;

// Get library version.
LibraryVersion mh_get_version(void);
#endif

#endif //CASYNC_MH_H
