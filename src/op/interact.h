/*
 * Cozmonaut
 * Copyright 2019 The Cozmonaut Contributors
 */

#ifndef OP_INTERACT_H
#define OP_INTERACT_H

/** Arguments for interaction. */
struct op_interact_args {
  int __not_empty;
};

/**
 * Main function for interaction.
 *
 * @param args The operation arguments
 * @return Zero on success, otherwise nonzero
 */
int op_interact_main(struct op_interact_args* args);

#endif // #ifndef OP_INTERACT_H
