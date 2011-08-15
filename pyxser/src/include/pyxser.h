/* -*- mode: c; indent-tabs-mode: nil; tab-width: 4; c-file-style: "python" -*- */
/* vim:set ft=c ff=unix ts=4 sw=4 enc=latin1 noexpandtab: */
/* kate: space-indent off; indent-width 4; mixedindent off; indent-mode cstyle; */
/*
  Copyright (c) 2009 Daniel Molina Wegener <dmw@coder.cl>

  This file is part of pyxser.

  pyxser is free software: you can redistribute it and/or modify
  it under the terms of the GNU Lesser General Public License as
  published by the Free Software Foundation, either version 3 of
  the License, or (at your option) any later version.

  pyxser is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with pyxser.  If not, see <http://www.gnu.org/licenses/>.

  <!DOCTYPE pyxs:obj
  PUBLIC "-//coder.cl//DTD pyxser 1.0//EN"
  "http://projects.coder.cl/pyxser/dtd/pyxser-1.0.dtd">
*/

#ifndef PYXSER_H
#define PYXSER_H 1

#include <Python.h>
#include <datetime.h>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <libxml/xmlschemas.h>

#ifdef __cplusplus
extern "C" {
#endif /* !__cplusplus */

#define PYXSER_VERSION					"1.6-r1"

#define PYTHON_IS_NOT_NONE(o)                   \
	(((PyObject *)o != Py_None) &&              \
	 ((PyObject *)o != (PyObject *)NULL))

#define PYTHON_IS_NONE(o)                       \
	(((PyObject *)o == Py_None) ||              \
	 ((PyObject *)o == (PyObject *)NULL))

#define PYXSER_FREE_OBJECT(o)                   \
    if (PYTHON_IS_NOT_NONE(o)) {                \
        Py_DECREF(o);                           \
    }

#define PYXSER_XMLFREE(o)                       \
    if ((void *)o != (void *)NULL) {            \
        xmlFree(o);                             \
        o = NULL;                               \
    }                                           \

#define PYXSER_INCREF(o)                        \
    if (PYTHON_IS_NOT_NONE(o)) {                \
        Py_XINCREF(o);                          \
    }

#define PYXSER_DECREF(o)                        \
    if (PYTHON_IS_NOT_NONE(o)) {                \
        Py_XDECREF(o);                          \
    }

/* This was changed according to the PEP0353
 * http://www.python.org/dev/peps/pep-0353/ on those
 * Python versions where Py_ssize_t is not present */
#if PY_VERSION_HEX < 0x02050000 && !defined(PY_SSIZE_T_MIN)

#ifndef Py_TYPE
#define Py_TYPE(ob)                             \
    (((PyObject*)(ob))->ob_type)
#endif /* Py_TYPE */

#ifndef PyAnySet_CheckExact
#define PyAnySet_CheckExact(ob)                                         \
    (Py_TYPE(ob) == &PySet_Type || Py_TYPE(ob) == &PyFrozenSet_Type)
#endif /* PyAnySet_CheckExact */

typedef int Py_ssize_t;

#define PY_SSIZE_T_MAX INT_MAX
#define PY_SSIZE_T_MIN INT_MIN
#endif

#if PY_MAJOR_VERSION >= 3

#define PyString_Check                  PyBytes_Check
#define PyString_CheckExact             PyBytes_CheckExact
#define PyString_FromString             PyBytes_FromString
#define PyString_FromStringAndSize      PyUnicode_FromStringAndSize
#define PyString_GET_SIZE               PyBytes_GET_SIZE
#define PyString_AS_STRING              PyBytes_AS_STRING
#define PyString_AsString               PyBytes_AsString
#define PyInt_Check                     PyLong_Check
#define PyInt_CheckExact                PyLong_CheckExact
#define PyInt_FromString                PyLong_FromString
#define PyFloat_FromString(s, n)        PyFloat_FromString(s)

#define PYXSER_GET_ATTR_NAME(currentKey, enc, unic, args, sz)           \
    unic = (PyObject *)NULL;                                            \
    args->name = (char *)NULL;                                          \
    if (PYTHON_IS_NOT_NONE(currentKey)) {                               \
        sz = PyUnicode_GET_SIZE(currentKey);                            \
        ub = PyUnicode_AS_UNICODE(currentKey);                          \
        unic = PyUnicode_Encode(ub, sz, pyxser_xml_encoding,            \
                                pyxser_xml_encoding_mode);              \
        if (PYTHON_IS_NOT_NONE(unic)) {                                 \
            args->name = PyString_AS_STRING(unic);                      \
        } else {                                                        \
            PyErr_Clear();                                              \
            args->name = (char *)NULL;                                  \
        }                                                               \
    }

#else /* PY_MAJOR_VERSION >= 3 */

#define PYXSER_GET_ATTR_NAME(currentKey, enc, unic, args, sz)           \
    unic = (PyObject *)NULL;                                            \
    args->name = (char *)NULL;                                          \
    if (PYTHON_IS_NOT_NONE(currentKey)                                  \
        && (pyxserUnicode_CheckExact(currentKey)) == 1) {               \
        sz = PyUnicode_GET_SIZE(currentKey);                            \
        ub = PyUnicode_AS_UNICODE(currentKey);                          \
        unic = PyUnicode_Encode(ub, sz, pyxser_xml_encoding,            \
                                pyxser_xml_encoding_mode);              \
        if (PYTHON_IS_NOT_NONE(unic)) {                                 \
            args->name = PyString_AS_STRING(unic);                      \
        } else {                                                        \
            PyErr_Clear();                                              \
            args->name = (char *)NULL;                                  \
        }                                                               \
    }                                                                   \
    if (PYTHON_IS_NOT_NONE(currentKey)                                  \
        && (pyxserString_CheckExact(currentKey)) == 1) {                \
        args->name = PyString_AS_STRING(currentKey);                    \
    }

#endif /* PY_MAJOR_VERSION >= 3 */

/* lol code xD */
#define PYTHON_HAZ_KLASS(o)                     \
	(PyObject_HasAttrString(o, PYTHON_CLASS_ATTR) == 1)

#define PYTHON_SERX_PHASE_NONE			0
#define PYTHON_SERX_PHASE_CONCRETE		1
#define PYTHON_SERX_PHASE_OBJECT		2

#define PYXSER_INVALID_ARGUMENT			-1
#define PYXSER_NOT_FOUND				0
#define PYXSER_FOUND					1

    typedef struct pythonUnserializationArguments_ {
        int depth;
        int depthcnt;
        int error;
        int construct;
        PyObject **doc;
        PyObject **current;
        PyObject **tree;
        PyObject *typemap;
        PyDictObject **dups;
        PyDictObject **modules;
        xmlDocPtr *docPtr;
        xmlNodePtr *rootNode;
        xmlNodePtr *currentNode;
        char *encoding;
    } PythonUnserializationArguments;

    typedef struct pythonSerializationArguments_ {
        int error;
        PyObject **item;
        PyObject **ck;
        PyObject **o;
        PyObject **selector;
        PyObject *typemap;
        xmlDocPtr *docptr;
        xmlNodePtr *rootNode;
        xmlNodePtr *currentNode;
        PyListObject **dupSrcItems;
        char *enc;
        char *name;
        int *depth;
        int *depthcnt;
    } PythonSerializationArguments;

    typedef PythonUnserializationArguments *PyxSerDeserializationArgsPtr;
    typedef PythonSerializationArguments *PyxSerializationArgsPtr;

    typedef struct pythonTypeSerialize_ {
        int available;
        int (*check)(PyObject *o);
        xmlNodePtr (*serializer)(PyxSerializationArgsPtr args);
    } PythonTypeSerialize;

    typedef struct pythonTypeDeserialize_ {
        int available;
        int (*check)(xmlNodePtr node);
        PyObject *(*deserializer)(PyxSerDeserializationArgsPtr obj);
    } PythonTypeDeserialize;

#include "include/pyxser_tools.h"
#include "include/pyxser_serializer.h"
#include "include/pyxser_numbers.h"
#include "include/pyxser_strings.h"
#include "include/pyxser_collections.h"
#include "include/pyxser_typem.h"

    extern const char msg_non_object[];
    extern const char msg_non_method[];
    extern const char msg_non_xml[];
    extern const char msg_non_canon_encoding[];

    extern const char xml_version[];
    extern const char xml_encoding[];

    extern const char pyxser_attr_class[];
    extern const char pyxser_attr_dict[];
    extern const char pyxser_attr_module[];
    extern const char pyxser_attr_name[];
    extern const char pyxser_attr_all[];

    extern const char pyxser_version[];

    extern const char pyxser_xml_attr_id[];
    extern const char pyxser_xml_attr_item[];
    extern const char pyxser_xml_attr_module[];
    extern const char pyxser_xml_attr_name[];
    extern const char pyxser_xml_attr_ns[];
    extern const char pyxser_xml_attr_ns_prefix[];
    extern const char pyxser_xml_attr_ref[];
    extern const char pyxser_xml_attr_size[];
    extern const char pyxser_xml_attr_type[];
    extern const char pyxser_xml_attr_version[];
    extern const char pyxser_xml_attr_key[];
    extern const char pyxser_xml_attr_xml_ns[];
    extern const char pyxser_xml_attr_xml_xsi[];
    extern const char pyxser_xml_attr_xml_xsi_value[];

    extern const char pyxser_xml_element_collection[];
    extern const char pyxser_xml_element_object[];
    extern const char pyxser_xml_element_prop[];

    extern const char pyxser_xml_encoding[];
    extern const char pyxser_xml_encoding_mode[];
    extern const char pyxser_xml_version[];

    extern const char pyxser_xml_dtd_location[];
    extern const char pyxser_xml_dtd_c14n_location[];

    extern const char pyxser_xml_xsd_location[];
    extern const char pyxser_xml_xsd_c14n_location[];

    extern const char pyxser_xml_attr_xml_xsd[]; /* = "xmlns:xsi" */
    extern const char pyxser_xml_attr_xml_xsd_prfx[]; /* = "xmlns:xsi" */
    extern const char pyxser_xml_attr_xml_xsi[]; /* = "xmlns:xsi" */
    extern const char pyxser_xml_attr_xml_xsi_prfx[]; /* = "xsi" */
    extern const char pyxser_xml_attr_xml_xsi_value[]; /* = "http://www.w3.org/2001/XMLSchema-instance" */
    extern const char pyxser_xml_attr_xml_xloc[]; /* = "xsi:schemaLocation" */
    extern const char pyxser_xml_attr_xml_xloc_value[]; /* = "http://projects.coder.cl/pyxser/dtd/pyxser-1.0.xsd" */
    extern const char pyxser_xml_dtd[];

    extern PyObject *invalid_encoding_exception;
    extern PyObject *invalid_xml_exception;
    extern PyObject *invalid_argument_exception;

#ifdef __cplusplus
};
#endif /* !__cplusplus */

#endif /* PYXSER_H */
/* pyserx.h ends here */
