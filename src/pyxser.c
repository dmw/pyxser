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
#include "include/pyxser_serializer.h"

#ifdef PYXSER_DEBUG
#warning USING DEBUG!
#endif /* !PYXSER_DEBUG */

const char msg_non_arguments[] = \
	"No arguments given.";

const char msg_wrong_argument[] = \
	"Wrong argument types or invalid number of arguments.";

const char msg_non_object[] = \
	"Argument given for serialization/deserialization is "
    "not a Python Object or is None.";

const char msg_non_method[] = \
	"Invalid call, been called as object.";

const char msg_non_xml[] = \
	"Invalid XML, please read the documentation.";

const char msg_non_canon_encoding[] = \
	"Invalid encoding, canonical XML uses UTF-8.";

const char msg_invalid_encoding[] = \
    "Invalid encoding, can not use this encoding.";

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

const char pyxser_module_name[] = "pyxser";
const char pyxser_xml_attr_id[] = "objid";
const char pyxser_xml_attr_item[] = "prop";
const char pyxser_xml_attr_module[] = "module";
const char pyxser_xml_attr_name[] = "name";
const char pyxser_xml_attr_ns_prefix[] = "pyxs";
const char pyxser_xml_attr_ref[] = "objref";
const char pyxser_xml_attr_size[] = "size";
const char pyxser_xml_attr_type[] = "type";
const char pyxser_xml_attr_version[] = "version";
const char pyxser_xml_attr_key[] = "key";

const char pyxser_xml_element_collection[] = "col";
const char pyxser_xml_element_object[] = "obj";
const char pyxser_xml_element_prop[] = "prop";

const char pyxser_xml_encoding[] = "utf-8";
const char pyxser_xml_encoding_mode[] = "strict";

const char pyxser_xml_attr_xml_ns[] = "xmlns:pyxs";
const char pyxser_xml_attr_xml_xsd_prfx[] = "xmlns";
const char pyxser_xml_attr_xml_xsi[] = "xmlns:xsi";
const char pyxser_xml_attr_xml_xloc[] = "xmlns:schemaLocation";

const char pyxser_xml_version[] = "1.0";
const char pyxser_ext_version[] = "1.6-r1";
const char pyxser_ext_author[] = "Daniel Molina Wegener <dmw@coder.cl>";
const char pyxser_ext_site[] = "http://coder.cl/products/pyxser/";

const char pyxser_xml_attr_ns[] = "http://projects.coder.cl/pyxser/model/";
const char pyxser_xml_attr_xml_xsd[] = "http://www.w3.org/2000/xmlns/";
const char pyxser_xml_attr_xml_xsi_value[] = "http://www.w3.org/2001/XMLSchema-instance";
const char pyxser_xml_attr_xml_xloc_value[] = "http://projects.coder.cl/pyxser/dtd/pyxser-1.0.xsd";

const char pyxser_xml_dtd_location[] = PYXSER_DTD_FILE;
const char pyxser_xml_dtd_c14n_location[] = PYXSER_DTD_C14N_FILE;
const char pyxser_xml_xsd_location[] = PYXSER_XSD_FILE;
const char pyxser_xml_xsd_c14n_location[] = PYXSER_XSD_C14N_FILE;

extern xmlNsPtr pyxser_default_ns;
extern xmlNsPtr pyxser_xsi_ns;
extern xmlNsPtr pyxser_xsd_ns;

static PyObject *pyxserxml(PyObject *self, PyObject *args, PyObject *keywds);
static PyObject *pyxserxmlc14n(PyObject *self, PyObject *args, PyObject *keywds);
static PyObject *pyxserxmlc14nstrict(PyObject *self, PyObject *args, PyObject *keywds);

static PyObject *u_pyxserxml(PyObject *self, PyObject *args, PyObject *keywds);
static PyObject *u_pyxserxmlc14n(PyObject *self, PyObject *args, PyObject *keywds);
static PyObject *u_pyxserxmlc14nstrict(PyObject *self, PyObject *args, PyObject *keywds);

static PyObject *pyxunserxml(PyObject *self, PyObject *args, PyObject *keywds);
static PyObject *u_pyxunserxml(PyObject *self, PyObject *args, PyObject *keywds);

static PyObject *pyxvalidate(PyObject *self, PyObject *args, PyObject *keywds);
static PyObject *pyxvalidatec14n(PyObject *self, PyObject *args, PyObject *keywds);

static PyObject *u_pyxvalidate(PyObject *self, PyObject *args, PyObject *keywds);
static PyObject *u_pyxvalidatec14n(PyObject *self, PyObject *args, PyObject *keywds);

static PyObject *pyxvalidatedtd(PyObject *self, PyObject *args, PyObject *keywds);
static PyObject *pyxvalidatec14ndtd(PyObject *self, PyObject *args, PyObject *keywds);

static PyObject *u_pyxvalidatedtd(PyObject *self, PyObject *args, PyObject *keywds);
static PyObject *u_pyxvalidatec14ndtd(PyObject *self, PyObject *args, PyObject *keywds);

static PyObject *pyxgetdtd(PyObject *self, PyObject *args);
static PyObject *pyxgetdtdc14n(PyObject *self, PyObject *args);
static PyObject *pyxgetxsd(PyObject *self, PyObject *args);
static PyObject *pyxgetxsdc14n(PyObject *self, PyObject *args);
static PyObject *xmlcleanup(PyObject *self, PyObject *args);
static char *xmlgetencoding(int id);


PyObject *invalid_encoding_exception;
PyObject *invalid_xml_exception;
PyObject *invalid_argument_exception;
PyObject *pyxser_version_obj = (PyObject *)NULL;
PyObject *pyxser_author_obj = (PyObject *)NULL;
PyObject *pyxser_site_obj = (PyObject *)NULL;
PyObject *pyxser_doc_obj = (PyObject *)NULL;
PyObject *pyxstr_dtd = (PyObject *)NULL;
PyObject *pyxstr_dtd_c14n = (PyObject *)NULL;
PyObject *pyxstr_xsd = (PyObject *)NULL;
PyObject *pyxstr_xsd_c14n = (PyObject *)NULL;

static PyDictObject *pyxser_modules = (PyDictObject *)NULL;
void pyxser_unregister(void);
void *dummy(PyObject *obj);

typedef struct pyxser_encodings_s {
    const char *enc_f;
    const char *enc_t;
} pyxser_encodings_t;


static const char pyxser_module_doc[] = \
    "This module serializes Python Objects into XML. It uses a model\n"
    "based serialization. The serialization model is written under\n"
    "a XML Schema definition and Document Type Definition (DTD).\n"
    "\n"
    "pyxser supports any Python supported encoding. So, if you plan\n"
    "to serialize old charsets like iso-8859-1, you must not pass\n"
    "unicode strings to the serializer, instead you must create a type\n"
    "map which converts utf-8 strings to encoded iso-8859-1 strings\n"
    "using { 'unicode': to_encoded_latin1 }, with to_encoded_latin1\n"
    "as a function executing unicode.encode('iso-8859-1')\n"
    "\n"
    "* You can download the serialization model in the form of DTD\n"
    "from the following URL:\n"
    "\thttp://projects.coder.cl/pyxser/dtd/pyxser-1.0.dtd\n"
    "* You can download the serialization model in the form of XML\n"
    "Schema from the following URL:\n"
    "\thttp://projects.coder.cl/pyxser/dtd/pyxser-1.0.xsd\n"
    "* You can download the C14N compatible DTD form the following\n"
    "URL:\n"
    "\thttp://projects.coder.cl/pyxser/dtd/pyxser-1.0-c14n.dtd\n"
    "* You can download the C14N compatible XML Schema from the\n"
    "following URL:\n"
    "\thttp://projects.coder.cl/pyxser/dtd/pyxser-1.0-c14n.xsd\n"
    "\n\n"
    "_pyxser selectors_ are made to select which members of python\n"
    "objects are being serialized, so you create a function which\n"
    "receives a Python object, and it must return dictionary of\n"
    "{ 'attribute': value, 'attr2': value } which will be serialized\n"
    "by pyxser and you can include many attributes as you want\n"
    "\n"
    "_pyxser type maps_ are made to specify a serializer on pyxser\n"
    "so you an create custom serialization for certain types on\n"
    "your application, for example if you want to serialize a file\n"
    "as Base 64 encoded string, you must create a function which\n"
    "receives the file object and returns the Base 64 encoded\n"
    "string, and the put that key/value pair on a dictionary and\n"
    "pass that dictionary with { 'file': to_base64_function } as\n"
    "pyxser type map, and you can include many types as you want.\n"
    "\n\n";

