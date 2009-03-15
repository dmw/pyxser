/* -*- mode: c; indent-tabs-mode: t; tab-width: 4; c-file-style: "python" -*- */
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

    <!DOCTYPE pyxs:object
              PUBLIC "-//coder.cl//DTD pyxser 1.0//EN"
              "http://projects.coder.cl/pyxser/dtd/pyxser-1.0.dtd">
 */
#ifndef lint
static const char Id[] = "$Id$";
#endif /* !lint */

#include <Python.h>
#include <datetime.h>
#include <libxml/parser.h>
#include <libxml/tree.h>

#include "include/pyxser.h"

static const char msg_non_object[] = \
	"Invalid call, the function needs an XML string to restore an object";
static const char msg_non_method[] = \
	"Invalid call, been called as object";
static const char xml_version[] = \
	"1.0";
static const char xml_encoding[] = \
	"utf-8";

const char pyxser_attr_class[] = "__class__";
const char pyxser_attr_dict[] = "__dict__";
const char pyxser_attr_module[] = "__module__";
const char pyxser_attr_name[] = "__name__";
const char pyxser_attr_all[] = "__all__";
const char pyxser_version[] = PYXSER_VERSION;
const char pyxser_xml_attr_id[] = "id";
const char pyxser_xml_attr_item[] = "prop";
const char pyxser_xml_attr_module[] = "module";
const char pyxser_xml_attr_name[] = "name";
const char pyxser_xml_attr_ns[] = "http://coder.cl/pyxser/model/";
const char pyxser_xml_attr_ns_prefix[] = "pyxs";
const char pyxser_xml_attr_ref[] = "ref";
const char pyxser_xml_attr_size[] = "size";
const char pyxser_xml_attr_type[] = "type";
const char pyxser_xml_attr_version[] = "version";
const char pyxser_xml_attr_key[] = "key";
const char pyxser_xml_attr_xml_ns[] = "xmlns:pyxs";
const char pyxser_xml_element_collection[] = "col";
const char pyxser_xml_element_object[] = "obj";
const char pyxser_xml_element_item[] = "prop";
const char pyxser_xml_encoding[] = "utf-8";
const char pyxser_xml_version[] = "1.0";

static PyObject *pyxserxml(PyObject *self, PyObject *args);
static PyObject *pyxunserxml(PyObject *self, PyObject *args);

void *dummy(PyObject *obj);

static PyMethodDef serxMethods[] = {
    {"serialize", pyxserxml, METH_VARARGS, "Function to serialize an object as XML"},
    {"unserialize", pyxunserxml, METH_VARARGS, "Function unserialize an object as XML"},
    {NULL, NULL, 0, NULL}
};


PyMODINIT_FUNC
initpyxser(void)
{
    PyObject *m;
    m = Py_InitModule("pyxser", serxMethods);
    if (m == NULL)
        return;
	/* init module */
    LIBXML_TEST_VERSION;
}

static PyObject *
pyxserxml(PyObject *self, PyObject *args)
{
	PyObject *res = Py_None;
	PyObject *input = (PyObject *)NULL;
	xmlNodePtr serXml = (xmlNodePtr)NULL;
	xmlNodePtr rootNode = (xmlNodePtr)NULL;
	xmlDocPtr docXml = (xmlDocPtr)NULL;
	xmlChar *xmlBuff = (xmlChar *)NULL;
	int bufferSize = 0, ok = -1;
	PyListObject *dupItems = (PyListObject *)NULL;
	if (PYTHON_IS_NONE(args)) {
		/* error! don't have arguments */
		PyErr_SetString(PyExc_ValueError, msg_non_object);
		return res;
	}
	ok = PyArg_ParseTuple(args, "O", &input);
	if (!ok) {
		/* error! don't have arguments */
		PyErr_SetString(PyExc_ValueError, msg_non_object);
		return res;
	}
	dupItems = (PyListObject *)PyList_New(0);
	serXml = pyxser_SerializeXml(input, &docXml, &rootNode,
								 (xmlNodePtr *)NULL, dupItems);
	if (dupItems != (PyListObject *)NULL) {
		Py_DECREF(dupItems);
	}
	if (serXml != (xmlNodePtr)NULL
		&& docXml != (xmlDocPtr)NULL) {
		xmlDocDumpFormatMemoryEnc(docXml, &xmlBuff, &bufferSize,
								  xml_encoding, 1);
		if (xmlBuff != BAD_CAST NULL) {
			xmlFreeDoc(docXml);
			res = PyString_FromString((const char *)xmlBuff);
			Py_INCREF(res);
		}
	}
	return res;
	/* error! not created */
}

static PyObject *
pyxunserxml(PyObject *self, PyObject *args)
{
	PyObject *res = Py_None;
	PyObject *input = (PyObject *)NULL;
	PyObject *tree = (PyObject *)NULL;
	PyObject *current = (PyObject *)NULL;
	PyDictObject *dupItems = (PyDictObject *)NULL;
	PyDictObject *modules = (PyDictObject *)NULL;
	xmlNodePtr rootNode = (xmlNodePtr)NULL;
	xmlNodePtr currentNode = (xmlNodePtr)NULL;
	xmlDocPtr docXml = (xmlDocPtr)NULL;

	PythonUnserializationArguments obj;

	int ok = -1;
	if (PYTHON_IS_NONE(args)) {
		/* error! don't have arguments */
		PyErr_SetString(PyExc_ValueError, msg_non_object);
		return res;
	}
	ok = PyArg_ParseTuple(args, "S", &input);
	if (!ok) {
		/* error! don't have arguments */
		PyErr_SetString(PyExc_ValueError, msg_non_object);
		return res;
	}
	dupItems = (PyDictObject *)PyDict_New();

	obj.doc = &input;
	obj.current = &current;
	obj.tree = &tree;
	obj.dups = &dupItems;
	obj.modules = &modules;
	obj.docPtr = &docXml;
	obj.rootNode = &rootNode;
	obj.currentNode = &currentNode;

	res = pyxser_UnserializeXml(&obj);
	if (PYTHON_IS_NOT_NONE(res)) {
		return res;
	}
	return Py_None;
}


/* pyserx.c ends here */
