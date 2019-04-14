/*
 * Cozmonaut
 * Copyright 2019 The Cozmonaut Contributors
 */

#ifndef OP_FRIEND_LIST_H
#define OP_FRIEND_LIST_H

/** Arguments for friend listing. */
struct op_friend_list_args {
  /** The target friend ID. */
  int friend_id;
};

/**
 * Main function for friend listing.
 *
 * @param args The operation arguments
 * @return Zero on success, otherwise nonzero
 */
int op_friend_list_main(struct op_friend_list_args* args);

#endif // #ifndef OP_FRIEND_LIST_H
