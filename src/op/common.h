/*
 * Cozmonaut
 * Copyright 2019 The Cozmonaut Contributors
 */

#ifndef OP_COMMON_H
#define OP_COMMON_H

/**
 * Initialize common operation.
 *
 * @return Zero on success, otherwise nonzero
 */
int op_common_initialize();

/**
 * Finalize common operation.
 *
 * @return Zero on success, otherwise nonzero
 */
int op_common_finalize();

#endif // #ifndef OP_COMMON_H
