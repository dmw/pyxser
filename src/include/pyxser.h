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

#ifdef __cplusplus
extern "C" {
#endif /* !__cplusplus */

#define PYXSER_VERSION					"0.1"

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

/* lol code xD */
#define PYTHON_HAZ_KLASS(o)                             \
	(PyObject_HasAttrString(o, PYTHON_CLASS_ATTR) == 1)

#define PYTHON_SERX_PHASE_NONE			0
#define PYTHON_SERX_PHASE_CONCRETE		1
#define PYTHON_SERX_PHASE_OBJECT		2

#define PYXSER_INVALID_ARGUMENT			-1
#define PYXSER_NOT_FOUND				0
#define PYXSER_FOUND					1

    typedef struct pythonUnserializationArguments_ {
        PyObject **doc;
        PyObject **current;
        PyObject **tree;
        PyDictObject **dups;
        PyDictObject **modules;
        xmlDocPtr *docPtr;
        xmlNodePtr *rootNode;
        xmlNodePtr *currentNode;
        char *encoding;
        int depth;
        int depthcnt;
    } PythonUnserializationArguments;

    typedef PythonUnserializationArguments *PythonUnserializationArgumentsPtr;

    typedef struct pythonTypeSerialize_ {
        int available;
        int (*checker)(PyObject *o);
        xmlNodePtr (*serializer)(PyObject *o, char *name,
                                 PyListObject *dupItems, xmlDocPtr doc,
                                 int *depth, int *depthcnt);
    } PythonTypeSerialize;

    typedef struct pythonTypeDeserialize_ {
        int available;
        int (*check)(xmlNodePtr node);
        PyObject *(*deserializer)(PythonUnserializationArgumentsPtr obj);
    } PythonTypeDeserialize;

#include "include/pyxser_numbers.h"
#include "include/pyxser_strings.h"
#include "include/pyxser_collections.h"
#include "include/pyxser_tools.h"

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
    extern const char pyxser_xml_element_collection[];
    extern const char pyxser_xml_element_object[];
    extern const char pyxser_xml_element_prop[];
    extern const char pyxser_xml_encoding[];
    extern const char pyxser_xml_encoding_mode[];
    extern const char pyxser_xml_version[];
    extern const char pyxser_xml_dtd_location[];
    extern const char pyxser_xml_dtd_c14n_location[];
    extern const char pyxser_xml_dtd[];

    extern PyObject *invalid_encoding_exception;
    extern PyObject *invalid_xml_exception;
    extern PyObject *invalid_argument_exception;

#ifdef __cplusplus
};
#endif /* !__cplusplus */

#endif /* PYXSER_H */
/* pyserx.h ends here */