static const char serialize_documentation[] = \
    "Uses the next keyword argumens:\n"
    "   obj      ->      python object to serialize\n"
    "   enc      ->      xml serialization encoding\n"
    "   depth    ->      node serialization depth\n"
    "                    [optional, default 50]\n"
    "   selector ->      attribute selector function, it must\n"
    "                    receive the object to serialize and return a\n"
    "                    dictionary of strings containing attribute names\n"
    "                    pointing to the attributes being serialized.\n"
    "                    [optional, default None]\n"
    "   typemap  ->      a dictionary containing {'type': callback}\n"
    "                    entries, where type is __class__.__name__\n"
    "                    attribute and the callback is a serialization\n"
    "                    function which must return a string as result.\n"
    "                    [optional, default None]\n\n"
	"Gets any object defined in a Python module as class as argument\n"
	"and serializes it as XML. The serialization model is based on the\n"
	"pyxser DTD/XML Schema 1.0. Objects are serialized as pyxs:obj\n"
    "element, collections are serialized as pyxs:col element (tuples,\n"
    "lists and dictionaries) and properties are serialized as pyxs:prop\n"
    "elements.\n\n"
	"The serialization algorithm is a O(n) one, this means which the\n"
	"serializer runs over the object tree just one time and cross\n"
	"refernces are serialized as XML references through the IDREF\n"
	"attribute pyxs:ref.\n\n"
	"The serialization model resides in the pyxser public identifier DTD:\n"
	"    <!DOCTYPE pyxs:obj\n"
    "              PUBLIC '-//coder.cl//DTD pyxser 1.0//EN'\n"
    "             'http://projects.coder.cl/pyxser/dtd/pyxser-1.0.dtd'>\n\n"
    "Sample Selector (sel):\n\n"
    "def sel_filter(v):\n"
    "    r = ((not callable(v[1])) and (not v[0].startswith(\"__\")))\n"
    "    return r\n\n"
    "def sel(o):\n"
    "    r = dict(filter(sel_filter, inspect.getmembers(o)))\n"
    "    return r\n";

static const char serializec14n_documentation[] = \
    "Uses the next keyword argumens:\n"
    "   obj      ->      python object to serialize\n"
    "   depth    ->      node navigation depth\n"
    "                    [optional, default 50]\n\n"
    "   com      ->      with comments\n\n"
    "                    [optional, default 0 (false)]\n\n"
    "   selector ->      attribute selector function, it must\n"
    "                    receive the object to serialize and return a\n"
    "                    dictionary of strings containing attribute names\n"
    "                    pointing to the attributes being serialized.\n"
    "   typemap  ->      a dictionary containing {'type': callback}\n"
    "                    entries, where type is __class__.__name__\n"
    "                    attribute and the callback is a serialization\n"
    "                    function which must return a string as result.\n"
    "                    [optional, default None]\n\n"
    "Same as serialize(), but uses C14N canonization, to use exclusive\n"
    "canonization the 'exc' argument must differ from zero and to use\n"
    "comments 'com' must differ from zero. The encoding must be UTF-8\n"
    "for canonization.\n\n"
	"    <!DOCTYPE pyxs:obj\n"
    "              PUBLIC '-//coder.cl//DTD pyxser C14N 1.0//EN'\n"
    "             'http://projects.coder.cl/pyxser/dtd/pyxser-1.0-c14n.dtd'>\n\n"
    "* Information about Canonical XML at:\n"
    "\thttp://www.w3.org/TR/xml-c14n\n"
    "* Information about Exclusive Canonical XML at:\n"
    "\thttp://www.w3.org/TR/xml-exc-c14n\n"
    "NOTE: The canonical DTD converts all ID, IDREF and NMTOKEN\n"
    "      attributes to CDATA attributes\n";

static const char serializec14n_documentation_strict[] = \
    "Uses the next keyword argumens:\n"
    "   obj      ->      python object to serialize\n"
    "   depth    ->      node navigation depth\n"
    "                    [optional, default 50]\n\n"
    "   com      ->      with comments\n\n"
    "                    [optional, default 0 (false)]\n\n"
    "   selector ->      attribute selector function, it must\n"
    "                    receive the object to serialize and return a\n"
    "                    dictionary of strings containing attribute names\n"
    "                    pointing to the attributes being serialized.\n"
    "   typemap  ->      a dictionary containing {'type': callback}\n"
    "                    entries, where type is __class__.__name__\n"
    "                    attribute and the callback is a serialization\n"
    "                    function which must return a string as result.\n"
    "                    [optional, default None]\n\n"
    "Same as serialize_c14n(), but uses C14N canonization in a strict\n"
    "mode and rather than serializing NMTOKEN, ID, and IDREF attributes,\n"
    "uses the C14N canon to execute the XML tree rendering. The encoding\n"
    "must be UTF-8 for canonization.\n\n"
	"    <!DOCTYPE pyxs:obj\n"
    "              PUBLIC '-//coder.cl//DTD pyxser C14N 1.0//EN'\n"
    "             'http://projects.coder.cl/pyxser/dtd/pyxser-1.0.dtd'>\n\n"
    "* Information about Canonical XML at:\n"
    "\thttp://www.w3.org/TR/xml-c14n\n"
    "* Information about Exclusive Canonical XML at\n"
    "\thttp://www.w3.org/TR/xml-exc-c14n\n"
    "NOTE: C14N serialized objects can not be deserialized because we\n"
    "      need the ID and IDREF attributes to suppor cross referenced\n"
    "      objects.\n";

static const char u_serialize_documentation[] = \
    "The same as serialize() but returns a Python unicode\n"
    "object.\n";

static const char u_serializec14n_documentation[] = \
    "The same as serialize_c14n() but returns a Python unicode\n"
    "object.\n";

static const char u_serializec14n_documentation_strict[] = \
    "The same as serialize_c14n_strict() but returns a Python\n"
    "unicode object.\n";

static const char deserialize_documentation[] = \
    "Uses the next keyword argumens:\n"
    "   obj     ->      the serialized python object\n"
    "   enc     ->      xml serialization encoding\n"
    "   typemap ->      a dictionary containing {'type': callback}\n"
    "                   entries, where type is __class__.__name__\n"
    "                   attribute and the callback is a deserialization\n"
    "                   function which must return the original type as\n"
    "                   result.\n"
    "                   [optional, default None]\n"
    "   cinit   ->      if it is False, the deserialization will not create\n"
    "                   objects using their default constructor, instead will\n"
    "                   create objects using the raw instance, without\n"
    "                   initialization.\n\n"
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
    "The typemap argument, holds a dictionary with {'type': callback_contructor}\n"
    "which uses the XML node content as argument for callback_constructor\n"
    "so you can create a map for serialized types which needs a special\n"
    "constructor. The callback_constructor is called as:\n"
    "   callback_constructor(node->content, current_deserialized_object)\n\n"
	"Every serilized object is validated against the pyxser XML Schema 1.0\n\n";

static const char u_deserialize_documentation[] = \
    "Same as unserialize() but uses an Python unicode object as input\n";

static const char validate_documentation[] = \
    "Uses the next keyword argumens:\n"
    "   obj     ->      the serialized python object\n"
    "   enc     ->      xml serialization encoding\n\n"
	"Validates the XML input string against the pyxser XML Schema 1.0\n"
    "in your local filesystem.\n\n";

static const char validate_documentation_c14n[] = \
    "Uses the next keyword argumens:\n"
    "   obj     ->      the serialized python object\n"
    "   enc     ->      xml serialization encoding\n\n"
	"Validates the XML input string against the pyxser XML Schema C14N\n"
    "1.0 in your local filesystem.\n\n";

static const char validate_documentation_dtd[] = \
    "Uses the next keyword argumens:\n"
    "   obj     ->      the serialized python object\n"
    "   enc     ->      xml serialization encoding\n\n"
	"Validates the XML input string against the pyxser DTD 1.0\n"
    "in your local filesystem.\n\n";

static const char validate_documentation_c14n_dtd[] = \
    "Uses the next keyword argumens:\n"
    "   obj     ->      the serialized python object\n"
    "   enc     ->      xml serialization encoding\n\n"
	"Validates the XML input string against the pyxser DTD C14N\n"
    "1.0 in your local filesystem.\n\n";

static const char u_validate_documentation[] = \
    "Same as validate() but uses a Python unicode object\n"
    "as input\n";

static const char u_validate_documentation_c14n[] = \
    "Same as validate_c14n() but uses a Python unicode object\n"
    "as input\n";

static const char u_validate_documentation_dtd[] = \
    "Same as validate_dtd() but uses a Python unicode object\n"
    "as input\n";

static const char u_validate_documentation_c14n_dtd[] = \
    "Same as validate_c14n_dtd() but uses a Python unicode object\n"
    "as input\n";

static const char getdtd_documentation[] = \
	"This function returns the pyxser 1.0 DTD location in your\n"
    "system as string\n\n";

static const char getdtd_c14n_documentation[] = \
	"This function returns the pyxser 1.0 DTD for C14N location\n"
    "in your system as string\n\n";

static const char getxsd_documentation[] = \
	"This function returns the pyxser 1.0 XML Schema location in\n"
    "your system as string\n\n";

