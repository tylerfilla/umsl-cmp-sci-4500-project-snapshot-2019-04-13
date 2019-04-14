/*
 * Cozmonaut
 * Copyright 2019 The Cozmonaut Contributors
 */

#ifndef OP_FRIEND_REMOVE_H
#define OP_FRIEND_REMOVE_H

/** Arguments for friend removal. */
struct op_friend_remove_args {
  /** The target friend ID. */
  int friend_id;
};

/**
 * Main function for friend removal.
 *
 * @param args The operation arguments
 * @return Zero on success, otherwise nonzero
 */
int op_friend_remove_main(struct op_friend_remove_args* args);

#endif // #ifndef OP_FRIEND_REMOVE_H
