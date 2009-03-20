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

    <!DOCTYPE pyxs:obj
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
const char pyxser_xml_attr_ns[] = "http://projects.coder.cl/pyxser/model/";
const char pyxser_xml_attr_ns_prefix[] = "pyxs";
const char pyxser_xml_attr_ref[] = "ref";
const char pyxser_xml_attr_size[] = "size";
const char pyxser_xml_attr_type[] = "type";
const char pyxser_xml_attr_version[] = "version";
const char pyxser_xml_attr_key[] = "key";
const char pyxser_xml_attr_xml_ns[] = "xmlns:pyxs";
const char pyxser_xml_element_collection[] = "col";
const char pyxser_xml_element_object[] = "obj";
const char pyxser_xml_element_prop[] = "prop";
const char pyxser_xml_encoding[] = "utf-8";
const char pyxser_xml_version[] = "1.0";

const char pyxser_xml_dtd_location[] = PYXSER_DTD_FILE;

static PyObject *pyxserxml(PyObject *self, PyObject *args);
static PyObject *pyxserxmlnonstd(PyObject *self, PyObject *args);
static PyObject *pyxunserxml(PyObject *self, PyObject *args);
static PyObject *pyxvalidate(PyObject *self, PyObject *args);
static PyObject *pyxgetdtd(PyObject *self, PyObject *args);


void *dummy(PyObject *obj);

static const char serialize_documentation[] = \
	"Gets any object defined in a Python module as class as argument\n"
	"and serializes it as XML. The serialization model is based on the\n"
	"pyxser DTD 1.0. Objects are serialized as pyxs:obj element, collections\n"
	"are serialized as pyxs:col element (tuples, lists and dictionaries)\n"
	"and properties are serialized as pyxs:prop elements.\n\n"
	"The serialization algorithm is a O(n) one, this means which the\n"
	"serializer runs over the object tree just one time and cross\n"
	"refernces are serialized as XML references through the IDREF\n"
	"attribute pyxs:ref. Also this serialization method is supposed\n"
	"to support C14N\n\n"
	"The serialization model resides in the pyxser public identifier DTD:\n"
	"    <!DOCTYPE pyxs:obj\n"
    "              PUBLIC '-//coder.cl//DTD pyxser 1.0//EN'\n"
    "              'http://projects.coder.cl/pyxser/dtd/pyxser-1.0.dtd'>\n";

static const char deserialize_documentation[] = \
	"Takes an XML string as arguments to deserialize it and be converted\n"
	"back to it's original Python object. The deserialization algorithm\n"
	"supports automatic module loading, but searches for them in the module\n"
	"dictionary first to reach the original object type. It needs that the\n"
	"implied modules can be recheable by Python to get back the objects in\n"
	"it's original form.\n\n"
	"The deserialization algorithm is a O(n) one, this means that forward\n"
	"references are not supported because the first ocurrence of any object\n"
	"it's serialized once and then referenced, but not referenced and then\n"
	"serilized.\n\n"
	"Every serilized object is validated against the pyxser DTD 1.0\n\n";

static const char validate_documentation[] = \
	"Validates the XML input string against the pyxser DTD 1.0 in your\n"
	"local filesystem.\n\n";

static const char getdtd_documentation[] = \
	"The serialization model resides in the pyxser public identifier DTD.\n"
	"This function returns the pyxser DTD location in your system as string\n\n";


static PyMethodDef serxMethods[] = {
    {"serialize", pyxserxml, METH_VARARGS, serialize_documentation},
    {"unserialize", pyxunserxml, METH_VARARGS, deserialize_documentation},
    {"validate", pyxvalidate, METH_VARARGS, validate_documentation},
    {"getdtd", pyxgetdtd, METH_VARARGS, getdtd_documentation},
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
			res = PyString_FromString((const char *)xmlBuff);
			Py_INCREF(res);
		}
	}
	xmlFreeDoc(docXml);
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

static PyObject *
pyxgetdtd(PyObject *self, PyObject *args)
{
	PyObject *res;
	res = PyString_FromString((const char *)pyxser_xml_dtd_location);
	Py_INCREF(res);
	return res;
}

static PyObject *
pyxvalidate(PyObject *self, PyObject *args)
{
	xmlDocPtr docPtr = (xmlDocPtr)NULL;
	PyObject *res = Py_False;
	PyObject *input = (PyObject *)NULL;
	char *docstr = (char *)NULL;

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

	docstr = PyString_AS_STRING(input);
	docPtr = xmlReadMemory(docstr, strlen(docstr), NULL,
						   (const char *)pyxser_xml_encoding, 0);
	if (docPtr != (xmlDocPtr)NULL) {
		if ((pyxser_ValidateDocument(docPtr)) == 1) {
			res = Py_True;
		}
	}
	return res;
}


/* pyserx.c ends here */