static const char getxsd_c14n_documentation[] = \
	"This function returns the pyxser 1.0 XML Schema for C14N\n"
    "location in your system as string\n\n";

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

    {"u_serialize", (PyCFunction)u_pyxserxml,
     METH_VARARGS | METH_KEYWORDS,
     u_serialize_documentation},

    {"u_serialize_c14n", (PyCFunction)u_pyxserxmlc14n,
     METH_VARARGS | METH_KEYWORDS,
     u_serializec14n_documentation},

    {"u_serialize_c14n_strict", (PyCFunction)u_pyxserxmlc14nstrict,
     METH_VARARGS | METH_KEYWORDS,
     u_serializec14n_documentation_strict},

    {"unserialize", (PyCFunction)pyxunserxml,
     METH_VARARGS | METH_KEYWORDS,
     deserialize_documentation},

    {"u_unserialize", (PyCFunction)u_pyxunserxml,
     METH_VARARGS | METH_KEYWORDS,
     u_deserialize_documentation},

    {"validate", (PyCFunction)pyxvalidate,
     METH_VARARGS | METH_KEYWORDS,
     validate_documentation},

    {"validate_c14n", (PyCFunction)pyxvalidatec14n,
     METH_VARARGS | METH_KEYWORDS,
     validate_documentation_c14n},

    {"u_validate", (PyCFunction)u_pyxvalidate,
     METH_VARARGS | METH_KEYWORDS,
     u_validate_documentation},

    {"u_validate_c14n", (PyCFunction)u_pyxvalidatec14n,
     METH_VARARGS | METH_KEYWORDS,
     u_validate_documentation_c14n},

    {"validate_dtd", (PyCFunction)pyxvalidatedtd,
     METH_VARARGS | METH_KEYWORDS,
     validate_documentation_dtd},

    {"validate_c14n_dtd", (PyCFunction)pyxvalidatec14ndtd,
     METH_VARARGS | METH_KEYWORDS,
     validate_documentation_c14n_dtd},

    {"u_validate_dtd", (PyCFunction)u_pyxvalidatedtd,
     METH_VARARGS | METH_KEYWORDS,
     u_validate_documentation_dtd},

    {"u_validate_c14n_dtd", (PyCFunction)u_pyxvalidatec14ndtd,
     METH_VARARGS | METH_KEYWORDS,
     u_validate_documentation_c14n_dtd},

    {"getdtd", pyxgetdtd, METH_VARARGS, getdtd_documentation},
    {"getdtd_c14n", pyxgetdtdc14n, METH_VARARGS, getdtd_c14n_documentation},
    {"getxsd", pyxgetxsd, METH_VARARGS, getxsd_documentation},
    {"getxsd_c14n", pyxgetxsdc14n, METH_VARARGS, getxsd_c14n_documentation},

    {"xmlcleanup", xmlcleanup, METH_VARARGS, xmlcleanup_documentation},

    {NULL, NULL, 0, NULL}
};

#if PY_MAJOR_VERSION >= 3

static struct PyModuleDef pyxser_module_def = {
    PyModuleDef_HEAD_INIT,
    pyxser_module_name,
    pyxser_module_doc,
    -1,
    serxMethods,
    NULL,
    NULL,
    NULL,
    pyxser_unregister
};

PyMODINIT_FUNC
PyInit_pyxser(void)
{
    PyObject *m;
    m = PyModule_Create(&pyxser_module_def);
    if (m == NULL)
        return NULL;
	/* init module */
    Py_AtExit(pyxser_unregister);

    /* we add the version */
    pyxser_version_obj = PyString_FromString(pyxser_ext_version);
    PyModule_AddObject(m, "__version__", pyxser_version_obj);

    /* we add the author */
    pyxser_author_obj = PyString_FromString(pyxser_ext_author);
    PyModule_AddObject(m, "__author__", pyxser_author_obj);

    /* we add the site */
    pyxser_site_obj = PyString_FromString(pyxser_ext_site);
    PyModule_AddObject(m, "__site__", pyxser_site_obj);

    LIBXML_TEST_VERSION;

    xmlInitParser();
    return m;
}
#endif
#if PY_MAJOR_VERSION == 2
PyMODINIT_FUNC
initpyxser(void)
{
    PyObject *m;
    m = Py_InitModule("pyxser", serxMethods);
    if (m == NULL)
        return;
	/* init module */
    Py_AtExit(pyxser_unregister);

    /* we add the version */
    pyxser_version_obj = PyString_FromString(pyxser_ext_version);
    PyModule_AddObject(m, "__version__", pyxser_version_obj);

    /* we add the author */
    pyxser_author_obj = PyString_FromString(pyxser_ext_author);
    PyModule_AddObject(m, "__author__", pyxser_author_obj);

    /* we add the site */
    pyxser_site_obj = PyString_FromString(pyxser_ext_site);
    PyModule_AddObject(m, "__site__", pyxser_site_obj);

    /* we add the site */
    pyxser_doc_obj = PyString_FromString(pyxser_module_doc);
    PyModule_AddObject(m, "__doc__", pyxser_doc_obj);

    LIBXML_TEST_VERSION;

    xmlInitParser();
}
#endif

static PyObject *
pyxserxml(PyObject *self, PyObject *args, PyObject *keywds)
{
	PyObject *res = (PyObject *)NULL;
	PyObject *input = (PyObject *)NULL;
    PyObject *selector = (PyObject *)NULL;
    PyObject *typemap = (PyObject *)NULL;
	PyListObject *dupItems = (PyListObject *)NULL;

	xmlNodePtr serXml = (xmlNodePtr)NULL;
	xmlNodePtr rootNode = (xmlNodePtr)NULL;
	xmlDocPtr docXml = (xmlDocPtr)NULL;
	xmlChar *xmlBuff = (xmlChar *)NULL;

    PythonSerializationArguments sargs;

    static char *kwlist[] = {"obj", "enc", "depth", "selector",
                             "typemap", NULL};
    char *py_enc = (char *)NULL;
    char *in_enc = (char *)NULL;
    int py_depth = 0;
    int py_depth_cnt = 1;
	int bufferSize = 0, ok = -1;

	if (PYTHON_IS_NONE(args)) {
		/* error! don't have arguments */
		PyErr_SetString(PyExc_ValueError, msg_non_arguments);
		return NULL;
	}

	ok = PyArg_ParseTupleAndKeywords(args, keywds, "Os|iOO", kwlist,
                                     &input, &in_enc, &py_depth,
                                     &selector, &typemap);
	if (!ok) {
		/* error! don't have arguments */
		PyErr_SetString(PyExc_ValueError, msg_wrong_argument);
		return NULL;
	}

    if (py_depth == 0) {
        py_depth = 50;
    }

    py_enc = xmlgetencoding(xmlParseCharEncoding(in_enc));

    if (py_enc == (char *)NULL) {
		PyErr_SetString(PyExc_ValueError, msg_invalid_encoding);
		return NULL;
    }

    if (PYTHON_IS_NONE(input)) {
		PyErr_SetString(PyExc_ValueError, msg_non_object);
		return NULL;
    }

	dupItems = (PyListObject *)PyList_New(0);
    Py_XINCREF(input);

    sargs.o = &input;
    sargs.docptr = &docXml;
    sargs.rootNode = &rootNode;
    sargs.currentNode = &rootNode;
    sargs.dupSrcItems = &dupItems;
    sargs.enc = py_enc;
    sargs.depth = &py_depth;
    sargs.depthcnt = &py_depth_cnt;
    sargs.selector = &selector;
    sargs.typemap = typemap;

	serXml = pyxser_SerializeXml(&sargs);

    Py_XDECREF(input);
    Py_XDECREF(dupItems);

    if (docXml == (xmlDocPtr)NULL) {
        PyErr_SetString(PyExc_ValueError, msg_non_object);
        return NULL;
    }

    if (serXml == (xmlNodePtr)NULL) {
        xmlFreeDoc(docXml);
        PyErr_SetString(PyExc_ValueError, msg_non_object);
        return NULL;
    }

    xmlDocDumpFormatMemoryEnc(docXml, &xmlBuff, &bufferSize,
                              py_enc, 1);

    if (xmlBuff == BAD_CAST NULL) {
        xmlFreeDoc(docXml);
        PyErr_SetString(PyExc_ValueError, msg_non_object);
        return NULL;
    }
    res = PyString_FromStringAndSize((const char *)xmlBuff,
                                     bufferSize);
    if (PYTHON_IS_NOT_NONE(res)) {
        xmlFree(xmlBuff);
        xmlFreeDoc(docXml);
        PyErr_Clear();
        return res;
    } else {
        xmlFree(xmlBuff);
        xmlFreeDoc(docXml);
        PyErr_SetString(PyExc_ValueError, msg_non_object);
        return NULL;
	}
}

static PyObject *
pyxserxmlc14n(PyObject *self, PyObject *args, PyObject *keywds)
{
	PyObject *res = (PyObject *)NULL;
	PyObject *input = (PyObject *)NULL;
    PyObject *selector = (PyObject *)NULL;
    PyObject *typemap = (PyObject *)NULL;
	PyListObject *dupItems = (PyListObject *)NULL;

	xmlNodePtr serXml = (xmlNodePtr)NULL;
	xmlNodePtr rootNode = (xmlNodePtr)NULL;
	xmlDocPtr docXml = (xmlDocPtr)NULL;
    xmlOutputBufferPtr xmlBuff = (xmlOutputBufferPtr)NULL;
    xmlChar *docPtr = (xmlChar *)NULL;

    PythonSerializationArguments sargs;

    static char *kwlist[] = {"obj", "depth", "com", "selector",
                             "typemap", NULL};
    int py_depth = 999999;
    int py_depth_cnt = 1;
    int py_exc = 0;
    int py_com = 0;
	int ok = -1;
    int ret = 0;

	if (PYTHON_IS_NONE(args)) {
		/* error! don't have arguments */
		PyErr_SetString(PyExc_ValueError, msg_non_arguments);
		return NULL;
	}

	ok = PyArg_ParseTupleAndKeywords(args, keywds, "O|iiO", kwlist,
                                     &input, &py_depth, &py_com,
                                     &selector, &typemap);
	if (!ok) {
		/* error! don't have arguments */
		PyErr_SetString(PyExc_ValueError, msg_wrong_argument);
		return NULL;
	}

    if (PYTHON_IS_NONE(input)) {
		PyErr_SetString(PyExc_ValueError, msg_non_object);
		return NULL;
    }

    if (py_depth == 0) {
        py_depth = 50;
    }

	dupItems = (PyListObject *)PyList_New(0);
    Py_XINCREF(input);

    sargs.o = &input;
    sargs.item = &input;
    sargs.docptr = &docXml;
    sargs.rootNode = &rootNode;
    sargs.currentNode = &rootNode;
    sargs.dupSrcItems = &dupItems;
    sargs.enc = (char *)pyxser_xml_encoding;
    sargs.depth = &py_depth;
    sargs.depthcnt = &py_depth_cnt;
    sargs.selector = &selector;
    sargs.typemap = typemap;

	serXml = pyxser_SerializeXml(&sargs);

    Py_XDECREF(input);
    Py_XDECREF(dupItems);

    if (docXml == (xmlDocPtr)NULL) {
        PyErr_SetString(PyExc_ValueError, msg_non_object);
        return NULL;
    }

    if (serXml == (xmlNodePtr)NULL) {
        xmlFreeDoc(docXml);
        PyErr_SetString(PyExc_ValueError, msg_non_object);
        return NULL;
    }

    xmlBuff = xmlAllocOutputBuffer(NULL);
    ret = xmlC14NExecute(docXml, pyxser_C14NRenderAllNodes, docXml,
                         py_exc, NULL, py_com, xmlBuff);
    if (ret < 0) {
        xmlFree(docPtr);
        xmlOutputBufferClose(xmlBuff);
        PyErr_SetString(PyExc_ValueError, msg_non_object);
        return NULL;
    }
    if (xmlBuff == NULL) {
        xmlFree(docPtr);
        PyErr_SetString(PyExc_ValueError, msg_non_object);
        return NULL;
    }
    ret = xmlBuff->buffer->use;
    docPtr = BAD_CAST xmlStrndup(xmlBuff->buffer->content, ret);
    res = PyString_FromStringAndSize((const char *)docPtr, ret);
    xmlOutputBufferClose(xmlBuff);
    if (PYTHON_IS_NOT_NONE(res)) {
        xmlFree(docPtr);
        xmlFreeDoc(docXml);
        PyErr_Clear();
        return res;
    } else {
        xmlFree(docPtr);
        xmlFreeDoc(docXml);
        PyErr_SetString(PyExc_ValueError, msg_non_object);
        return NULL;
    }
	/* error! not created */
}

