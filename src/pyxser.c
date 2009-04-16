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
#ifndef lint
static const char Id[] = "$Id$";
#endif /* !lint */

#include <Python.h>
#include <datetime.h>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <libxml/c14n.h>

#include "include/pyxser.h"

const char msg_non_object[] = \
	"Invalid arguments, please read the documentation.";
const char msg_non_method[] = \
	"Invalid call, been called as object.";
const char msg_non_xml[] = \
	"Invalid XML, please read the documentation.";
const char msg_non_canon_encoding[] = \
	"Invalid encoding, canonical XML uses UTF-8.";
const char xml_version[] = \
	"1.0";
const char xml_encoding[] = \
	"utf-8";

const char pyxser_attr_class[] = "__class__";
const char pyxser_attr_dict[] = "__dict__";
const char pyxser_attr_module[] = "__module__";
const char pyxser_attr_name[] = "__name__";
const char pyxser_attr_all[] = "__all__";
const char pyxser_version[] = PYXSER_VERSION;
const char pyxser_xml_attr_id[] = "objid";
const char pyxser_xml_attr_item[] = "prop";
const char pyxser_xml_attr_module[] = "module";
const char pyxser_xml_attr_name[] = "name";
const char pyxser_xml_attr_ns[] = "http://projects.coder.cl/pyxser/model/";
const char pyxser_xml_attr_ns_prefix[] = "pyxs";
const char pyxser_xml_attr_ref[] = "objref";
const char pyxser_xml_attr_size[] = "size";
const char pyxser_xml_attr_type[] = "type";
const char pyxser_xml_attr_version[] = "version";
const char pyxser_xml_attr_key[] = "key";
const char pyxser_xml_attr_xml_ns[] = "xmlns:pyxs";
const char pyxser_xml_element_collection[] = "col";
const char pyxser_xml_element_object[] = "obj";
const char pyxser_xml_element_prop[] = "prop";
const char pyxser_xml_encoding[] = "utf-8";
const char pyxser_xml_encoding_mode[] = "strict";
const char pyxser_xml_version[] = "1.0";
const char pyxser_ext_version[] = "0.2rc1";

const char pyxser_xml_dtd_location[] = PYXSER_DTD_FILE;
const char pyxser_xml_dtd_c14n_location[] = PYXSER_DTD_C14N_FILE;

static PyObject *pyxserxml(PyObject *self, PyObject *args, PyObject *keywds);
static PyObject *pyxserxmlc14n(PyObject *self, PyObject *args, PyObject *keywds);
static PyObject *pyxserxmlc14nstrict(PyObject *self, PyObject *args, PyObject *keywds);
static PyObject *pyxunserxml(PyObject *self, PyObject *args, PyObject *keywds);
static PyObject *pyxvalidate(PyObject *self, PyObject *args, PyObject *keywds);
static PyObject *pyxvalidatec14n(PyObject *self, PyObject *args, PyObject *keywds);
static PyObject *pyxgetdtd(PyObject *self, PyObject *args);
static PyObject *xmlcleanup(PyObject *self, PyObject *args);
static char *xmlgetencoding(int id);


PyObject *invalid_encoding_exception;
PyObject *invalid_xml_exception;
PyObject *invalid_argument_exception;

static PyDictObject *pyxser_modules = (PyDictObject *)NULL;
void pyxser_register_encodings(void);
void pyxser_unregister(void);
void *dummy(PyObject *obj);

typedef struct pyxser_encodings_s {
    const char *enc_f;
    const char *enc_t;
} pyxser_encodings_t;

pyxser_encodings_t pyxser_encodings[] = {
    {"ISO-8859-1", "iso-8859-1"},
    {"ISO-8859-1", "latin1"},
    {"ISO-8859-1", "Latin1"},
    {"ISO-8859-1", "iso-Latin-1"},
    {"ISO-8859-1", "iso-latin-1"},
    {"UTF-8", "utf-8"},
    {"UTF-8", "utf8"},
    {"UTF-8", "UTF8"},
    {NULL, NULL}
};

