/*
 * Cozmonaut
 * Copyright 2019 The Cozmonaut Contributors
 */

#ifndef VERSION_H
#define VERSION_H

#include "util.h"

/** The program version major number. Bumps for breaking changes. */
#define VERSION_MAJOR __version_major

/** The program version minor number. Bumps for backward-compatible changes. */
#define VERSION_MINOR __version_minor

/** The program version patch number. Bumps for tweaks and bug fixes. */
#define VERSION_PATCH __version_patch

/** The full program version string. */
#define VERSION_STRING            \
    STRINGIFY(VERSION_MAJOR) "."  \
    STRINGIFY(VERSION_MINOR) "."  \
    STRINGIFY(VERSION_PATCH)

/** The build timestamp. */
#define VERSION_TIMESTAMP __DATE__ " " __TIME__

/** The version control branch. */
#define VERSION_VCS_BRANCH STRINGIFY(__git_refspec)

/** The version control hash. */
#define VERSION_VCS_HASH STRINGIFY(__git_hash)

#endif // #ifndef VERSION_H
