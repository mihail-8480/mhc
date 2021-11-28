#include MH_SELFTEST_IMPL
#include <stdio.h>

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

#ifdef DEBUG
MH_CONSTRUCTOR void mh_print_version() {
    var ver = mh_get_version();
    char message[1024];
    sprintf(message, "%s/%llu.%llu.%llu.%llu - %s", ver->name, ver->major, ver->minor, ver->patch, ver->tweak, ver->description);
    mh_debug(message, ver);
}
#endif

LibraryVersion mh_get_version(void) { // NOLINT(readability-const-return-type)
    return &version;
}