static const char serialize_documentation[] = \
    "Uses the next keyword argumens:\n"
    "   obj     ->      python object to serialize\n"
    "   enc     ->      xml serialization encoding\n"
    "   depth   ->      node navigation depth\n\n"
	"Gets any object defined in a Python module as class as argument\n"
	"and serializes it as XML. The serialization model is based on the\n"
	"pyxser DTD 1.0. Objects are serialized as pyxs:obj element, collections\n"
	"are serialized as pyxs:col element (tuples, lists and dictionaries)\n"
	"and properties are serialized as pyxs:prop elements.\n\n"
	"The serialization algorithm is a O(n) one, this means which the\n"
	"serializer runs over the object tree just one time and cross\n"
	"refernces are serialized as XML references through the IDREF\n"
	"attribute pyxs:ref.\n\n"
	"The serialization model resides in the pyxser public identifier DTD:\n"
	"    <!DOCTYPE pyxs:obj\n"
    "              PUBLIC '-//coder.cl//DTD pyxser 1.0//EN'\n"
    "              'http://projects.coder.cl/pyxser/dtd/pyxser-1.0.dtd'>\n";

static const char serializec14n_documentation[] = \
    "Uses the next keyword argumens:\n"
    "   obj     ->      python object to serialize\n"
    "   depth   ->      node navigation depth\n"
    "   exc     ->      exclusive canonization\n"
    "   com     ->      with comments\n\n"
    "Same as serialize(), but uses C14N canonization, to use exclusive\n"
    "canonization the 'exc' argument must differ from zero and to use\n"
    "comments 'com' must differ from zero. The encoding must be UTF-8\n"
    "for canonization.\n\n"
	"    <!DOCTYPE pyxs:obj\n"
    "              PUBLIC '-//coder.cl//DTD pyxser C14N 1.0//EN'\n"
    "              'http://projects.coder.cl/pyxser/dtd/pyxser-1.0-c14n.dtd'>\n\n"
    "* Information about Canonical XML at:\n\thttp://www.w3.org/TR/xml-c14n\n"
    "* Information about Exclusive Canonical XML at\n\thttp://www.w3.org/TR/xml-exc-c14n\n"
    "NOTE: The canonical DTD converts all ID, IDREF and NMTOKEN\n"
    "      attributes to CDATA attributes\n";

static const char serializec14n_documentation_strict[] = \
    "Uses the next keyword argumens:\n"
    "   obj     ->      python object to serialize\n"
    "   depth   ->      node navigation depth\n"
    "   exc     ->      exclusive canonization\n"
    "   com     ->      with comments\n\n"
    "Same as serialize_c14n(), but uses C14N canonization in a strict\n"
    "mode and rather than serializing NMTOKEN, ID, and IDREF attributes,\n"
    "uses the C14N canon to execute the XML tree rendering. The encoding\n"
    "must be UTF-8 for canonization.\n\n"
	"    <!DOCTYPE pyxs:obj\n"
    "              PUBLIC '-//coder.cl//DTD pyxser C14N 1.0//EN'\n"
    "              'http://projects.coder.cl/pyxser/dtd/pyxser-1.0.dtd'>\n\n"
    "* Information about Canonical XML at:\n\thttp://www.w3.org/TR/xml-c14n\n"
    "* Information about Exclusive Canonical XML at\n\thttp://www.w3.org/TR/xml-exc-c14n\n"
    "NOTE: C14N serialized objects can not be deserialized because we\n"
    "      need the ID and IDREF attributes to suppor cross referenced\n"
    "      objects.\n";

static const char deserialize_documentation[] = \
    "Uses the next keyword argumens:\n"
    "   obj     ->      the serialized python object\n"
    "   enc     ->      xml serialization encoding\n"
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
    "Uses the next keyword argumens:\n"
    "   obj     ->      the serialized python object\n"
    "   enc     ->      xml serialization encoding\n\n"
	"Validates the XML input string against the pyxser DTD 1.0 in your\n"
	"local filesystem.\n\n";

static const char validate_documentation_c14n[] = \
    "Uses the next keyword argumens:\n"
    "   obj     ->      the serialized python object\n"
    "   enc     ->      xml serialization encoding\n\n"
	"Validates the XML input string against the pyxser DTD 1.0 C14N in\n"
    "your local filesystem.\n\n";

