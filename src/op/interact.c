/*
 * Cozmonaut
 * Copyright 2019 The Cozmonaut Contributors
 */

#include <stdio.h>

#define PY_SSIZE_T_CLEAN
#include <Python.h>

#include "common.h"
#include "interact.h"

/** The Python code for this operation. */
static const char OPERATION_CODE[] =
  "from _core import Server\n"
  "from cozmonaut.client import Client\n"
  "server = Server()\n"
  "client = Client(server)\n"
  "result = client.entry_interact()\n";

int op_interact_main(struct op_interact_args* args) {
  // Initialize the operation
  if (op_common_initialize()) {
    fprintf(stderr, "failed to initialize operation\n");
    return 1;
  }

  // Run the operation code
  PyRun_SimpleString(OPERATION_CODE);

  // Finalize the operation
  if (op_common_finalize()) {
    fprintf(stderr, "failed to finalize operation\n");
    return 1;
  }

  return 0;
}
