/*
 * Cozmonaut
 * Copyright 2019 The Cozmonaut Contributors
 */

#include "global.h"

/** The global program info. */
static struct global g__;

// Prepare immutable view of it
const struct global* const g = &g__;

// Prepare mutable view of it
struct global* const g_mut = &g__;