static const char getdtd_documentation[] = \
	"This function returns the pyxser DTD location in your system as\n"
    "string\n\n";

static const char xmlcleanup_documentation[] = \
	"Calls the cleanup function for the libxml2 parser, be carefull\n"
    "using this function, and try to use it when the parser really\n"
    "isn't in use.\n\n";

static PyMethodDef serxMethods[] = {
    {"serialize", (PyCFunction)pyxserxml,
     METH_VARARGS | METH_KEYWORDS,
     serialize_documentation},

    {"serialize_c14n", (PyCFunction)pyxserxmlc14n,
     METH_VARARGS | METH_KEYWORDS,
     serializec14n_documentation},

    {"serialize_c14n_strict", (PyCFunction)pyxserxmlc14nstrict,
     METH_VARARGS | METH_KEYWORDS,
     serializec14n_documentation_strict},

    {"unserialize", (PyCFunction)pyxunserxml,
     METH_VARARGS | METH_KEYWORDS,
     deserialize_documentation},

    {"validate", (PyCFunction)pyxvalidate,
     METH_VARARGS | METH_KEYWORDS,
     validate_documentation},

    {"validate_c14n", (PyCFunction)pyxvalidatec14n,
     METH_VARARGS | METH_KEYWORDS,
     validate_documentation_c14n},

    {"getdtd", pyxgetdtd, METH_VARARGS, getdtd_documentation},

    {"xmlcleanup", xmlcleanup, METH_VARARGS, xmlcleanup_documentation},

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
    Py_AtExit(pyxser_unregister);
    pyxser_register_encodings();
    PyModule_AddObject(m, "__version__", PyString_FromString(pyxser_ext_version));
    LIBXML_TEST_VERSION;
}

static PyObject *
pyxserxmlc14n(PyObject *self, PyObject *args, PyObject *keywds)
{
	PyObject *res = Py_None;
	PyObject *input = (PyObject *)NULL;
	PyListObject *dupItems = (PyListObject *)NULL;

	xmlNodePtr serXml = (xmlNodePtr)NULL;
	xmlNodePtr rootNode = (xmlNodePtr)NULL;
	xmlDocPtr docXml = (xmlDocPtr)NULL;
    xmlOutputBufferPtr xmlBuff = (xmlOutputBufferPtr)NULL;
    xmlChar *docPtr = (xmlChar *)NULL;

    static char *kwlist[] = {"obj", "depth", "exc", "com", NULL};
    int py_depth = 999999;
    int py_depth_cnt = 1;
    int py_exc = 0;
    int py_com = 0;
	int ok = -1;
    int ret = 0;

	if (PYTHON_IS_NONE(args)) {
		/* error! don't have arguments */
		PyErr_SetString(PyExc_ValueError, msg_non_object);
		return NULL;
	}
	ok = PyArg_ParseTupleAndKeywords(args, keywds, "Oiii", kwlist,
                                     &input, &py_depth,
                                     &py_exc, &py_com);
	if (!ok) {
		/* error! don't have arguments */
		PyErr_SetString(PyExc_ValueError, msg_non_object);
		return NULL;
	}

    if (PYTHON_IS_NONE(input)) {
		PyErr_SetString(PyExc_ValueError, msg_non_object);
		return NULL;
    }

    if (py_depth == 0) {
        py_depth = 999999;
    }

	dupItems = (PyListObject *)PyList_New(0);

	serXml = pyxser_SerializeXml(input, &docXml, &rootNode,
								 (xmlNodePtr *)NULL, dupItems,
                                 xml_encoding, &py_depth, &py_depth_cnt);

	if (PYTHON_IS_NOT_NONE(dupItems)) {
		Py_DECREF(dupItems);
	}

	if (serXml != (xmlNodePtr)NULL
		&& docXml != (xmlDocPtr)NULL) {
        if ((pyxser_ValidateDocumentC14N(docXml)) == 1) {
            xmlBuff = xmlAllocOutputBuffer(NULL);
            ret = xmlC14NExecute(docXml, pyxser_C14NRenderAllNodes, docXml,
                                 py_exc, NULL, py_com, xmlBuff);
            if (ret < 0) {
                xmlOutputBufferClose(xmlBuff);
                PyErr_SetString(PyExc_ValueError, msg_non_object);
                return NULL;
            }
            if (xmlBuff != NULL) {
                ret = xmlBuff->buffer->use;
                docPtr = BAD_CAST xmlStrndup(xmlBuff->buffer->content, ret);
                res = PyString_FromStringAndSize((char *)docPtr, ret);
                xmlOutputBufferClose(xmlBuff);
                if (PYTHON_IS_NOT_NONE(res)) {
                    xmlFree(docPtr);
                    xmlFreeDoc(docXml);
                    return res;
                }
            } else {
                xmlFreeDoc(docXml);
                PyErr_SetString(PyExc_ValueError, msg_non_object);
                return NULL;
            }
        }
	}
	xmlFreeDoc(docXml);
	return NULL;
	/* error! not created */
}

