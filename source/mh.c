#include <mh.h>

#define MH_STRINGIFY1(x) #x
#define MH_STRINGIFY(x) MH_STRINGIFY1(x)

mh_library_version_t version = {
    .name = MH_STRINGIFY(MH_PROJECT_NAME),
    .major = MH_PROJECT_VERSION_MAJOR,
    .minor = MH_PROJECT_VERSION_MINOR,
    .patch = MH_PROJECT_VERSION_PATCH,
    .tweak = MH_PROJECT_VERSION_TWEAK,
    .description = MH_STRINGIFY(MH_PROJECT_DESCRIPTION)
};

LibraryVersion mh_get_version(void) { // NOLINT(readability-const-return-type)
    return &version;
}