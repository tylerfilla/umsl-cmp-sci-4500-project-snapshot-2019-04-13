/*
 * Cozmonaut
 * Copyright 2019 The Cozmonaut Contributors
 */

#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <structmember.h>

#include "common.h"

typedef struct {
  PyObject_HEAD
  PyObject* first;
  PyObject* last;
  int number;
} server;

static void type_server_dealloc(server* self) {
  Py_XDECREF(self->first);
  Py_XDECREF(self->last);
  Py_TYPE(self)->tp_free((PyObject*) self);
}

static PyObject* type_server_new(PyTypeObject* type, PyObject* args, PyObject* kwds) {
  server* self;

  self = (server*) type->tp_alloc(type, 0);
  if (self != NULL) {
    self->first = PyUnicode_FromString("");
    if (self->first == NULL) {
      Py_DECREF(self);
      return NULL;
    }

    self->last = PyUnicode_FromString("");
    if (self->last == NULL) {
      Py_DECREF(self);
      return NULL;
    }

    self->number = 0;
  }

  return (PyObject*) self;
}

static int type_server_init(server* self, PyObject* args, PyObject* kwds) {
  PyObject* first = NULL, * last = NULL, * tmp;

  static char* kwlist[] = {"first", "last", "number", NULL};

  if (!PyArg_ParseTupleAndKeywords(args, kwds, "|SSi", kwlist, &first, &last, &self->number)) {
    return -1;
  }

  if (first) {
    tmp = self->first;
    Py_INCREF(first);
    self->first = first;
    Py_DECREF(tmp);
  }

  if (last) {
    tmp = self->last;
    Py_INCREF(last);
    self->last = last;
    Py_DECREF(tmp);
  }

  return 0;
}

static PyMemberDef type_server_members[] = {
  {
    .name = "number",
    .type = T_INT,
    .offset = offsetof(server, number),
    .flags = 0,
    .doc = "server number",
  },
  {NULL},
};

static PyObject* type_server_getfirst(server* self, void* closure) {
  Py_INCREF(self->first);
  return self->first;
}

static int type_server_setfirst(server* self, PyObject* value, void* closure) {
  if (value == NULL) {
    PyErr_SetString(PyExc_TypeError, "Cannot delete the first attribute");
    return -1;
  }

  if (!PyUnicode_Check(value)) {
    PyErr_SetString(PyExc_TypeError, "The first attribute value must be a string");
    return -1;
  }

  Py_DECREF(self->first);
  Py_INCREF(value);
  self->first = value;

  return 0;
}

static PyObject* type_server_getlast(server* self, void* closure) {
  Py_INCREF(self->last);
  return self->last;
}

static int type_server_setlast(server* self, PyObject* value, void* closure) {
  if (value == NULL) {
    PyErr_SetString(PyExc_TypeError, "Cannot delete the last attribute");
    return -1;
  }

  if (!PyUnicode_Check(value)) {
    PyErr_SetString(PyExc_TypeError, "The last attribute value must be a string");
    return -1;
  }

  Py_DECREF(self->last);
  Py_INCREF(value);
  self->last = value;

  return 0;
}

static PyGetSetDef type_server_getset[] = {
  {
    .name = "first",
    .get = (getter) type_server_getfirst,
    .set = (setter) type_server_setfirst,
    .doc = "first name",
    .closure = NULL,
  },
  {
    .name = "last",
    .get = (getter) type_server_getlast,
    .set = (setter) type_server_setlast,
    .doc = "last name",
    .closure = NULL,
  },
  {NULL},
};

static PyObject* type_server_name(server* self) {
  return PyUnicode_FromFormat("%S %S", self->first, self->last);
}

/** _core.Server methods. */
static PyMethodDef type_server_methods[] = {
  {
    .ml_name = "name",
    .ml_meth = (PyCFunction) type_server_name,
    .ml_flags = METH_NOARGS,
    .ml_doc = "Return the name, combining the first and last name",
  },
  {NULL},
};