static PyObject *
pyxserxmlc14nstrict(PyObject *self, PyObject *args, PyObject *keywds)
{
	PyObject *res = Py_None;
	PyObject *input = (PyObject *)NULL;
	PyListObject *dupItems = (PyListObject *)NULL;

	xmlNodePtr serXml = (xmlNodePtr)NULL;
	xmlNodePtr rootNode = (xmlNodePtr)NULL;
	xmlDocPtr docXml = (xmlDocPtr)NULL;
    xmlChar *xmlBuff = (xmlChar *)NULL;

    static char *kwlist[] = {"obj", "depth", "exc", "com", NULL};
    int py_depth = 999999;
    int py_depth_cnt = 1;
    int py_exc = 0;
    int py_com = 0;
	int ok = -1;
    int ret = 0;

	if (PYTHON_IS_NONE(args)) {
		/* error! don't have arguments */
		PyErr_SetString(PyExc_ValueError, msg_non_object);
		return NULL;
	}
	ok = PyArg_ParseTupleAndKeywords(args, keywds, "Oiii", kwlist,
                                     &input, &py_depth,
                                     &py_exc, &py_com);
	if (!ok) {
		/* error! don't have arguments */
		PyErr_SetString(PyExc_ValueError, msg_non_object);
		return NULL;
	}

    if (PYTHON_IS_NONE(input)) {
		PyErr_SetString(PyExc_ValueError, msg_non_object);
		return NULL;
    }

    if (py_depth == 0) {
        py_depth = 999999;
    }

	dupItems = (PyListObject *)PyList_New(0);

	serXml = pyxser_SerializeXml(input, &docXml, &rootNode,
								 (xmlNodePtr *)NULL, dupItems,
                                 xml_encoding, &py_depth, &py_depth_cnt);

	if (dupItems != (PyListObject *)NULL) {
		Py_DECREF(dupItems);
	}

	if (serXml != (xmlNodePtr)NULL
		&& docXml != (xmlDocPtr)NULL) {
        if ((pyxser_ValidateDocumentC14N(docXml)) == 1) {
            ret = xmlC14NDocDumpMemory(docXml, NULL, py_exc, NULL, py_com, &xmlBuff);
            if (xmlBuff != NULL && ret > 0) {
                res = PyString_FromStringAndSize((char *)xmlBuff, ret);
                if (PYTHON_IS_NOT_NONE(res)) {
                    xmlFree(xmlBuff);
                    xmlFreeDoc(docXml);
                    return res;
                } else {
                    xmlFree(xmlBuff);
                    xmlFreeDoc(docXml);
                    PyErr_SetString(PyExc_ValueError, msg_non_object);
                    return NULL;
                }
            } else {
                if (xmlBuff != (xmlChar *)NULL) {
                    xmlFree(xmlBuff);
                }
                xmlFreeDoc(docXml);
                PyErr_SetString(PyExc_ValueError, msg_non_object);
                return NULL;
            }
        }
	}
    PyErr_SetString(PyExc_ValueError, msg_non_object);
	xmlFreeDoc(docXml);
	return NULL;
	/* error! not created */
}

