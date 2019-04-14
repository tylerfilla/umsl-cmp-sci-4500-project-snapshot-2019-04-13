/*
 * Cozmonaut
 * Copyright 2019 The Cozmonaut Contributors
 */

#include <stdio.h>

#define PY_SSIZE_T_CLEAN
#include <Python.h>

#include "common.h"
#include "friend_list.h"

/** The Python code for this operation. */
static const char OPERATION_CODE[] =
  "from _core import Server\n"
  "from cozmonaut.client import Client\n"
  "server = Server()\n"
  "client = Client(server)\n"
  "result = client.entry_friend_list(args['friend_id'])\n";

int op_friend_list_main(struct op_friend_list_args* args) {
  // Initialize the operation
  if (op_common_initialize()) {
    fprintf(stderr, "failed to initialize operation\n");
    return 1;
  }

  // Look up the __main__ module
  // We'll execute under this context
  PyObject* py_module = PyImport_AddModule("__main__");
  if (py_module == NULL) {
    PyErr_Print();
    PyErr_Clear();
    return 1;
  }

  // Grab the dictionary from the main module
  PyObject* py_module_dict = PyModule_GetDict(py_module);
  if (py_module_dict == NULL) {
    PyErr_Print();
    PyErr_Clear();

    Py_DECREF(py_module);
    return 1;
  }

  // Create a dictionary for operation arguments
  PyObject* py_args = PyDict_New();
  if (py_args == NULL) {
    PyErr_Print();
    PyErr_Clear();

    Py_DECREF(py_module_dict);
    Py_DECREF(py_module);
    return 1;
  }

  // Add argument dictionary to module
  // This will let us access it from the operation code
  if (PyDict_SetItemString(py_module_dict, "args", py_args) < 0) {
    PyErr_Print();
    PyErr_Clear();

    Py_DECREF(py_args);
    Py_DECREF(py_module_dict);
    Py_DECREF(py_module);
    return 1;
  }

  // The friend_id argument
  PyObject* py_args_friend_id = PyLong_FromLong(args->friend_id);
  if (py_args_friend_id == NULL) {
    PyErr_Print();
    PyErr_Clear();

    Py_DECREF(py_args);
    Py_DECREF(py_module_dict);
    Py_DECREF(py_module);
    return 1;
  }

  // Map friend_id argument to argument dictionary
  if (PyDict_SetItemString(py_args, "friend_id", py_args_friend_id) < 0) {
    PyErr_Print();
    PyErr_Clear();

    Py_DECREF(py_args_friend_id);
    Py_DECREF(py_args);
    Py_DECREF(py_module_dict);
    Py_DECREF(py_module);
    return 1;
  }

  // Run the operation code
  // This will call into our main Python code
  PyObject* py_result = PyRun_String(OPERATION_CODE, Py_file_input, py_module_dict, py_module_dict);
  if (py_result == NULL) {
    PyErr_Print();
    PyErr_Clear();

    Py_DECREF(py_args_friend_id);
    Py_DECREF(py_args);
    Py_DECREF(py_module_dict);
    Py_DECREF(py_module);
    return 1;
  }

  // Clean up on success
  Py_DECREF(py_result);
  Py_DECREF(py_args_friend_id);
  Py_DECREF(py_args);
  Py_DECREF(py_module_dict);
  Py_DECREF(py_module);

  // Finalize the operation
  if (op_common_finalize()) {
    fprintf(stderr, "failed to finalize operation\n");
    return 1;
  }

  return 0;
}