static PyObject *
pyxserxmlc14nstrict(PyObject *self, PyObject *args, PyObject *keywds)
{
	PyObject *res = (PyObject *)NULL;
	PyObject *input = (PyObject *)NULL;
    PyObject *selector = (PyObject *)NULL;
    PyObject *typemap = (PyObject *)NULL;
	PyListObject *dupItems = (PyListObject *)NULL;

	xmlNodePtr serXml = (xmlNodePtr)NULL;
	xmlNodePtr rootNode = (xmlNodePtr)NULL;
	xmlDocPtr docXml = (xmlDocPtr)NULL;
    xmlChar *xmlBuff = (xmlChar *)NULL;

    PythonSerializationArguments sargs;

    static char *kwlist[] = {"obj", "depth", "com", "selector",
                             "typemap", NULL};
    int py_depth = 999999;
    int py_depth_cnt = 1;
    int py_exc = 0;
    int py_com = 0;
	int ok = -1;
    int ret = 0;

	if (PYTHON_IS_NONE(args)) {
		/* error! don't have arguments */
		PyErr_SetString(PyExc_ValueError, msg_non_arguments);
		return NULL;
	}
	ok = PyArg_ParseTupleAndKeywords(args, keywds, "O|iiOO", kwlist,
                                     &input, &py_depth, &py_com,
                                     &selector, &typemap);
	if (!ok) {
		/* error! don't have arguments */
		PyErr_SetString(PyExc_ValueError, msg_wrong_argument);
		return NULL;
	}

    if (PYTHON_IS_NONE(input)) {
		PyErr_SetString(PyExc_ValueError, msg_non_object);
		return NULL;
    }

    if (py_depth == 0) {
        py_depth = 50;
    }

	dupItems = (PyListObject *)PyList_New(0);
    Py_XINCREF(input);

    sargs.o = &input;
    sargs.docptr = &docXml;
    sargs.rootNode = &rootNode;
    sargs.currentNode = &rootNode;
    sargs.dupSrcItems = &dupItems;
    sargs.enc = (char *)pyxser_xml_encoding;
    sargs.depth = &py_depth;
    sargs.depthcnt = &py_depth_cnt;
    sargs.selector = &selector;
    sargs.typemap = typemap;

	serXml = pyxser_SerializeXml(&sargs);

    Py_XDECREF(input);
    Py_XDECREF(dupItems);

    if (docXml == (xmlDocPtr)NULL) {
        PyErr_SetString(PyExc_ValueError, msg_non_object);
        return NULL;
    }

    if (serXml == (xmlNodePtr)NULL) {
        xmlFreeDoc(docXml);
        PyErr_SetString(PyExc_ValueError, msg_non_object);
        return NULL;
    }

    ret = xmlC14NDocDumpMemory(docXml, NULL, py_exc, NULL,
                               py_com, &xmlBuff);
    if (xmlBuff == NULL || ret <= 0) {
        if (xmlBuff != (xmlChar *)NULL) {
            xmlFree(xmlBuff);
        }
        xmlFreeDoc(docXml);
        PyErr_SetString(PyExc_ValueError, msg_non_object);
        return NULL;
    }
    res = PyString_FromStringAndSize((const char *)xmlBuff, ret);
    if (PYTHON_IS_NOT_NONE(res)) {
        xmlFree(xmlBuff);
        xmlFreeDoc(docXml);
        PyErr_Clear();
        return res;
    } else {
        if (xmlBuff != (xmlChar *)NULL) {
            xmlFree(xmlBuff);
        }
        xmlFreeDoc(docXml);
        PyErr_SetString(PyExc_ValueError, msg_non_object);
        return NULL;
    }
	/* error! not created */
}

static PyObject *
u_pyxserxml(PyObject *self, PyObject *args, PyObject *keywds)
{
	PyObject *res = Py_None;
	PyObject *input = (PyObject *)NULL;
    PyObject *selector = (PyObject *)NULL;
    PyObject *typemap = (PyObject *)NULL;
	PyListObject *dupItems = (PyListObject *)NULL;

	xmlNodePtr serXml = (xmlNodePtr)NULL;
	xmlNodePtr rootNode = (xmlNodePtr)NULL;
	xmlDocPtr docXml = (xmlDocPtr)NULL;
	xmlChar *xmlBuff = (xmlChar *)NULL;

    PythonSerializationArguments sargs;

    static char *kwlist[] = {"obj", "enc", "depth", "selector",
                             "typemap", NULL};
    char *py_enc = (char *)NULL;
    char *in_enc = (char *)NULL;
    int py_depth = 0;
    int py_depth_cnt = 1;
	int bufferSize = 0, ok = -1;

	if (PYTHON_IS_NONE(args)) {
		/* error! don't have arguments */
		PyErr_SetString(PyExc_ValueError, msg_non_arguments);
		return NULL;
	}

	ok = PyArg_ParseTupleAndKeywords(args, keywds, "Os|iOO", kwlist,
                                     &input, &in_enc, &py_depth,
                                     &selector, &typemap);

	if (!ok) {
		/* error! don't have arguments */
		PyErr_SetString(PyExc_ValueError, msg_wrong_argument);
		return NULL;
	}

    if (py_depth == 0) {
        py_depth = 50;
    }

    py_enc = xmlgetencoding(xmlParseCharEncoding(in_enc));

    if (py_enc == (char *)NULL) {
		PyErr_SetString(PyExc_ValueError, msg_invalid_encoding);
		return NULL;
    }

    if (PYTHON_IS_NONE(input)) {
		PyErr_SetString(PyExc_ValueError, msg_non_object);
		return NULL;
    }

	dupItems = (PyListObject *)PyList_New(0);

    Py_XINCREF(input);

    sargs.o = &input;
    sargs.docptr = &docXml;
    sargs.rootNode = &rootNode;
    sargs.currentNode = &rootNode;
    sargs.dupSrcItems = &dupItems;
    sargs.enc = py_enc;
    sargs.depth = &py_depth;
    sargs.depthcnt = &py_depth_cnt;
    sargs.selector = &selector;
    sargs.typemap = typemap;

	serXml = pyxser_SerializeXml(&sargs);

    Py_XDECREF(input);
    Py_XDECREF(dupItems);

    if (docXml == (xmlDocPtr)NULL) {
        PyErr_SetString(PyExc_ValueError, msg_non_object);
        return NULL;
    }

    if (serXml == (xmlNodePtr)NULL) {
        xmlFreeDoc(docXml);
        PyErr_SetString(PyExc_ValueError, msg_non_object);
        return NULL;
    }

    xmlDocDumpFormatMemoryEnc(docXml, &xmlBuff, &bufferSize,
                              py_enc, 1);
    if (xmlBuff == BAD_CAST NULL) {
        xmlFreeDoc(docXml);
        PyErr_SetString(PyExc_ValueError, msg_non_object);
        return NULL;
    }
    res = PyUnicode_Decode((const char *)xmlBuff, bufferSize,
                           (char *)py_enc, pyxser_xml_encoding_mode);
    if (PYTHON_IS_NOT_NONE(res)) {
        xmlFree(xmlBuff);
        xmlFreeDoc(docXml);
        PyErr_Clear();
        return res;
    } else {
        xmlFree(xmlBuff);
        xmlFreeDoc(docXml);
        PyErr_SetString(PyExc_ValueError, msg_non_object);
        return NULL;
    }
	return NULL;
	/* error! not created */
}