static PyObject *
pyxserxml(PyObject *self, PyObject *args, PyObject *keywds)
{
	PyObject *res = Py_None;
	PyObject *input = (PyObject *)NULL;
	PyListObject *dupItems = (PyListObject *)NULL;

	xmlNodePtr serXml = (xmlNodePtr)NULL;
	xmlNodePtr rootNode = (xmlNodePtr)NULL;
	xmlDocPtr docXml = (xmlDocPtr)NULL;
	xmlChar *xmlBuff = (xmlChar *)NULL;

    static char *kwlist[] = {"obj", "enc", "depth", NULL};
    char *py_enc = (char *)NULL;
    char *in_enc = (char *)NULL;
    int py_depth = 0;
    int py_depth_cnt = 1;
	int bufferSize = 0, ok = -1;

	if (PYTHON_IS_NONE(args)) {
		/* error! don't have arguments */
		PyErr_SetString(PyExc_ValueError, msg_non_object);
		return NULL;
	}
	ok = PyArg_ParseTupleAndKeywords(args, keywds, "Osi", kwlist,
                                     &input, &in_enc, &py_depth);
	if (!ok) {
		/* error! don't have arguments */
		PyErr_SetString(PyExc_ValueError, msg_non_object);
		return NULL;
	}

    if (py_depth == 0) {
        py_depth = 999999;
    }

    py_enc = xmlgetencoding(xmlParseCharEncoding(in_enc));
    if (py_enc == (char *)NULL) {
		PyErr_SetString(PyExc_ValueError, msg_non_object);
		return NULL;
    }

    if (PYTHON_IS_NONE(input)) {
		PyErr_SetString(PyExc_ValueError, msg_non_object);
		return NULL;
    }

	dupItems = (PyListObject *)PyList_New(0);

	serXml = pyxser_SerializeXml(input, &docXml, &rootNode,
								 (xmlNodePtr *)NULL, dupItems,
                                 py_enc, &py_depth, &py_depth_cnt);

	if (dupItems != (PyListObject *)NULL) {
		Py_DECREF(dupItems);
	}

	if (serXml != (xmlNodePtr)NULL
		&& docXml != (xmlDocPtr)NULL) {
        if ((pyxser_ValidateDocument(docXml)) == 1) {
            xmlDocDumpFormatMemoryEnc(docXml, &xmlBuff, &bufferSize,
                                      py_enc, 1);
            if (xmlBuff != BAD_CAST NULL) {
                res = PyString_FromStringAndSize((char *)xmlBuff,
                                                 bufferSize);
                if (PYTHON_IS_NOT_NONE(res)) {
                    xmlFree(xmlBuff);
                    xmlFreeDoc(docXml);
                    return res;
                } else {
                    xmlFree(xmlBuff);
                    xmlFreeDoc(docXml);
                    PyErr_SetString(PyExc_ValueError, msg_non_object);
                    return NULL;
                }
            } else {
                xmlFreeDoc(docXml);
                PyErr_SetString(PyExc_ValueError, msg_non_object);
                return NULL;
            }
        } else {
            xmlFreeDoc(docXml);
            PyErr_SetString(PyExc_ValueError, msg_non_object);
            return NULL;
        }
	}
	xmlFreeDoc(docXml);
	return NULL;
	/* error! not created */
}

