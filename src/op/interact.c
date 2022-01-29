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
  "from cozmonaut.entry_point.interact import EntryPointInteract\n"
  "EntryPointInteract().main()\n";

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