static PyObject *
u_pyxserxmlc14n(PyObject *self, PyObject *args, PyObject *keywds)
{
	PyObject *res = Py_None;
	PyObject *input = (PyObject *)NULL;
    PyObject *selector = (PyObject *)NULL;
    PyObject *typemap = (PyObject *)NULL;
	PyListObject *dupItems = (PyListObject *)NULL;

	xmlNodePtr serXml = (xmlNodePtr)NULL;
	xmlNodePtr rootNode = (xmlNodePtr)NULL;
	xmlDocPtr docXml = (xmlDocPtr)NULL;
    xmlOutputBufferPtr xmlBuff = (xmlOutputBufferPtr)NULL;
    xmlChar *docPtr = (xmlChar *)NULL;

    PythonSerializationArguments sargs;

    static char *kwlist[] = {"obj", "depth", "com", "selector",
                             "typemap", NULL};
    int py_depth = 999999;
    int py_depth_cnt = 1;
    int py_exc = 0;
    int py_com = 0;
	int ok = -1;
    int ret = 0;

	if (PYTHON_IS_NONE(args)) {
		/* error! don't have arguments */
		PyErr_SetString(PyExc_ValueError, msg_non_arguments);
		return NULL;
	}
	ok = PyArg_ParseTupleAndKeywords(args, keywds, "O|iiOO", kwlist,
                                     &input, &py_depth,
                                     &py_com, &selector,
                                     &typemap);
	if (!ok) {
		/* error! don't have arguments */
		PyErr_SetString(PyExc_ValueError, msg_wrong_argument);
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
    Py_XINCREF(input);

    sargs.o = &input;
    sargs.docptr = &docXml;
    sargs.rootNode = &rootNode;
    sargs.currentNode = &rootNode;
    sargs.dupSrcItems = &dupItems;
    sargs.enc = (char *)pyxser_xml_encoding;
    sargs.depth = &py_depth;
    sargs.depthcnt = &py_depth_cnt;
    sargs.selector = &selector;
    sargs.typemap = typemap;

	serXml = pyxser_SerializeXml(&sargs);

    Py_XDECREF(input);
    Py_XDECREF(dupItems);

    if (docXml == (xmlDocPtr)NULL) {
        PyErr_SetString(PyExc_ValueError, msg_non_object);
        return NULL;
    }

    if (serXml == (xmlNodePtr)NULL) {
        xmlFreeDoc(docXml);
        PyErr_SetString(PyExc_ValueError, msg_non_object);
        return NULL;
    }

    xmlBuff = xmlAllocOutputBuffer(NULL);
    ret = xmlC14NExecute(docXml, pyxser_C14NRenderAllNodes, docXml,
                         py_exc, NULL, py_com, xmlBuff);
    if (ret < 0) {
        xmlFree(docPtr);
        xmlOutputBufferClose(xmlBuff);
        PyErr_SetString(PyExc_ValueError, msg_non_object);
        return NULL;
    }
    if (xmlBuff == NULL) {
        xmlFreeDoc(docXml);
        PyErr_SetString(PyExc_ValueError, msg_non_object);
        return NULL;
    }
    ret = xmlBuff->buffer->use;
    docPtr = BAD_CAST xmlStrndup(xmlBuff->buffer->content, ret);
    res = PyUnicode_Decode((const char *)docPtr, ret,
                           (char *)pyxser_xml_encoding,
                           pyxser_xml_encoding_mode);
    xmlOutputBufferClose(xmlBuff);
    if (PYTHON_IS_NOT_NONE(res)) {
        xmlFree(docPtr);
        xmlFreeDoc(docXml);
        PyErr_Clear();
        return res;
    }
	return NULL;
	/* error! not created */
}

static PyObject *
u_pyxserxmlc14nstrict(PyObject *self, PyObject *args, PyObject *keywds)
{
	PyObject *res = Py_None;
	PyObject *input = (PyObject *)NULL;
    PyObject *selector = (PyObject *)NULL;
    PyObject *typemap = (PyObject *)NULL;
	PyListObject *dupItems = (PyListObject *)NULL;

	xmlNodePtr serXml = (xmlNodePtr)NULL;
	xmlNodePtr rootNode = (xmlNodePtr)NULL;
	xmlDocPtr docXml = (xmlDocPtr)NULL;
    xmlChar *xmlBuff = (xmlChar *)NULL;

    PythonSerializationArguments sargs;

    static char *kwlist[] = {"obj", "depth", "com", "selector",
                             "typemap", NULL};
    int py_depth = 999999;
    int py_depth_cnt = 1;
    int py_exc = 0;
    int py_com = 0;
	int ok = -1;
    int ret = 0;

	if (PYTHON_IS_NONE(args)) {
		/* error! don't have arguments */
		PyErr_SetString(PyExc_ValueError, msg_non_arguments);
		return NULL;
	}
	ok = PyArg_ParseTupleAndKeywords(args, keywds, "O|iiOO", kwlist,
                                     &input, &py_depth,
                                     &py_com, &selector, &typemap);
	if (!ok) {
		/* error! don't have arguments */
		PyErr_SetString(PyExc_ValueError, msg_wrong_argument);
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

    Py_XINCREF(input);

    sargs.o = &input;
    sargs.docptr = &docXml;
    sargs.rootNode = &rootNode;
    sargs.currentNode = &rootNode;
    sargs.dupSrcItems = &dupItems;
    sargs.enc = (char *)pyxser_xml_encoding;
    sargs.depth = &py_depth;
    sargs.depthcnt = &py_depth_cnt;
    sargs.selector = &selector;
    sargs.typemap = typemap;

	serXml = pyxser_SerializeXml(&sargs);

    Py_XDECREF(input);
    Py_XDECREF(dupItems);

    if (docXml == (xmlDocPtr)NULL) {
        PyErr_SetString(PyExc_ValueError, msg_non_object);
        return NULL;
    }

    if (serXml == (xmlNodePtr)NULL) {
        xmlFreeDoc(docXml);
        PyErr_SetString(PyExc_ValueError, msg_non_object);
        return NULL;
    }

    ret = xmlC14NDocDumpMemory(docXml, NULL, py_exc, NULL, py_com, &xmlBuff);
    if (xmlBuff == NULL || ret <= 0) {
        if (xmlBuff != (xmlChar *)NULL) {
            xmlFree(xmlBuff);
        }
        xmlFreeDoc(docXml);
        PyErr_SetString(PyExc_ValueError, msg_non_object);
        return NULL;
    }
    res = PyUnicode_Decode((const char *)xmlBuff, ret,
                           (char *)pyxser_xml_encoding,
                           pyxser_xml_encoding_mode);
    if (PYTHON_IS_NOT_NONE(res)) {
        xmlFree(xmlBuff);
        xmlFreeDoc(docXml);
        PyErr_Clear();
        return res;
    } else {
        xmlFree(xmlBuff);
        xmlFreeDoc(docXml);
        PyErr_SetString(PyExc_ValueError, msg_non_object);
        return NULL;
    }
    PyErr_SetString(PyExc_ValueError, msg_non_object);
	xmlFreeDoc(docXml);
	return NULL;
	/* error! not created */
}

static PyObject *
pyxunserxml(PyObject *self, PyObject *args, PyObject *keywds)
{
	PyObject *res = Py_None;
	PyObject *cinit = Py_True;
	PyObject *input = (PyObject *)NULL;
	PyObject *tree = (PyObject *)NULL;
	PyObject *current = (PyObject *)NULL;
    PyObject *typemap = (PyObject *)NULL;
	PyDictObject *dupItems = (PyDictObject *)NULL;

	xmlNodePtr rootNode = (xmlNodePtr)NULL;
	xmlNodePtr currentNode = (xmlNodePtr)NULL;
	xmlDocPtr docXml = (xmlDocPtr)NULL;

    static char *kwlist[] = {"obj", "enc", "typemap",
                             "cinit", NULL};
    char *py_enc = (char *)NULL;
    char *in_enc = (char *)NULL;
    int py_depth = 0;

	PythonUnserializationArguments obj;

	int ok = -1;
	if (PYTHON_IS_NONE(args)) {
		/* error! don't have arguments */
		PyErr_SetString(PyExc_ValueError, msg_non_arguments);
		return res;
	}

	ok = PyArg_ParseTupleAndKeywords(args, keywds, "Os|OO",
                                     kwlist, &input, &in_enc,
                                     &typemap, &cinit);
	if (!ok) {
		/* error! don't have arguments */
		PyErr_SetString(PyExc_ValueError, msg_wrong_argument);
		return NULL;
	}

    if (pyxser_modules == (PyDictObject *)NULL) {
        pyxser_modules = (PyDictObject *)PyDict_New();
    }

    py_enc = xmlgetencoding(xmlParseCharEncoding(in_enc));
    if (py_enc == (char *)NULL) {
		PyErr_SetString(PyExc_ValueError, msg_invalid_encoding);
		return NULL;
    }

    if (PYTHON_IS_NONE(input)) {
		PyErr_SetString(PyExc_ValueError, msg_non_object);
		return NULL;
    }

    Py_XINCREF(input);

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
    obj.typemap = typemap;
    if (cinit == Py_False) {
        obj.construct = 0;
    } else {
        obj.construct = 1;
    }

    res = pyxser_UnserializeXml(&obj);

    Py_XDECREF(input);
    Py_XDECREF(dupItems);

    if (res == NULL) {
        PyErr_SetString(PyExc_ValueError, msg_non_object);
        return NULL;
    }

    return res;
}

static PyObject *
u_pyxunserxml(PyObject *self, PyObject *args, PyObject *keywds)
{
	PyObject *res = Py_None;
	PyObject *unic = Py_None;
    PyObject *cinit = Py_True;
	PyObject *input = (PyObject *)NULL;
	PyObject *tree = (PyObject *)NULL;
	PyObject *current = (PyObject *)NULL;
    PyObject *typemap = (PyObject *)NULL;
	PyDictObject *dupItems = (PyDictObject *)NULL;

	xmlNodePtr rootNode = (xmlNodePtr)NULL;
	xmlNodePtr currentNode = (xmlNodePtr)NULL;
	xmlDocPtr docXml = (xmlDocPtr)NULL;

    static char *kwlist[] = {"obj", "enc", "typemap", "cinit", NULL};
    char *py_enc = (char *)NULL;
    char *in_enc = (char *)NULL;
    int py_depth = 0;

	PythonUnserializationArguments obj;

	int ok = -1;
	if (PYTHON_IS_NONE(args)) {
		/* error! don't have arguments */
		PyErr_SetString(PyExc_ValueError, msg_non_arguments);
		return res;
	}
	ok = PyArg_ParseTupleAndKeywords(args, keywds, "Os|OO",
                                     kwlist, &input, &in_enc,
                                     &typemap, &cinit);
	if (!ok) {
		/* error! don't have arguments */
		PyErr_SetString(PyExc_ValueError, msg_wrong_argument);
		return NULL;
	}

    if (PYTHON_IS_NONE(pyxser_modules)) {
        pyxser_modules = (PyDictObject *)PyDict_New();
    }

    py_enc = xmlgetencoding(xmlParseCharEncoding(in_enc));
    if (py_enc == (char *)NULL) {
		PyErr_SetString(PyExc_ValueError, msg_invalid_encoding);
		return NULL;
    }

    if (PYTHON_IS_NONE(input)) {
		PyErr_SetString(PyExc_ValueError, msg_non_object);
		return NULL;
    }

    Py_XINCREF(input);

    unic = PyUnicode_AsEncodedString(input, (char *)py_enc,
                                     pyxser_xml_encoding_mode);

    if (PYTHON_IS_NONE(unic)) {
        Py_XDECREF(input);
		PyErr_SetString(PyExc_ValueError, msg_non_object);
		return NULL;
    }

    obj.doc = &unic;
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
    obj.typemap = typemap;
    if (cinit == Py_False) {
        obj.construct = 0;
    } else {
        obj.construct = 1;
    }
    res = pyxser_UnserializeXml(&obj);

    Py_XDECREF(input);
    Py_XDECREF(unic);
    Py_XDECREF(dupItems);

    if (res == NULL) {
        PyErr_SetString(PyExc_ValueError, msg_non_object);
        return NULL;
    }

    return res;
}

static PyObject *
pyxgetdtd(PyObject *self, PyObject *args)
{
    if (pyxstr_dtd == NULL) {
        pyxstr_dtd = PyString_FromString((const char *)pyxser_xml_dtd_location);
    }
    Py_XINCREF(pyxstr_dtd);
	return pyxstr_dtd;
}

static PyObject *
pyxgetdtdc14n(PyObject *self, PyObject *args)
{
    if (pyxstr_dtd_c14n == NULL) {
        pyxstr_dtd_c14n = PyString_FromString((const char *)pyxser_xml_dtd_c14n_location);
    }
    Py_XINCREF(pyxstr_dtd_c14n);
	return pyxstr_dtd_c14n;
}

static PyObject *
pyxgetxsd(PyObject *self, PyObject *args)
{
    if (pyxstr_xsd == NULL) {
        pyxstr_xsd = PyString_FromString((const char *)pyxser_xml_xsd_location);
    }
    Py_XINCREF(pyxstr_xsd);
	return pyxstr_xsd;
}

static PyObject *
pyxgetxsdc14n(PyObject *self, PyObject *args)
{
    if (pyxstr_xsd_c14n == NULL) {
        pyxstr_xsd_c14n = PyString_FromString((const char *)pyxser_xml_xsd_c14n_location);
    }
    Py_XINCREF(pyxstr_xsd_c14n);
	return pyxstr_xsd_c14n;
}

static PyObject *
u_pyxvalidate(PyObject *self, PyObject *args, PyObject *keywds)
{
	xmlDocPtr docPtr = (xmlDocPtr)NULL;
	PyObject *res = Py_False;
	PyObject *input = (PyObject *)NULL;
	PyObject *unic = (PyObject *)NULL;
    size_t data_size = 0;
	char *docstr = (char *)NULL;
    int parseopts = XML_PARSE_RECOVER;

    static char *kwlist[] = {"obj", "enc", NULL};
    char *py_enc = (char *)NULL;
    char *in_enc = (char *)NULL;

	int ok = -1;
	if (PYTHON_IS_NONE(args)) {
		/* error! don't have arguments */
		PyErr_SetString(PyExc_ValueError, msg_non_arguments);
		return NULL;
	}
	ok = PyArg_ParseTupleAndKeywords(args, keywds, "Os", kwlist,
                                     &input, &in_enc);
	if (!ok) {
		/* error! don't have arguments */
		PyErr_SetString(PyExc_ValueError, msg_wrong_argument);
		return NULL;
	}

    py_enc = xmlgetencoding(xmlParseCharEncoding(in_enc));
    if (py_enc == (char *)NULL) {
		PyErr_SetString(PyExc_ValueError, msg_invalid_encoding);
		return NULL;
    }

    if (PYTHON_IS_NONE(input)) {
		PyErr_SetString(PyExc_ValueError, msg_non_object);
		return NULL;
    }

    Py_XINCREF(input);

    unic = PyUnicode_AsEncodedString(input, (char *)py_enc,
                                     pyxser_xml_encoding_mode);
    if (PYTHON_IS_NONE(unic)) {
		PyErr_SetString(PyExc_ValueError, msg_non_object);
		return NULL;
    }

    data_size = PyUnicode_GET_DATA_SIZE(unic);
    if (PYTHON_IS_NONE(unic)) {
        Py_XDECREF(input);
        PyErr_SetString(PyExc_ValueError, msg_non_xml);
        return NULL;
    }

    docstr = PyString_AS_STRING(unic);
    docPtr = xmlReadMemory(docstr, data_size, NULL,
                           (const char *)py_enc, parseopts);

    if (docPtr != (xmlDocPtr)NULL) {
        if ((pyxser_ValidateDocumentXSD(docPtr)) == 1) {
            res = Py_True;
        }
        xmlFreeDoc(docPtr);
    } else {
        Py_XDECREF(unic);
        Py_XDECREF(input);
        PyErr_SetString(PyExc_ValueError, msg_non_xml);
        return NULL;
    }
    Py_XDECREF(unic);
    Py_XDECREF(input);
    PyErr_Clear();
    Py_XINCREF(res);
    return res;
}

static PyObject *
u_pyxvalidatec14n(PyObject *self, PyObject *args, PyObject *keywds)
{
	xmlDocPtr docPtr = (xmlDocPtr)NULL;
	PyObject *res = Py_False;
	PyObject *input = (PyObject *)NULL;
	PyObject *unic = (PyObject *)NULL;
    size_t data_size = 0;
	char *docstr = (char *)NULL;
    int parseopts = XML_PARSE_RECOVER;

    static char *kwlist[] = { "obj", "enc", NULL };
    char *py_enc = (char *)NULL;
    char *in_enc = (char *)NULL;

	int ok = -1;
	if (PYTHON_IS_NONE(args)) {
		/* error! don't have arguments */
		PyErr_SetString(PyExc_ValueError, msg_non_arguments);
		return NULL;
	}
	ok = PyArg_ParseTupleAndKeywords(args, keywds, "Os", kwlist,
                                     &input, &in_enc);
	if (!ok) {
		/* error! don't have arguments */
		PyErr_SetString(PyExc_ValueError, msg_wrong_argument);
		return NULL;
	}

    py_enc = xmlgetencoding(xmlParseCharEncoding(in_enc));
    if (py_enc == (char *)NULL) {
		PyErr_SetString(PyExc_ValueError, msg_invalid_encoding);
		return NULL;
    }

    if (PYTHON_IS_NONE(input)) {
		PyErr_SetString(PyExc_ValueError, msg_non_object);
		return NULL;
    }

    Py_INCREF(input);

    unic = PyUnicode_AsEncodedString(input, (char *)py_enc,
                                     pyxser_xml_encoding_mode);
    Py_XDECREF(input);

    if (PYTHON_IS_NONE(unic)) {
		PyErr_SetString(PyExc_ValueError, msg_non_object);
		return NULL;
    }

    data_size = PyUnicode_GET_DATA_SIZE(unic);
    if (PYTHON_IS_NONE(unic)) {
        PyErr_SetString(PyExc_ValueError, msg_non_xml);
        return NULL;
    }

    docstr = PyString_AS_STRING(unic);
    docPtr = xmlReadMemory(docstr, data_size, NULL,
                           (const char *)py_enc, parseopts);
    if (docPtr != (xmlDocPtr)NULL) {
        if ((pyxser_ValidateDocumentXSDC14N(docPtr)) == 1) {
            res = Py_True;
        }
        xmlFreeDoc(docPtr);
    } else {
        Py_XDECREF(unic);
        PyErr_SetString(PyExc_ValueError, msg_non_xml);
        return NULL;
    }
    Py_XDECREF(unic);
    PyErr_Clear();
    Py_XINCREF(res);
    return res;
}

static PyObject *
u_pyxvalidatedtd(PyObject *self, PyObject *args, PyObject *keywds)
{
	xmlDocPtr docPtr = (xmlDocPtr)NULL;
	PyObject *res = Py_False;
	PyObject *input = (PyObject *)NULL;
	PyObject *unic = (PyObject *)NULL;
    size_t data_size = 0;
	char *docstr = (char *)NULL;
    int parseopts = XML_PARSE_RECOVER;

    static char *kwlist[] = {"obj", "enc", NULL};
    char *py_enc = (char *)NULL;
    char *in_enc = (char *)NULL;

	int ok = -1;
	if (PYTHON_IS_NONE(args)) {
		/* error! don't have arguments */
		PyErr_SetString(PyExc_ValueError, msg_non_arguments);
		return NULL;
	}
	ok = PyArg_ParseTupleAndKeywords(args, keywds, "Os", kwlist,
                                     &input, &in_enc);
	if (!ok) {
		/* error! don't have arguments */
		PyErr_SetString(PyExc_ValueError, msg_wrong_argument);
		return NULL;
	}

    py_enc = xmlgetencoding(xmlParseCharEncoding(in_enc));
    if (py_enc == (char *)NULL) {
		PyErr_SetString(PyExc_ValueError, msg_invalid_encoding);
		return NULL;
    }

    if (PYTHON_IS_NONE(input)) {
		PyErr_SetString(PyExc_ValueError, msg_non_object);
		return NULL;
    }

    Py_XINCREF(input);

    unic = PyUnicode_AsEncodedString(input, (char *)py_enc,
                                     pyxser_xml_encoding_mode);
    Py_XDECREF(input);

    if (PYTHON_IS_NONE(unic)) {
        Py_XDECREF(input);
		PyErr_SetString(PyExc_ValueError, msg_non_object);
		return NULL;
    }

    data_size = PyUnicode_GET_DATA_SIZE(unic);
    if (PYTHON_IS_NONE(unic)) {
        PyErr_SetString(PyExc_ValueError, msg_non_xml);
        return NULL;
    }
    docstr = PyString_AS_STRING(unic);
    docPtr = xmlReadMemory(docstr, data_size, NULL,
                           (const char *)py_enc, parseopts);
    if (docPtr != (xmlDocPtr)NULL) {
        if ((pyxser_ValidateDocument(docPtr)) == 1) {
            res = Py_True;
        }
        xmlFreeDoc(docPtr);
    } else {
        Py_XDECREF(unic);
        PyErr_SetString(PyExc_ValueError, msg_non_xml);
        return NULL;
    }
    Py_XDECREF(unic);
    Py_XINCREF(res);
    PyErr_Clear();
    return res;
}

static PyObject *
u_pyxvalidatec14ndtd(PyObject *self, PyObject *args, PyObject *keywds)
{
	xmlDocPtr docPtr = (xmlDocPtr)NULL;
	PyObject *res = Py_False;
	PyObject *input = (PyObject *)NULL;
	PyObject *unic = (PyObject *)NULL;
    size_t data_size = 0;
	char *docstr = (char *)NULL;
    int parseopts = XML_PARSE_RECOVER;

    static char *kwlist[] = {"obj", "enc", NULL};
    char *py_enc = (char *)NULL;
    char *in_enc = (char *)NULL;

	int ok = -1;
	if (PYTHON_IS_NONE(args)) {
		/* error! don't have arguments */
		PyErr_SetString(PyExc_ValueError, msg_non_arguments);
		return NULL;
	}
	ok = PyArg_ParseTupleAndKeywords(args, keywds, "Os", kwlist,
                                     &input, &in_enc);
	if (!ok) {
		/* error! don't have arguments */
		PyErr_SetString(PyExc_ValueError, msg_wrong_argument);
		return NULL;
	}

    py_enc = xmlgetencoding(xmlParseCharEncoding(in_enc));
    if (py_enc == (char *)NULL) {
		PyErr_SetString(PyExc_ValueError, msg_invalid_encoding);
		return NULL;
    }

    if (PYTHON_IS_NONE(input)) {
		PyErr_SetString(PyExc_ValueError, msg_non_object);
		return NULL;
    }

    Py_XINCREF(input);

    unic = PyUnicode_Encode(PyUnicode_AS_UNICODE(input),
                            PyUnicode_GET_SIZE(input),
                            (char *)py_enc,
                            pyxser_xml_encoding_mode);
    if (PYTHON_IS_NONE(unic)) {
        Py_XDECREF(input);
		PyErr_SetString(PyExc_ValueError, msg_non_object);
		return NULL;
    }

    data_size = PyUnicode_GET_DATA_SIZE(unic);
    docstr = PyString_AS_STRING(unic);
    docPtr = xmlReadMemory(docstr, data_size, NULL,
                           (const char *)py_enc, parseopts);

    if (docPtr != (xmlDocPtr)NULL) {
        if ((pyxser_ValidateDocumentC14N(docPtr)) == 1) {
            res = Py_True;
        }
        xmlFreeDoc(docPtr);
    } else {
        Py_XDECREF(unic);
        Py_XDECREF(input);
        PyErr_SetString(PyExc_ValueError, msg_non_xml);
        return NULL;
    }
    Py_XDECREF(unic);
    Py_XDECREF(input);
    PyErr_Clear();
    Py_XINCREF(res);
    return res;
}

static PyObject *
pyxvalidate(PyObject *self, PyObject *args, PyObject *keywds)
{
	xmlDocPtr docPtr = (xmlDocPtr)NULL;
	PyObject *res = Py_False;
    PyObject *enc = Py_None;
	PyObject *input = (PyObject *)NULL;

	char *docstr = (char *)NULL;
    int parseopts = XML_PARSE_RECOVER;

    static char *kwlist[] = {"obj", "enc", NULL};
    char *py_enc = (char *)NULL;
    char *in_enc = (char *)NULL;

	int ok = -1;
	if (PYTHON_IS_NONE(args)) {
		/* error! don't have arguments */
		PyErr_SetString(PyExc_ValueError, msg_non_arguments);
		return NULL;
	}
	ok = PyArg_ParseTupleAndKeywords(args, keywds, "Os", kwlist,
                                     &input, &in_enc);
	if (!ok) {
		/* error! don't have arguments */
		PyErr_SetString(PyExc_ValueError, msg_wrong_argument);
		return NULL;
	}

    py_enc = xmlgetencoding(xmlParseCharEncoding(in_enc));
    if (py_enc == (char *)NULL) {
		PyErr_SetString(PyExc_ValueError, msg_invalid_encoding);
		return NULL;
    }

    if (PYTHON_IS_NONE(input)) {
		PyErr_SetString(PyExc_ValueError, msg_non_object);
		return NULL;
    }

    Py_XINCREF(input);

#if PY_MAJOR_VERSION >= 3
    enc = pyxser_GetStringFromUnicode(input);
    docstr = PyString_AS_STRING(enc);
    docPtr = xmlReadMemory(docstr, strlen(docstr),
                           NULL, (const char *)py_enc, parseopts);
#else /* PY_MAJOR_VERSION >= 3 */
    docstr = PyString_AS_STRING(input);
    docPtr = xmlReadMemory(docstr, PyString_GET_SIZE(input),
                           NULL, (const char *)py_enc, parseopts);
#endif /* PY_MAJOR_VERSION >= 3 */

    if (docPtr != (xmlDocPtr)NULL) {
        if ((pyxser_ValidateDocumentXSD(docPtr)) == 1) {
            res = Py_True;
        }
        xmlFreeDoc(docPtr);
    } else {
        Py_XDECREF(input);
        PyErr_SetString(PyExc_ValueError, msg_non_xml);
        return NULL;
    }
    PyErr_Clear();
    Py_XDECREF(input);
    Py_XINCREF(res);

#if PY_MAJOR_VERSION >= 3
    Py_XDECREF(enc);
#endif /* PY_MAJOR_VERSION >= 3 */

    return res;
}

static PyObject *
pyxvalidatec14n(PyObject *self, PyObject *args, PyObject *keywds)
{
	xmlDocPtr docPtr = (xmlDocPtr)NULL;
	PyObject *res = Py_False;
	PyObject *input = (PyObject *)NULL;
    PyObject *enc = Py_None;

	char *docstr = (char *)NULL;
    int parseopts = XML_PARSE_RECOVER;

    static char *kwlist[] = {"obj", "enc", NULL};
    char *py_enc = (char *)NULL;
    char *in_enc = (char *)NULL;

	int ok = -1;
	if (PYTHON_IS_NONE(args)) {
		/* error! don't have arguments */
		PyErr_SetString(PyExc_ValueError, msg_non_arguments);
		return NULL;
	}
	ok = PyArg_ParseTupleAndKeywords(args, keywds, "Os", kwlist,
                                     &input, &in_enc);
	if (!ok) {
		/* error! don't have arguments */
		PyErr_SetString(PyExc_ValueError, msg_wrong_argument);
		return NULL;
	}

    py_enc = xmlgetencoding(xmlParseCharEncoding(in_enc));
    if (py_enc == (char *)NULL) {
		PyErr_SetString(PyExc_ValueError, msg_invalid_encoding);
		return NULL;
    }

    if (PYTHON_IS_NONE(input)) {
		PyErr_SetString(PyExc_ValueError, msg_non_object);
		return NULL;
    }

    Py_XINCREF(input);

#if PY_MAJOR_VERSION >= 3
    enc = pyxser_GetStringFromUnicode(input);
    docstr = PyString_AS_STRING(enc);
    docPtr = xmlReadMemory(docstr, strlen(docstr),
                           NULL, (const char *)py_enc, parseopts);
#else /* PY_MAJOR_VERSION >= 3 */
    docstr = PyString_AS_STRING(input);
    docPtr = xmlReadMemory(docstr, PyString_GET_SIZE(input),
                           NULL, (const char *)py_enc, parseopts);
#endif /* PY_MAJOR_VERSION >= 3 */

    if (docPtr != (xmlDocPtr)NULL) {
        if ((pyxser_ValidateDocumentXSDC14N(docPtr)) == 1) {
            res = Py_True;
        }
        xmlFreeDoc(docPtr);
    } else {
        Py_XDECREF(input);
        PyErr_SetString(PyExc_ValueError, msg_non_xml);
        return NULL;
    }
    PyErr_Clear();
    Py_XINCREF(res);
    Py_XDECREF(input);

#if PY_MAJOR_VERSION >= 3
    Py_XDECREF(enc);
#endif /* PY_MAJOR_VERSION >= 3 */

    return res;
}

static PyObject *
pyxvalidatedtd(PyObject *self, PyObject *args, PyObject *keywds)
{
	xmlDocPtr docPtr = (xmlDocPtr)NULL;
	PyObject *res = Py_False;
	PyObject *enc = Py_None;
	PyObject *input = (PyObject *)NULL;
	char *docstr = (char *)NULL;
    int parseopts = XML_PARSE_RECOVER;

    static char *kwlist[] = {"obj", "enc", NULL};
    char *py_enc = (char *)NULL;
    char *in_enc = (char *)NULL;

	int ok = -1;
	if (PYTHON_IS_NONE(args)) {
		/* error! don't have arguments */
		PyErr_SetString(PyExc_ValueError, msg_non_arguments);
		return NULL;
	}
	ok = PyArg_ParseTupleAndKeywords(args, keywds, "Os", kwlist,
                                     &input, &in_enc);
	if (!ok) {
		/* error! don't have arguments */
		PyErr_SetString(PyExc_ValueError, msg_wrong_argument);
		return NULL;
	}

    py_enc = xmlgetencoding(xmlParseCharEncoding(in_enc));
    if (py_enc == (char *)NULL) {
		PyErr_SetString(PyExc_ValueError, msg_invalid_encoding);
		return NULL;
    }

    if (PYTHON_IS_NONE(input)) {
		PyErr_SetString(PyExc_ValueError, msg_non_object);
		return NULL;
    }

#if PY_MAJOR_VERSION >= 3
    enc = pyxser_GetStringFromUnicode(input);
    docstr = PyString_AS_STRING(enc);
    docPtr = xmlReadMemory(docstr, strlen(docstr),
                           NULL, (const char *)py_enc, parseopts);
#else /* PY_MAJOR_VERSION >= 3 */
    docstr = PyString_AS_STRING(input);
    docPtr = xmlReadMemory(docstr, PyString_GET_SIZE(input),
                           NULL, (const char *)py_enc, parseopts);
#endif /* PY_MAJOR_VERSION >= 3 */

    docstr = PyString_AS_STRING(input);
    docPtr = xmlReadMemory(docstr, PyString_GET_SIZE(input),
                           NULL, (const char *)py_enc, parseopts);
    if (docPtr != (xmlDocPtr)NULL) {
        if ((pyxser_ValidateDocumentXSD(docPtr)) == 1) {
            res = Py_True;
        }
        xmlFreeDoc(docPtr);
    } else {
        Py_XDECREF(input);
        PyErr_SetString(PyExc_ValueError, msg_non_xml);
        return NULL;
    }
    PyErr_Clear();
    Py_XDECREF(input);
    Py_XINCREF(res);

#if PY_MAJOR_VERSION >= 3
    Py_XDECREF(enc);
#endif /* PY_MAJOR_VERSION >= 3 */

    return res;
}

static PyObject *
pyxvalidatec14ndtd(PyObject *self, PyObject *args, PyObject *keywds)
{
	xmlDocPtr docPtr = (xmlDocPtr)NULL;
	PyObject *res = Py_False;
	PyObject *enc = Py_None;
	PyObject *input = (PyObject *)NULL;
	char *docstr = (char *)NULL;
    int parseopts = XML_PARSE_RECOVER;

    static char *kwlist[] = {"obj", "enc", NULL};
    char *py_enc = (char *)NULL;
    char *in_enc = (char *)NULL;

	int ok = -1;
	if (PYTHON_IS_NONE(args)) {
		/* error! don't have arguments */
		PyErr_SetString(PyExc_ValueError, msg_non_arguments);
		return NULL;
	}
	ok = PyArg_ParseTupleAndKeywords(args, keywds, "Os", kwlist,
                                     &input, &in_enc);
	if (!ok) {
		/* error! don't have arguments */
		PyErr_SetString(PyExc_ValueError, msg_wrong_argument);
		return NULL;
	}

    py_enc = xmlgetencoding(xmlParseCharEncoding(in_enc));
    if (py_enc == (char *)NULL) {
		PyErr_SetString(PyExc_ValueError, msg_invalid_encoding);
		return NULL;
    }

    if (PYTHON_IS_NONE(input)) {
		PyErr_SetString(PyExc_ValueError, msg_non_object);
		return NULL;
    }

#if PY_MAJOR_VERSION >= 3
    enc = pyxser_GetStringFromUnicode(input);
    docstr = PyString_AS_STRING(enc);
    docPtr = xmlReadMemory(docstr, strlen(docstr),
                           NULL, (const char *)py_enc, parseopts);
#else /* PY_MAJOR_VERSION >= 3 */
    docstr = PyString_AS_STRING(input);
    docPtr = xmlReadMemory(docstr, PyString_GET_SIZE(input),
                           NULL, (const char *)py_enc, parseopts);
#endif /* PY_MAJOR_VERSION >= 3 */

    docstr = PyString_AS_STRING(input);
    docPtr = xmlReadMemory(docstr, PyString_GET_SIZE(input), NULL,
                           (const char *)py_enc, parseopts);
    if (docPtr != (xmlDocPtr)NULL) {
        if ((pyxser_ValidateDocumentXSDC14N(docPtr)) == 1) {
            res = Py_True;
        }
        xmlFreeDoc(docPtr);
    } else {
        Py_XDECREF(input);
        PyErr_SetString(PyExc_ValueError, msg_non_xml);
        return NULL;
    }
    Py_XDECREF(input);
    Py_XINCREF(res);
    PyErr_Clear();

#if PY_MAJOR_VERSION >= 3
    Py_XDECREF(enc);
#endif /* PY_MAJOR_VERSION >= 3 */

    return res;
}

static PyObject *
xmlcleanup(PyObject *self, PyObject *args)
{
    xmlFreeNs(pyxser_default_ns);
    pyxser_default_ns = NULL;

    xmlFreeDtd(pyxser_dtd_object);
    pyxser_dtd_object = NULL;

    xmlFreeDtd(pyxser_dtd_c14n_object);
    pyxser_dtd_c14n_object = NULL;

    xmlCleanupEncodingAliases();

    xmlCleanupParser();

    Py_INCREF(Py_None);
    PyErr_Clear();

    return Py_None;
}


void
pyxser_unregister(void)
{
    pyxser_version_obj = NULL;

    Py_XDECREF(pyxser_modules);
    pyxser_modules = NULL;

    xmlFreeDtd(pyxser_dtd_object);
    pyxser_dtd_object = (xmlDtdPtr)NULL;

    xmlFreeDtd(pyxser_dtd_c14n_object);
    pyxser_dtd_c14n_object = (xmlDtdPtr)NULL;

    xmlFreeDoc(pyxser_xsd_doc);
    pyxser_xsd_doc = (xmlDocPtr)NULL;

    xmlFreeDoc(pyxser_xsd_c14n_doc);
    pyxser_xsd_c14n_doc = (xmlDocPtr)NULL;

    xmlFreeNs(pyxser_default_ns);
    pyxser_default_ns = NULL;

    xmlCleanupEncodingAliases();

    xmlCleanupParser();
}

static char *
xmlgetencoding(int id)
{
    switch (id) {
    case XML_CHAR_ENCODING_ERROR:
    case XML_CHAR_ENCODING_NONE:
    case XML_CHAR_ENCODING_ASCII:
        return "ascii";
    case XML_CHAR_ENCODING_UTF8:
        return "utf-8";
    case XML_CHAR_ENCODING_UTF16LE:
    case XML_CHAR_ENCODING_UTF16BE:
        return "utf-16";
    case XML_CHAR_ENCODING_UCS4LE:
    case XML_CHAR_ENCODING_UCS4BE:
    case XML_CHAR_ENCODING_UCS4_2143:
    case XML_CHAR_ENCODING_UCS4_3412:
        return "ucs-4";
    case XML_CHAR_ENCODING_EBCDIC:
        return "ebcdic";
    case XML_CHAR_ENCODING_UCS2:
        return "ucs-2";
    case XML_CHAR_ENCODING_8859_1:
        return "iso-8859-1";
    case XML_CHAR_ENCODING_8859_2:
        return "iso-8859-2";
    case XML_CHAR_ENCODING_8859_3:
        return "iso-8859-3";
    case XML_CHAR_ENCODING_8859_4:
        return "iso-8859-4";
    case XML_CHAR_ENCODING_8859_5:
        return "iso-8859-5";
    case XML_CHAR_ENCODING_8859_6:
        return "iso-8859-6";
    case XML_CHAR_ENCODING_8859_7:
        return "iso-8859-7";
    case XML_CHAR_ENCODING_8859_8:
        return "iso-8859-8";
    case XML_CHAR_ENCODING_8859_9:
        return "iso-8859-9";
    case XML_CHAR_ENCODING_SHIFT_JIS:
        return "shift-jis";
    case XML_CHAR_ENCODING_2022_JP:
        return "iso-2022-JP";
    case XML_CHAR_ENCODING_EUC_JP:
        return "euc-jp";
    default:
        /* we use utf-8 as default */
        return (char *)NULL;
    }
}


/* pyserx.c ends here */