static PyObject *
pyxunserxml(PyObject *self, PyObject *args, PyObject *keywds)
{
	PyObject *res = Py_None;
	PyObject *input = (PyObject *)NULL;
	PyObject *tree = (PyObject *)NULL;
	PyObject *current = (PyObject *)NULL;
	PyDictObject *dupItems = (PyDictObject *)NULL;

	xmlNodePtr rootNode = (xmlNodePtr)NULL;
	xmlNodePtr currentNode = (xmlNodePtr)NULL;
	xmlDocPtr docXml = (xmlDocPtr)NULL;

    static char *kwlist[] = {"obj", "enc", NULL};
    char *py_enc = (char *)NULL;
    int py_depth = 0;

	PythonUnserializationArguments obj;

	int ok = -1;
	if (PYTHON_IS_NONE(args)) {
		/* error! don't have arguments */
		PyErr_SetString(PyExc_ValueError, msg_non_object);
		return res;
	}
	ok = PyArg_ParseTupleAndKeywords(args, keywds, "Os", kwlist,
                                     &input, &py_enc);
	if (!ok) {
		/* error! don't have arguments */
		PyErr_SetString(PyExc_ValueError, msg_non_object);
		return NULL;
	}

    if (PYTHON_IS_NONE(pyxser_modules)) {
        pyxser_modules = (PyDictObject *)PyDict_New();
    }

    if (py_enc == (char *)NULL) {
		PyErr_SetString(PyExc_ValueError, msg_non_object);
		return NULL;
    }

    if (PYTHON_IS_NONE(input)) {
		PyErr_SetString(PyExc_ValueError, msg_non_object);
		return NULL;
    }

	obj.doc = &input;
	obj.current = &current;
	obj.tree = &tree;
	obj.dups = &dupItems;
	obj.modules = &pyxser_modules;
	obj.docPtr = &docXml;
	obj.rootNode = &rootNode;
	obj.currentNode = &currentNode;
    obj.encoding = py_enc;
    obj.depth = py_depth;
    obj.depthcnt = 0;

	res = pyxser_UnserializeXml(&obj);
    if (res == NULL) {
		PyErr_SetString(PyExc_ValueError, msg_non_object);
        return NULL;
    }
    return res;
}

static PyObject *
pyxgetdtd(PyObject *self, PyObject *args)
{
	PyObject *res;
	res = PyString_FromString((const char *)pyxser_xml_dtd_location);
	return res;
}

static PyObject *
pyxvalidate(PyObject *self, PyObject *args, PyObject *keywds)
{
	xmlDocPtr docPtr = (xmlDocPtr)NULL;
	PyObject *res = Py_False;
	char *input = (char *)NULL;
	char *docstr = (char *)NULL;
    int parseopts = XML_PARSE_RECOVER;

    static char *kwlist[] = {"obj", "enc", NULL};
    char *py_enc = (char *)NULL;

	int ok = -1;
	if (PYTHON_IS_NONE(args)) {
		/* error! don't have arguments */
		PyErr_SetString(PyExc_ValueError, msg_non_object);
		return NULL;
	}
	ok = PyArg_ParseTupleAndKeywords(args, keywds, "Os", kwlist,
                                     &input, &py_enc);
	if (!ok) {
		/* error! don't have arguments */
		PyErr_SetString(PyExc_ValueError, msg_non_object);
		return NULL;
	}

    py_enc = xmlgetencoding(xmlParseCharEncoding(py_enc));
    if (py_enc == (char *)NULL) {
        py_enc = (char *)xml_encoding;
    }

    docstr = PyString_AS_STRING(input);
	docPtr = xmlReadMemory(docstr, strlen(docstr), NULL,
                           (const char *)py_enc, parseopts);
	if (docPtr != (xmlDocPtr)NULL) {
		if ((pyxser_ValidateDocument(docPtr)) == 1) {
			res = Py_True;
		}
        xmlFreeDoc(docPtr);
	} else {
		PyErr_SetString(PyExc_ValueError, msg_non_xml);
		return NULL;
    }
    return res;
}

static PyObject *
pyxvalidatec14n(PyObject *self, PyObject *args, PyObject *keywds)
{
	xmlDocPtr docPtr = (xmlDocPtr)NULL;
	PyObject *res = Py_False;
	char *input = (char *)NULL;
	char *docstr = (char *)NULL;
    int parseopts = XML_PARSE_RECOVER;

    static char *kwlist[] = {"obj", "enc", NULL};
    char *py_enc = (char *)NULL;

	int ok = -1;
	if (PYTHON_IS_NONE(args)) {
		/* error! don't have arguments */
		PyErr_SetString(PyExc_ValueError, msg_non_object);
		return NULL;
	}
	ok = PyArg_ParseTupleAndKeywords(args, keywds, "Os", kwlist,
                                     &input, &py_enc);
	if (!ok) {
		/* error! don't have arguments */
		PyErr_SetString(PyExc_ValueError, msg_non_object);
		return NULL;
	}

    py_enc = xmlgetencoding(xmlParseCharEncoding(py_enc));
    if (py_enc == (char *)NULL) {
        py_enc = (char *)xml_encoding;
    }

    docstr = PyString_AS_STRING(input);
	docPtr = xmlReadMemory(docstr, strlen(docstr), NULL,
                           (const char *)py_enc, parseopts);
	if (docPtr != (xmlDocPtr)NULL) {
		if ((pyxser_ValidateDocumentC14N(docPtr)) == 1) {
			res = Py_True;
		}
        xmlFreeDoc(docPtr);
	} else {
		PyErr_SetString(PyExc_ValueError, msg_non_xml);
		return NULL;
    }
	return res;
}

