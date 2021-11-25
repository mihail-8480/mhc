#include <mh.h>

#define mh_stringify1(x) #x
#define mh_stringify(x) mh_stringify1(x)

mh_library_version_t version = {
    .name = mh_stringify(MH_PROJECT_NAME),
    .major = MH_PROJECT_VERSION_MAJOR,
    .minor = MH_PROJECT_VERSION_MINOR,
    .patch = MH_PROJECT_VERSION_PATCH,
    .tweak = MH_PROJECT_VERSION_TWEAK,
    .description = mh_stringify(MH_PROJECT_DESCRIPTION)
};

LibraryVersion mh_get_version(void) { // NOLINT(readability-const-return-type)
    return &version;
}