/** _core.Server type. */
static PyTypeObject type_server = {
  PyVarObject_HEAD_INIT(NULL, 0)
  .tp_name = "_core.Server",
  .tp_basicsize = sizeof(server),
  .tp_itemsize = 0,
  .tp_dealloc = (destructor) type_server_dealloc,
  .tp_print = NULL,
  .tp_getattr = NULL,
  .tp_setattr = NULL,
  .tp_as_async = NULL,
  .tp_repr = NULL,
  .tp_as_number = NULL,
  .tp_as_sequence = NULL,
  .tp_as_mapping = NULL,
  .tp_hash = NULL,
  .tp_call = NULL,
  .tp_str = NULL,
  .tp_getattro = NULL,
  .tp_setattro = NULL,
  .tp_as_buffer = NULL,
  .tp_flags = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,
  .tp_doc = "The core server type.",
  .tp_traverse = NULL,
  .tp_clear = NULL,
  .tp_richcompare = NULL,
  .tp_weaklistoffset = 0,
  .tp_iter = NULL,
  .tp_iternext = NULL,
  .tp_methods = type_server_methods,
  .tp_members = type_server_members,
  .tp_getset = type_server_getset,
  .tp_base = NULL,
  .tp_dict = NULL,
  .tp_descr_get = NULL,
  .tp_descr_set = NULL,
  .tp_dictoffset = 0,
  .tp_init = (initproc) type_server_init,
  .tp_alloc = NULL,
  .tp_new = type_server_new,
};

/** _core extension module. */
static PyModuleDef module_core = {
  PyModuleDef_HEAD_INIT,
  .m_name = "_core",
  .m_doc = "The cozmonaut core module.",
  .m_size = -1,
  .m_methods = NULL,
  .m_slots = NULL,
  .m_traverse = NULL,
  .m_clear = NULL,
  .m_free = NULL,
};

/** Initialize _core extension module. */
PyMODINIT_FUNC PyInit_core() {
  if (PyType_Ready(&type_server) < 0) {
    return NULL;
  }

  PyObject* m__core = PyModule_Create(&module_core);
  if (m__core == NULL) {
    return NULL;
  }

  Py_INCREF(&type_server);
  PyModule_AddObject(m__core, "Server", (PyObject*) &type_server);

  return m__core;
}

int op_common_initialize() {
  // Make _core module available for importing
  PyImport_AppendInittab("_core", &PyInit_core);

  // Spin up the Python VM
  Py_Initialize();

  // Import "sys" module
  PyObject* m_sys = PyImport_ImportModule("sys");
  if (m_sys == NULL) {
    PyErr_Print();
    PyErr_Clear();
    return 1;
  }

  // Look up path attribute on sys module
  PyObject* m_sys_path = PyObject_GetAttrString(m_sys, "path");
  if (m_sys_path == NULL) {
    PyErr_Print();
    PyErr_Clear();

    Py_DECREF(m_sys);
    return 1;
  }

  // The Python module directory name
  // TODO: Allow supplying this via command-line argument
  PyObject* directory_name = PyUnicode_FromString("python");
  if (directory_name == NULL) {
    PyErr_Print();
    PyErr_Clear();

    Py_DECREF(m_sys_path);
    Py_DECREF(m_sys);
    return 1;
  }

  // Add Python module directory to search path
  // This will let the interpreter pick up our modules
  if (PyList_Append(m_sys_path, directory_name) < 0) {
    PyErr_Print();
    PyErr_Clear();

    Py_DECREF(directory_name);
    Py_DECREF(m_sys_path);
    Py_DECREF(m_sys);
    return 1;
  }

  Py_DECREF(directory_name);
  Py_DECREF(m_sys_path);
  Py_DECREF(m_sys);
  return 0;
}

int op_common_finalize() {
  // Wind down the interpreter
  if (Py_FinalizeEx() < 0) {
    fprintf(stderr, "failed to safely bring down the interpreter\n");
    return 1;
  }

  return 0;
}