static PyObject *
xmlcleanup(PyObject *self, PyObject *args)
{
    if (PYTHON_IS_NOT_NONE(pyxser_modules)) {
        Py_DECREF(pyxser_modules);
        pyxser_modules = NULL;
    }
    xmlFreeDtd(pyxser_dtd_object);
    pyxser_dtd_object = NULL;
    xmlFreeDtd(pyxser_dtd_c14n_object);
    pyxser_dtd_c14n_object = NULL;
    xmlCleanupParser();
    xmlCleanupEncodingAliases();
    pyxser_register_encodings();
    Py_INCREF(Py_None);
    return Py_None;
}


void
pyxser_unregister(void)
{
    if (PYTHON_IS_NOT_NONE(pyxser_modules)) {
        Py_DECREF(pyxser_modules);
        pyxser_modules = NULL;
    }
    xmlFreeDtd(pyxser_dtd_object);
    xmlCleanupEncodingAliases();
    xmlCleanupParser();
}


void
pyxser_register_encodings(void)
{
    int c;
    for (c = 0; pyxser_encodings[c].enc_f != NULL
             && pyxser_encodings[c].enc_t != NULL; c++) {
        xmlAddEncodingAlias(pyxser_encodings[c].enc_f,
                            pyxser_encodings[c].enc_t);
    }
}


static char *
xmlgetencoding(int id)
{
    switch (id) {
    case XML_CHAR_ENCODING_ERROR:
    case XML_CHAR_ENCODING_NONE:
    case XML_CHAR_ENCODING_ASCII:
        return "ASCII";
    case XML_CHAR_ENCODING_UTF8:
        return "UTF-8";
    case XML_CHAR_ENCODING_UTF16LE:
    case XML_CHAR_ENCODING_UTF16BE:
        return "UTF-16";
    case XML_CHAR_ENCODING_UCS4LE:
    case XML_CHAR_ENCODING_UCS4BE:
    case XML_CHAR_ENCODING_UCS4_2143:
    case XML_CHAR_ENCODING_UCS4_3412:
        return "UCS-4";
    case XML_CHAR_ENCODING_EBCDIC:
        return "EBCDIC";
    case XML_CHAR_ENCODING_UCS2:
        return "UCS-2";
    case XML_CHAR_ENCODING_8859_1:
        return "ISO-8859-1";
    case XML_CHAR_ENCODING_8859_2:
        return "ISO-8859-2";
    case XML_CHAR_ENCODING_8859_3:
        return "ISO-8859-3";
    case XML_CHAR_ENCODING_8859_4:
        return "ISO-8859-4";
    case XML_CHAR_ENCODING_8859_5:
        return "ISO-8859-5";
    case XML_CHAR_ENCODING_8859_6:
        return "ISO-8859-6";
    case XML_CHAR_ENCODING_8859_7:
        return "ISO-8859-7";
    case XML_CHAR_ENCODING_8859_8:
        return "ISO-8859-8";
    case XML_CHAR_ENCODING_8859_9:
        return "ISO-8859-9";
    case XML_CHAR_ENCODING_SHIFT_JIS:
        return "SHIFT-JIS";
    case XML_CHAR_ENCODING_2022_JP:
        return "ISO-2022-JP";
    case XML_CHAR_ENCODING_EUC_JP:
        return "EUC-JP";
    default:
        return (char *)NULL;
    }
}


/* pyserx.c ends here */
