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

#include <stdarg.h>

#include "include/pyxser.h"

xmlNsPtr pyxser_default_ns;
xmlNsPtr pyxser_xsi_ns;
xmlNsPtr pyxser_xsd_ns;
xmlDocPtr pyxser_xsd_doc = (xmlDocPtr)NULL;
xmlDocPtr pyxser_xsd_c14n_doc = (xmlDocPtr)NULL;

void pyxser_CreateDocument(xmlDocPtr doc, xmlDocPtr *docptr,
                           xmlNodePtr *rootNode,
                           xmlNodePtr *xmlRootNode,
                           const char *enc,
                           PyObject *o,
                           char *objnam,
                           xmlNodePtr *currentNode);


void pyxser_validity_exception(void *ctx, const char *msg, va_list args);


static const char type_int[] = "int";
static const char type_long[] = "long";
static const char type_bool[] = "bool";
static const char type_str[] = "str";
static const char type_unicode[] = "unicode";
static const char type_float[] = "float";
static const char type_double[] = "double";
static const char type_complex[] = "complex";
static const char type_buffer[] = "buffer";
static const char type_list[] = "list";
static const char type_tuple[] = "tuple";
static const char type_dict[] = "dict";
static const char type_set[] = "set";
static const char type_sequence[] = "list";
static const char type_main[] = "__main__";
#if PY_MAJOR_VERSION >= 3
static const char type_builtin[] = "builtins";
#else
static const char type_builtin[] = "__builtin__";
#endif /* PY_MAJOR_VERSION >= 3 */
static const char schema_encoding[] = "utf-8";

xmlDtdPtr pyxser_dtd_object = (xmlDtdPtr)NULL;
xmlDtdPtr pyxser_dtd_c14n_object = (xmlDtdPtr)NULL;

const PythonTypeSerialize serxConcreteTypes[] = {
	/* Numeric Types */
	{1, pyxserInt_CheckExact, pyxser_SerializeExactInt},
	{1, pyxserInt_Check, pyxser_SerializeInt},
	{1, pyxserLong_CheckExact, pyxser_SerializeLong},
	{1, pyxserBool_Check, pyxser_SerializeBoolean},
	{1, pyxserFloat_CheckExact, pyxser_SerializeExactFloat},
	{1, pyxserFloat_Check, pyxser_SerializeFloat},
	{1, pyxserComplex_CheckExact, pyxser_SerializeExactComplex},
	{1, pyxserComplex_Check, pyxser_SerializeComplex},
	/* Strings */
	{1, pyxserString_Check, pyxser_SerializeString},
	{1, pyxserString_CheckExact, pyxser_SerializeExactString},
	{1, pyxserUnicode_CheckExact, pyxser_SerializeExactUnicode},
	{1, pyxserUnicode_Check, pyxser_SerializeUnicode},
	/* {1, pyxserBuffer_Check, pyxser_SerializeBuffer}, */
	/* Tuples */
	{1, pyxserTuple_Check, pyxser_SerializeTuple},
	/* Lists */
	{1, pyxserList_Check, pyxser_SerializeList},
	/* Dictionaries */
	{1, pyxserDict_Check, pyxser_SerializeDict},
    /* Weakrefs */
    {1, pyxserWeakref_Check, pyxser_SerializeWeakref},
#if 0 /* I near future, according to user proposals we will
         join a better representation of those objects */
    /* Sets */
	{1, pyxserAnySet_Check, pyxser_SerializeSet},
	{1, pyxserAnySet_CheckExact, pyxser_SerializeSet},
	{1, pyxserFrozenSet_CheckExact, pyxser_SerializeSet},
    /* Sequences */
    {1, pyxserSequence_Check, pyxser_SerializeSequence},
	/* Files */
	{1, pyxserFile_CheckExact, pyxser_SerializeExactFile},
	{1, pyxserFile_Check, pyxser_SerializeFile},
	/* Instrospection */
	{1, pyxserInstance_Check, pyxser_SerializeInstance},
	{1, pyxserFunction_Check, pyxser_SerializeFunction},
	{1, pyxserMethod_Check, pyxser_SerializeMethod},
	{1, pyxserModule_Check, pyxser_SerializeModule},
	{1, pyxserModule_CheckExact, pyxser_SerializeExactModule},
	{1, pyxserSeqIter_Check, pyxser_SerializeIterator},
	{1, pyxserSlice_Check, pyxser_SerializeSlice},
	{1, pyxserCell_Check, pyxser_SerializeCell},
	{1, pyxserGen_Check, pyxser_SerializeGenerator},
	{1, pyxserDate_CheckExact, pyxser_SerializeGenerator}
#endif /* !0 */
	{0, NULL, NULL}
};

const PythonTypeDeserialize unserxConcreteTypes[] = {
	/* Numeric Types */
	{1, pyxunserInt_Check, pyxunser_SerializeInt},
	{1, pyxunserInt_CheckExact, pyxunser_SerializeExactInt},
	{1, pyxunserBool_Check, pyxunser_SerializeBoolean},
	{1, pyxunserLong_CheckExact, pyxunser_SerializeLong},
	{1, pyxunserFloat_Check, pyxunser_SerializeFloat},
	{1, pyxunserFloat_CheckExact, pyxunser_SerializeExactFloat},
	{1, pyxunserComplex_Check, pyxunser_SerializeComplex},
	{1, pyxunserComplex_CheckExact, pyxunser_SerializeExactComplex},
	/* Strings */
	{1, pyxunserString_Check, pyxunser_SerializeString},
	{1, pyxunserString_CheckExact, pyxunser_SerializeExactString},
	{1, pyxunserUnicode_Check, pyxunser_SerializeUnicode},
	{1, pyxunserUnicodeExact_CheckExact, pyxunser_SerializeExactUnicode},
	/* Dictionaries */
	{1, pyxunserDict_Check, pyxunser_SerializeDict},
	/* Lists */
	{1, pyxunserList_Check, pyxunser_SerializeList},
	/* Tuples */
	{1, pyxunserTuple_Check, pyxunser_SerializeTuple},
#if 0 /* I near future, according to user proposals we will
         join a better representation of those objects */
	{1, pyxunserBuffer_Check, pyxunser_SerializeBuffer},
    /* Sets */
	{1, pyxunserAnySet_Check, pyxunser_SerializeSet},
	{1, pyxunserAnySet_CheckExact, pyxunser_SerializeSet},
	{1, pyxunserFrozenSet_CheckExact, pyxunser_SerializeSet},
    /* Sequences */
    {1, pyxunserSequence_Check, pyxunser_SerializeSequence},
	/* Files */
	{1, pyxunserFile_Check, pyxunser_SerializeFile},
	{1, pyxunserFile_CheckExact, pyxunser_SerializeExactFile},
	/* Instrospection */
	{1, pyxunserInstance_Check, pyxunser_SerializeInstance},
	{1, pyxunserFunction_Check, pyxunser_SerializeFunction},
	{1, pyxunserMethod_Check, pyxunser_SerializeMethod},
	{1, pyxunserModule_Check, pyxunser_SerializeModule},
	{1, pyxunserModule_CheckExact, pyxunser_SerializeExactModule},
	{1, pyxunserSeqIter_Check, pyxunser_SerializeIterator},
	{1, pyxunserSlice_Check, pyxunser_SerializeSlice},
	{1, pyxunserCell_Check, pyxunser_SerializeCell},
	{1, pyxunserGen_Check, pyxunser_SerializeGenerator},
	{1, pyxunserDate_CheckExact, pyxunser_SerializeGenerator},
#endif /* !0 */
	{0, NULL, NULL}
};

char *
pyxser_GetClassName(PyObject *obj)
{
	PyObject *klass = Py_None;
	PyObject *cname = Py_None;
    PyObject *unic = (PyObject *)NULL;

	char *cn = (char *)NULL;

    if (PYTHON_IS_NONE(obj)) {
        return cn;
    }

    klass = PyObject_GetAttrString(obj, pyxser_attr_class);
    if (PYTHON_IS_NONE(klass)) {
        PyErr_Clear();
        return cn;
    }

    cname = PyObject_GetAttrString(klass, pyxser_attr_name);
    if (PYTHON_IS_NONE(cname)) {
        PyErr_Clear();
        Py_DECREF(klass);
        return cn;
    }

#if PY_MAJOR_VERSION >= 3

    unic = PyUnicode_Encode(PyUnicode_AS_UNICODE(cname),
                            PyUnicode_GET_SIZE(cname),
                            pyxser_xml_encoding,
                            pyxser_xml_encoding_mode);
    if (PYTHON_IS_NONE(unic)) {
        PyErr_Clear();
        unic = PyUnicode_FromEncodedObject(cname, pyxser_xml_encoding,
                                           pyxser_xml_encoding_mode);
    }

    if (PYTHON_IS_NONE(unic)) {
        PyErr_Clear();
        return (char *)NULL;
    }

    cn = PyString_AS_STRING(unic);

#else /* PY_MAJOR_VERSION >= 3 */

    if (!PyString_Check(cname)) {
        PYXSER_FREE_OBJECT(klass);
        PYXSER_FREE_OBJECT(cname);
        return cn;
    }

    cn = PyString_AS_STRING(cname);

#endif /* PY_MAJOR_VERSION >= 3 */

    PYXSER_FREE_OBJECT(klass);
    PYXSER_FREE_OBJECT(cname);
    PyErr_Clear();

    if (strnlen(cn, 128) == 0) {
        return (char *)NULL;
    } else {
        return cn;
    }
}


xmlNodePtr
pyxser_SerializePrimitiveType(PyxSerializationArgsPtr args)
{

    PyObject *o = *args->o;
    char *name = args->name;
    xmlDocPtr doc = *args->docptr;

	PyObject *str = (PyObject *)NULL;
	PyObject *classPtr = (PyObject *)NULL;
	PyObject *className = (PyObject *)NULL;
#if PY_MAJOR_VERSION >= 3
    PyObject *unic = (PyObject *)NULL;
#endif /* PY_MAJOR_VERSION >= 3 */

	xmlNodePtr newElementNode = (xmlNodePtr)NULL;
	xmlNodePtr newTextNode = (xmlNodePtr)NULL;
	xmlAttrPtr typeAttr = (xmlAttrPtr)NULL;
	xmlAttrPtr nameAttr = (xmlAttrPtr)NULL;

	char *sptr = (char *)NULL;
	char *nptr = (char *)NULL;

	if (PYTHON_IS_NONE(o)) {
		return (xmlNodePtr)NULL;
	}

	str = PyObject_Str(o);
	if (PYTHON_IS_NONE(str)) {
		return (xmlNodePtr)NULL;
	}

#if PY_MAJOR_VERSION >= 3

    unic = PyUnicode_Encode(PyUnicode_AS_UNICODE(str),
                            PyUnicode_GET_SIZE(str),
                            pyxser_xml_encoding,
                            pyxser_xml_encoding_mode);
    if (PYTHON_IS_NONE(unic)) {
        PyErr_Clear();
        unic = PyUnicode_FromEncodedObject(str, pyxser_xml_encoding,
                                           pyxser_xml_encoding_mode);
    }

    if (PYTHON_IS_NONE(unic)) {
        PyErr_Clear();
        return (xmlNodePtr)NULL;
    }

    sptr = PyString_AS_STRING(unic);
    PYXSER_FREE_OBJECT(unic);

#else

    sptr = PyString_AS_STRING(str);

#endif /* PY_MAJOR_VERSION >= 3 */

    if (sptr != (char *)NULL) {
        newElementNode = xmlNewDocNode(doc, pyxser_GetDefaultNs(),
                                       BAD_CAST pyxser_xml_element_prop,
                                       NULL);
        newTextNode = xmlNewDocText(doc, BAD_CAST sptr);
        if (name != (char *)NULL) {
            nameAttr = xmlNewProp(newElementNode,
                                  BAD_CAST pyxser_xml_attr_name,
                                  BAD_CAST name);
        }
        nptr = pyxser_GetClassName(o);
        if (nptr != (char *)NULL) {
            typeAttr = xmlNewProp(newElementNode,
                                  BAD_CAST pyxser_xml_attr_type,
                                  BAD_CAST nptr);
        }
        xmlAddChild(newElementNode, newTextNode);
    }
    PYXSER_FREE_OBJECT(className);
    PYXSER_FREE_OBJECT(classPtr);
    PYXSER_FREE_OBJECT(unic);
    PYXSER_FREE_OBJECT(str);
    PyErr_Clear();
	return newElementNode;
}


xmlNodePtr
pyxser_AddReference(PyObject *o, xmlNodePtr currentNode)
{
	xmlAttrPtr refAttr = (xmlAttrPtr)NULL;
	xmlNodePtr refNode = (xmlNodePtr)NULL;
	char newRef[41];
	unsigned long hash = 0;
	if (PYTHON_IS_NONE(o)
		|| currentNode == (xmlNodePtr)NULL) {
		return (xmlNodePtr)NULL;
	}
    if (PyWeakref_CheckRef(o)) {
		return (xmlNodePtr)NULL;
    }
	hash = (unsigned long)o;
    memset(newRef, 0, 41);
    snprintf(newRef, 41, "id%lu", hash);
    refAttr = xmlNewProp(currentNode,
                         BAD_CAST pyxser_xml_attr_ref,
                         BAD_CAST newRef);
	return refNode;
}


void
pyxser_AddIdentifier(PyObject *o, xmlNodePtr currentNode)
{
	xmlAttrPtr idAttr = (xmlAttrPtr)NULL;
	char newRef[41];
	unsigned long hash = 0;
	if (PYTHON_IS_NONE(o) || currentNode == (xmlNodePtr)NULL) {
		return;
	}
	hash = (unsigned long)o;
    memset(newRef, 0, 41);
    snprintf(newRef, 41, "id%lu", hash);
    idAttr = xmlNewProp(currentNode,
                        BAD_CAST pyxser_xml_attr_id,
                        BAD_CAST newRef);
}


int
pyxser_CheckBuiltInModule(PyObject *o)
{
    int r = 0;
	char *cn = (char *)NULL;
	PyObject *klass = Py_None;
	PyObject *mname = Py_None;
    PyObject *unic = (PyObject *)NULL;

	if (PYTHON_IS_NONE(o)) {
		return r;
	}
	klass = PyObject_GetAttrString(o, pyxser_attr_class);
	if (PYTHON_IS_NONE(klass)) {
        PyErr_Clear();
		return r;
	}
	mname = PyObject_GetAttrString(klass, pyxser_attr_module);
	if (PYTHON_IS_NONE(mname)) {
        PyErr_Clear();
        PYXSER_FREE_OBJECT(klass);
        return r;
	}
#if PY_MAJOR_VERSION >= 3
    unic = PyUnicode_Encode(PyUnicode_AS_UNICODE(mname),
                            PyUnicode_GET_SIZE(mname),
                            pyxser_xml_encoding,
                            pyxser_xml_encoding_mode);
    if (PYTHON_IS_NONE(unic)) {
        PyErr_Clear();
        unic = PyUnicode_FromEncodedObject(mname, pyxser_xml_encoding,
                                           pyxser_xml_encoding_mode);
    }

    if (PYTHON_IS_NONE(unic)) {
        PyErr_Clear();
        return 1;
    }

    cn = PyString_AS_STRING(unic);
    PYXSER_FREE_OBJECT(unic);
#else
    cn = PyString_AS_STRING(cname);
#endif /* PY_MAJOR_VERSION >= 3 */
    r = (!strncmp(cn, type_builtin, strlen(type_builtin)));
    PYXSER_FREE_OBJECT(klass);
    PYXSER_FREE_OBJECT(mname);
    return r;
}

void
pyxser_AddModuleAttr(PyObject *o, xmlNodePtr currentNode)
{
	char *cn = (char *)NULL;
	PyObject *klass = Py_None;
	PyObject *mname = Py_None;
    PyObject *unic = (PyObject *)NULL;
	xmlAttrPtr moduleAttr = (xmlAttrPtr)NULL;

	if (PYTHON_IS_NONE(o) ||
		currentNode == (xmlNodePtr)NULL) {
		return;
	}
	klass = PyObject_GetAttrString(o, pyxser_attr_class);
	if (PYTHON_IS_NONE(klass)) {
        PyErr_Clear();
		return;
	}
	mname = PyObject_GetAttrString(klass, pyxser_attr_module);
	if (PYTHON_IS_NONE(mname)) {
        PyErr_Clear();
        PYXSER_FREE_OBJECT(klass);
        return;
	}
#if PY_MAJOR_VERSION >= 3
    unic = PyUnicode_Encode(PyUnicode_AS_UNICODE(mname),
                            PyUnicode_GET_SIZE(mname),
                            pyxser_xml_encoding,
                            pyxser_xml_encoding_mode);
    if (PYTHON_IS_NONE(unic)) {
        PyErr_Clear();
        unic = PyUnicode_FromEncodedObject(mname, pyxser_xml_encoding,
                                           pyxser_xml_encoding_mode);
    }

    if (PYTHON_IS_NONE(unic)) {
        PyErr_Clear();
        return;
    }

    cn = PyString_AS_STRING(unic);
    PYXSER_FREE_OBJECT(unic);
#else
    cn = PyString_AS_STRING(mname);
#endif /* PY_MAJOR_VERSION >= 3 */
	moduleAttr = xmlNewProp(currentNode,
							BAD_CAST pyxser_xml_attr_module,
							BAD_CAST cn);
    PYXSER_FREE_OBJECT(klass);
    PYXSER_FREE_OBJECT(mname);
}


int
pyxser_PyListContains(PyListObject *lst, PyObject *o)
{
	PyObject *iterLst = (PyObject *)NULL;
	PyObject *item = (PyObject *)NULL;

	if (PYTHON_IS_NONE(lst) || PYTHON_IS_NONE(o)) {
		return PYXSER_INVALID_ARGUMENT;
	}
    if (!pyxserList_Check((PyObject *)lst)) {
		return PYXSER_INVALID_ARGUMENT;
    }
	iterLst = PyObject_GetIter((PyObject *)lst);
	if (PYTHON_IS_NONE(iterLst)) {
		return PYXSER_NOT_FOUND;
	}
	while ((item = PyIter_Next(iterLst))
		   != (PyObject *)NULL) {
        if (PYTHON_IS_NONE(item)) {
            break;
        }
        if (item == o) {
            PYXSER_FREE_OBJECT(item);
            PYXSER_FREE_OBJECT(iterLst);
            return PYXSER_FOUND;
        }
	}
    PYXSER_FREE_OBJECT(item);
    PYXSER_FREE_OBJECT(iterLst);
	return PYXSER_NOT_FOUND;
}

xmlNodePtr
pyxser_SerializeWeakref(PyxSerializationArgsPtr args)
{
    return NULL;
}

/* numbers */
int
pyxserInt_Check(PyObject *o)
{
	return PyInt_Check(o);
}

int
pyxserInt_CheckExact(PyObject *o)
{
	return PyInt_CheckExact(o);
}

int
pyxserBool_Check(PyObject *o)
{
	return PyBool_Check(o);
}

int
pyxserLong_CheckExact(PyObject *o)
{
	return PyLong_CheckExact(o);
}

int
pyxserFloat_Check(PyObject *o)
{
	return PyFloat_Check(o);
}

int
pyxserFloat_CheckExact(PyObject *o)
{
	return PyFloat_CheckExact(o);
}

int
pyxserComplex_Check(PyObject *o)
{
	return PyComplex_Check(o);
}

int
pyxserComplex_CheckExact(PyObject *o)
{
	return PyComplex_CheckExact(o);
}

/* strings */
int
pyxserString_Check(PyObject *o)
{
	return PyString_Check(o);
}

int
pyxserString_CheckExact(PyObject *o)
{
	return PyString_CheckExact(o);
}

int
pyxserUnicode_Check(PyObject *o)
{
	return PyUnicode_Check(o);
}

int
pyxserUnicode_CheckExact(PyObject *o)
{
	return PyUnicode_CheckExact(o);
}

int
pyxserBuffer_Check(PyObject *o)
{
#if PY_MAJOR_VERSION < 3
	return PyBuffer_Check(o);
#else
    return 0;
#endif /* PY_MAJOR_VERSION < 3 */
}

/* tuples */
int
pyxserTuple_Check(PyObject *o)
{
	return PyTuple_CheckExact(o);
}

int
pyxserTuple_CheckExact(PyObject *o)
{
	return PyTuple_CheckExact(o);
}

/* lists */
int
pyxserList_Check(PyObject *o)
{
	return PyList_Check(o);
}

int
pyxserList_CheckExact(PyObject *o)
{
	return PyList_CheckExact(o);
}

/* dictionaries */
int pyxserDict_Check(PyObject *o)
{
	return PyDict_CheckExact(o);
}

int
pyxserDict_CheckExact(PyObject *o)
{
	return PyDict_CheckExact(o);
}

/* files */
int
pyxserFile_Check(PyObject *o)
{
#if PY_MAJOR_VERSION < 3
	return PyFile_Check(o);
#else
    return 0;
#endif /* PY_MAJOR_VERSION < 3 */
}

int pyxserFile_CheckExact(PyObject *o)
{
#if PY_MAJOR_VERSION < 3
	return PyFile_CheckExact(o);
#else
    return 0;
#endif /* PY_MAJOR_VERSION < 3 */
}

/* instrospection */
int
pyxserInstance_Check(PyObject *o)
{
#if PY_MAJOR_VERSION < 3
	return PyInstance_Check(o);
#else
    return 0;
#endif /* PY_MAJOR_VERSION < 3 */
}

int
pyxserFunction_Check(PyObject *o)
{
	return PyFunction_Check(o);
}

int
pyxserMethod_Check(PyObject *o)
{
	return PyMethod_Check(o);
}

int
pyxserModule_Check(PyObject *o)
{
	return PyModule_Check(o);
}

int
pyxserModule_CheckExact(PyObject *o)
{
	return PyModule_CheckExact(o);
}

int
pyxserSeqIter_Check(PyObject *o)
{
	return PySeqIter_Check(o);
}

int
pyxserSlice_Check(PyObject *o)
{
	return PySlice_Check(o);
}

int
pyxserCell_Check(PyObject *o)
{
	return PyCell_Check(o);
}

int
pyxserGen_Check(PyObject *o)
{
	return PyGen_Check(o);
}

int
pyxserDate_CheckExact(PyObject *o)
{
	PyDateTime_IMPORT;
	return PyDate_Check(o);
}

int
pyxserAnySet_Check(PyObject *o)
{
	return PyAnySet_CheckExact(o);
}

int
pyxserAnySet_CheckExact(PyObject *o)
{
	return PyAnySet_CheckExact(o);
}

int
pyxserFrozenSet_CheckExact(PyObject *o)
{
	return PyFrozenSet_CheckExact(o);
}

int
pyxserWeakref_Check(PyObject *o)
{
    return (PyWeakref_Check(o));
}

int
pyxserSequence_Check(PyObject *o)
{
    return PySequence_Check(o);
}

PyObject *
pyxser_PyInstance_NewRaw(PyObject *cls)
{
#if PY_MAJOR_VERSION < 3
	PyObject *unser = (PyObject *)NULL;
    PyObject *ndict = (PyObject *)NULL;
    ndict = PyDict_New();
    unser = PyInstance_NewRaw(cls, ndict);
    if (PYTHON_IS_NONE(unser)) {
        Py_XDECREF(ndict);
        return NULL;
    }
    Py_XDECREF(ndict);
    return unser;
#else /* PY_MAJOR_VERSION >= 3 */
    PyObject *inst;
    PyObject *dict = PyDict_New();
    Py_INCREF(dict);
    inst = _PyObject_GC_New(cls->ob_type);
    if (inst == NULL) {
        Py_DECREF(dict);
        return NULL;
    }
    Py_INCREF(cls);
    _PyObject_GC_TRACK(inst);
    return (PyObject *)inst;
#endif /* PY_MAJOR_VERSION < 3 */
}

int
pyxunserInt_Check(xmlNodePtr node)
{
	char *prop;
	if (node == (xmlNodePtr)NULL) {
        return 0;
    }
    prop = (char *)xmlGetProp(node, BAD_CAST pyxser_xml_attr_type);
    if (prop == (char *)NULL) {
        return 0;
    }
    if ((strncmp(prop, type_int, strlen(type_int))) == 0) {
        PYXSER_XMLFREE(prop);
        return 1;
    }
    PYXSER_XMLFREE(prop);
	return 0;
}

int
pyxunserInt_CheckExact(xmlNodePtr node)
{
	return pyxunserInt_Check(node);
}


int
pyxunserBool_Check(xmlNodePtr node)
{
	char *prop;
	if (node == (xmlNodePtr)NULL) {
        return 0;
    }
    prop = (char *)xmlGetProp(node, BAD_CAST pyxser_xml_attr_type);
    if (prop == (char *)NULL) {
        return 0;
    }
    if ((strncmp(prop, type_bool, strlen(type_bool))) == 0) {
        PYXSER_XMLFREE(prop);
        return 1;
    }
    PYXSER_XMLFREE(prop);
	return 0;
}

int
pyxunserLong_CheckExact(xmlNodePtr node)
{
	char *prop;
	if (node == (xmlNodePtr)NULL) {
        return 0;
    }
    prop = (char *)xmlGetProp(node, BAD_CAST pyxser_xml_attr_type);
    if (prop == (char *)NULL) {
        return 0;
    }
    if ((strncmp(prop, type_long, strlen(type_long))) == 0) {
        PYXSER_XMLFREE(prop);
        return 1;
    }
    PYXSER_XMLFREE(prop);
	return 0;
}

int
pyxunserFloat_Check(xmlNodePtr node)
{
	char *prop;
	if (node == (xmlNodePtr)NULL) {
        return 0;
    }
    prop = (char *)xmlGetProp(node, BAD_CAST pyxser_xml_attr_type);
    if (prop == (char *)NULL) {
        return 0;
    }
    if ((strncmp(prop, type_float, strlen(type_float))) == 0) {
        PYXSER_XMLFREE(prop);
        return 1;
    }
    PYXSER_XMLFREE(prop);
	return 0;
}

int
pyxunserFloat_CheckExact(xmlNodePtr node)
{
	char *prop;
	if (node == (xmlNodePtr)NULL) {
        return 0;
    }
    prop = (char *)xmlGetProp(node, BAD_CAST pyxser_xml_attr_type);
    if (prop == (char *)NULL) {
        return 0;
    }
    if ((strncmp(prop, type_float, strlen(type_float))) == 0) {
        PYXSER_XMLFREE(prop);
        return 1;
    }
    PYXSER_XMLFREE(prop);
	return 0;
}

int
pyxunserComplex_Check(xmlNodePtr node)
{
	char *prop;
	if (node == (xmlNodePtr)NULL) {
        return 0;
    }
    prop = (char *)xmlGetProp(node, BAD_CAST pyxser_xml_attr_type);
    if (prop == (char *)NULL) {
        return 0;
    }
    if ((strncmp(prop, type_complex, strlen(type_complex))) == 0) {
        PYXSER_XMLFREE(prop);
        return 1;
    }
    PYXSER_XMLFREE(prop);
	return 0;
}

int
pyxunserComplex_CheckExact(xmlNodePtr node)
{
	char *prop;
	if (node == (xmlNodePtr)NULL) {
        return 0;
    }
    prop = (char *)xmlGetProp(node, BAD_CAST pyxser_xml_attr_type);
    if (prop == (char *)NULL) {
        return 0;
    }
    if ((strncmp(prop, type_complex, strlen(type_complex))) == 0) {
        PYXSER_XMLFREE(prop);
        return 1;
    }
    PYXSER_XMLFREE(prop);
	return 0;
}

int
pyxunserString_Check(xmlNodePtr node)
{
	char *prop;
	if (node == (xmlNodePtr)NULL) {
        return 0;
    }
    prop = (char *)xmlGetProp(node, BAD_CAST pyxser_xml_attr_type);
    if (prop == (char *)NULL) {
        return 0;
    }
    if ((strncmp(prop, type_str, strlen(type_str))) == 0) {
        PYXSER_XMLFREE(prop);
        return 1;
    }
    PYXSER_XMLFREE(prop);
	return 0;
}

int
pyxunserString_CheckExact(xmlNodePtr node)
{
	char *prop;
	if (node == (xmlNodePtr)NULL) {
        return 0;
    }
    prop = (char *)xmlGetProp(node, BAD_CAST pyxser_xml_attr_type);
    if (prop == (char *)NULL) {
        return 0;
    }
    if ((strncmp(prop, type_str, strlen(type_str))) == 0) {
        PYXSER_XMLFREE(prop);
        return 1;
    }
    PYXSER_XMLFREE(prop);
	return 0;
}

int
pyxunserUnicode_Check(xmlNodePtr node)
{
	char *prop;
	if (node == (xmlNodePtr)NULL) {
        return 0;
    }
    prop = (char *)xmlGetProp(node, BAD_CAST pyxser_xml_attr_type);
    if (prop == (char *)NULL) {
        return 0;
    }
    if ((strncmp(prop, type_unicode, strlen(type_unicode))) == 0) {
        PYXSER_XMLFREE(prop);
        return 1;
    }
    PYXSER_XMLFREE(prop);
	return 0;
}

int
pyxunserUnicodeExact_CheckExact(xmlNodePtr node)
{
	char *prop;
	if (node == (xmlNodePtr)NULL) {
        return 0;
    }
    prop = (char *)xmlGetProp(node, BAD_CAST pyxser_xml_attr_type);
    if (prop == (char *)NULL) {
        return 0;
    }
    if ((strncmp(prop, type_unicode, strlen(type_unicode))) == 0) {
        PYXSER_XMLFREE(prop);
        return 1;
    }
    PYXSER_XMLFREE(prop);
	return 0;
}

int
pyxunserBuffer_Check(xmlNodePtr node)
{
	char *prop;
	if (node == (xmlNodePtr)NULL) {
        return 0;
    }
    prop = (char *)xmlGetProp(node, BAD_CAST pyxser_xml_attr_type);
    if (prop == (char *)NULL) {
        return 0;
    }
    if ((strncmp(prop, type_buffer, strlen(type_buffer))) == 0) {
        PYXSER_XMLFREE(prop);
        return 1;
    }
    PYXSER_XMLFREE(prop);
	return 0;
}

int
pyxunserTuple_Check(xmlNodePtr node)
{
	char *prop;
	if (node == (xmlNodePtr)NULL) {
        return 0;
    }
    prop = (char *)xmlGetProp(node, BAD_CAST pyxser_xml_attr_type);
    if (prop == (char *)NULL) {
        return 0;
    }
    if ((strncmp(prop, type_tuple, strlen(type_tuple))) == 0) {
        PYXSER_XMLFREE(prop);
        return 1;
    }
    PYXSER_XMLFREE(prop);
	return 0;
}

int
pyxunserList_Check(xmlNodePtr node)
{
	char *prop;
	if (node == (xmlNodePtr)NULL) {
        return 0;
    }
    prop = (char *)xmlGetProp(node, BAD_CAST pyxser_xml_attr_type);
    if (prop == (char *)NULL) {
        return 0;
    }
    if ((strncmp(prop, type_list, strlen(type_list))) == 0) {
        PYXSER_XMLFREE(prop);
        return 1;
    }
    PYXSER_XMLFREE(prop);
	return 0;
}

int
pyxunserDict_Check(xmlNodePtr node)
{
	char *prop;
	if (node == (xmlNodePtr)NULL) {
        return 0;
    }
    prop = (char *)xmlGetProp(node, BAD_CAST pyxser_xml_attr_type);
    if (prop == (char *)NULL) {
        return 0;
    }
    if ((strncmp(prop, type_dict, strlen(type_dict))) == 0) {
        PYXSER_XMLFREE(prop);
        return 1;
    }
    PYXSER_XMLFREE(prop);
	return 0;
}

int
pyxunserSet_Check(xmlNodePtr node)
{
	char *prop;
	if (node == (xmlNodePtr)NULL) {
        return 0;
    }
    prop = (char *)xmlGetProp(node, BAD_CAST pyxser_xml_attr_type);
    if (prop == (char *)NULL) {
        return 0;
    }
    if ((strncmp(prop, type_set, strlen(type_set))) == 0) {
        PYXSER_XMLFREE(prop);
        return 1;
    }
    PYXSER_XMLFREE(prop);
	return 0;
}

int
pyxunserSequence_Check(xmlNodePtr node)
{
	char *prop;
	if (node == (xmlNodePtr)NULL) {
        return 0;
    }
    prop = (char *)xmlGetProp(node, BAD_CAST pyxser_xml_attr_type);
    if (prop == (char *)NULL) {
        return 0;
    }
    if ((strncmp(prop, type_sequence, strlen(type_sequence))) == 0) {
        PYXSER_XMLFREE(prop);
        return 1;
    }
    PYXSER_XMLFREE(prop);
	return 0;
}

int
pyxunserAnySet_Check(xmlNodePtr node)
{
    return pyxunserSet_Check(node);
}

int
pyxunserAnySet_CheckExact(xmlNodePtr node)
{
    return pyxunserSet_Check(node);
}

int
pyxunserFrozenSet_CheckExact(xmlNodePtr node)
{
    return pyxunserSet_Check(node);
}

xmlNsPtr
pyxser_GetDefaultNs(void)
{
	if (pyxser_default_ns != (xmlNsPtr)NULL) {
		return pyxser_default_ns;
	} else {
		pyxser_default_ns = xmlNewNs(NULL,
									 BAD_CAST pyxser_xml_attr_ns,
									 BAD_CAST pyxser_xml_attr_ns_prefix);
		return pyxser_default_ns;
	}
}

xmlNsPtr
pyxser_GetXsiNs(void)
{
	if (pyxser_xsi_ns != (xmlNsPtr)NULL) {
		return pyxser_xsi_ns;
	} else {
		pyxser_xsi_ns = xmlNewNs(NULL,
                                 BAD_CAST pyxser_xml_attr_xml_xsi_value,
                                 BAD_CAST pyxser_xml_attr_xml_xsi);
		return pyxser_xsi_ns;
	}
}

xmlNsPtr
pyxser_GetXsdNs(void)
{
	if (pyxser_xsd_ns != (xmlNsPtr)NULL) {
		return pyxser_xsi_ns;
	} else {
		pyxser_xsd_ns = xmlNewNs(NULL,
                                 BAD_CAST pyxser_xml_attr_xml_xsd,
                                 BAD_CAST pyxser_xml_attr_xml_xsd_prfx);
		return pyxser_xsd_ns;
	}
}

char *
pyxser_ExtractPropertyName(const char *name, xmlNodePtr node)
{
	if (node != (xmlNodePtr)NULL && name != (char *)NULL) {
		return (char *)xmlGetProp(node, BAD_CAST name);
	}
	return (char *)NULL;
}


PyObject *
pyxser_SearchObjectInMain(const char *name)
{
	PyObject *dictMod = PyImport_GetModuleDict();
	PyObject *item = (PyObject *)NULL;
	PyObject *currentKey = (PyObject *)NULL;
	PyObject *dictKeys = (PyObject *)NULL;
	PyObject *ct = (PyObject *)NULL;
    char *ck = (char *)NULL;
	int counter = 0;
	long tupleSize = 0;
	if (PYTHON_IS_NONE(dictMod)) {
		return (PyObject *)NULL;
	}
	dictKeys = PyDict_Keys(dictMod);
	tupleSize = PyDict_Size(dictMod);
	if (PYTHON_IS_NONE(dictKeys)) {
		return (PyObject *)NULL;
	}
	for (counter = 0; counter < tupleSize; counter++) {
		currentKey = PyList_GetItem(dictKeys, counter);
        ck = PyString_AS_STRING(currentKey);
        if (ck == (char *)NULL) {
            continue;
        }
        if ((strncmp(ck, type_main, strlen(type_main))) == 0) {
            item = PyDict_GetItem(dictMod, currentKey);
            if (PYTHON_IS_NONE(item)) {
                continue;
            }
            ct = (PyObject *)pyxser_SearchModuleType(item,
                                                     name);
            break;
        }
	}
    Py_XDECREF(dictKeys);
	return ct;
}


PyObject *
pyxser_SearchModule(const char *name)
{
	PyObject *dictMod = PyImport_GetModuleDict();
	PyObject *item = (PyObject *)NULL;
	PyObject *currentKey = (PyObject *)NULL;
	PyObject *dictKeys = (PyObject *)NULL;
	int counter = 0;
	long tupleSize = 0;
    if (name == (const char *)NULL) {
		return (PyObject *)NULL;
    }
	if (PYTHON_IS_NONE(dictMod)) {
		return (PyObject *)NULL;
	}
	dictKeys = PyDict_Keys(dictMod);
	tupleSize = PyDict_Size(dictMod);
	if (PYTHON_IS_NONE(dictKeys)) {
		return (PyObject *)NULL;
	}
	for (counter = 0; counter < tupleSize; counter++) {
		currentKey = PyList_GetItem(dictKeys, counter);
		if ((strncmp(PyString_AS_STRING(currentKey),
					 name,
					 strlen(type_main))) == 0) {
			item = PyDict_GetItem(dictMod, currentKey);
            break;
		}
	}
    Py_XDECREF(dictKeys);
	return item;
}

PyObject *
pyxser_SearchTypesInModules(const char *n_module,
                            const char *n_type,
                            PyDictObject **modules)
{
    PyObject *cacheMod;
    PyObject *chkMod;
    if (n_module == (const char *)NULL
        || n_type == (const char *)NULL) {
        return NULL;
    }
    if ((pyxser_ModuleNotMain(n_module)) == 0) {
        return NULL;
    }
    cacheMod = pyxser_SearchModule(n_module);
    cacheMod = cacheMod == (PyObject *)NULL ?
        PyImport_ImportModule(n_module) : cacheMod;
    if (PYTHON_IS_NONE(cacheMod)) {
        return NULL;
    }
    chkMod = pyxser_CacheModule((PyObject *)*modules, n_module);
    if (!PYTHON_IS_NOT_NONE(chkMod)) {
        PyDict_SetItemString((PyObject *)*modules,
                             n_module, cacheMod);
    }
    return (PyObject *)pyxser_SearchModuleType(cacheMod, n_type);
}

PyObject *
pyxser_CacheModule(PyObject *d, const char *name)
{
	PyObject *dictMod = d;
	PyObject *item = (PyObject *)NULL;
	PyObject *currentKey = (PyObject *)NULL;
	PyObject *dictKeys = (PyObject *)NULL;
	int counter = 0;
	long tupleSize = 0;
	if (PYTHON_IS_NONE(dictMod)) {
		return Py_None;
	}
	tupleSize = PyDict_Size(dictMod);
	if (tupleSize > 0) {
		dictKeys = PyDict_Keys(dictMod);
		if (PYTHON_IS_NONE(dictKeys)) {
			return Py_None;
		}
		for (counter = 0; counter < tupleSize; counter++) {
			currentKey = PyList_GetItem(dictKeys, counter);
			if ((strncmp(PyString_AS_STRING(currentKey),
						 name,
						 strlen(type_main))) == 0) {
				item = PyDict_GetItem(dictMod, currentKey);
                break;
			}
		}
        Py_XDECREF(dictKeys);
	}
	return item;
}


PyObject *
pyxser_GetStringFromUnicode(PyObject *unic)
{
    PyObject *enc;

    if (unic == (PyObject *)NULL) {
        return (PyObject *)NULL;
    }

    enc = PyUnicode_Encode(PyUnicode_AS_UNICODE(unic),
                            PyUnicode_GET_SIZE(unic),
                            pyxser_xml_encoding,
                            pyxser_xml_encoding_mode);
    if (PYTHON_IS_NONE(enc)) {
        PyErr_Clear();
        enc = PyUnicode_FromEncodedObject(unic, pyxser_xml_encoding,
                                          pyxser_xml_encoding_mode);
    }

    if (PYTHON_IS_NONE(enc)) {
        PyErr_Clear();
        return (xmlNodePtr)NULL;
    }

    return enc;

}


#if defined(_WIN32) || defined(_WIN64)
char *
#else
inline char *
#endif /* !_WIN32 && !_WIN64 */
pyxser_GetObjectIdentifier(xmlNodePtr node)
{
	if (node != (xmlNodePtr)NULL) {
		return (char *)xmlGetProp(node, BAD_CAST pyxser_xml_attr_id);
	}
	return (char *)NULL;
}

PyObject *
pyxser_CheckAvailableObject(PyObject *dict, char *id)
{
	PyObject *ret = (PyObject *)NULL;
	if (dict != (PyObject *)NULL
		&& id != (char *)NULL) {
		ret = PyDict_GetItemString(dict, id);
	}
	return ret;
}

void
pyxser_AddAvailableObject(PyObject *dict, char *id, PyObject *o)
{
	if (PYTHON_IS_NONE(dict)) {
		return;
	}
	if (id == (char *)NULL) {
		return;
	}
	if (PYTHON_IS_NONE(o)) {
		return;
	}
	PyDict_SetItemString(dict, id, o);
}


xmlDtdPtr
pyxser_GetPyxserDTD()
{
	if (pyxser_dtd_object == (xmlDtdPtr)NULL) {
		pyxser_dtd_object = xmlParseDTD(BAD_CAST NULL,
										BAD_CAST pyxser_xml_dtd_location);
	}
	return pyxser_dtd_object;
}

xmlDtdPtr
pyxser_GetPyxserDTDC14N()
{
	if (pyxser_dtd_c14n_object == (xmlDtdPtr)NULL) {
		pyxser_dtd_c14n_object = xmlParseDTD(BAD_CAST NULL,
                                             BAD_CAST pyxser_xml_dtd_c14n_location);
	}
	return pyxser_dtd_c14n_object;
}

void
pyxser_validity_exception(void *ctx, const char *msg, va_list args)
{
    char error_buffer[1024];
    memset(error_buffer, 0, 512);
    sprintf(error_buffer, msg, args);
    PyErr_SetString(invalid_xml_exception, error_buffer);
}

static int
px_printf(FILE *in, const char *pt, ...)
{
    return 0;
}

int
pyxser_ValidateDocument(xmlDocPtr doc)
{
	xmlDtdPtr dtd = pyxser_GetPyxserDTD();
	xmlValidCtxtPtr cvp;
	if ((cvp = xmlNewValidCtxt()) == NULL) {
		return 0;
	}
	cvp->userData = (void *) stderr;
	cvp->error = (xmlValidityErrorFunc) px_printf;
	cvp->warning = (xmlValidityWarningFunc) px_printf;
	if (!xmlValidateDtd(cvp, doc, dtd)) {
        xmlFreeValidCtxt(cvp);
        return 0;
	}
    xmlFreeValidCtxt(cvp);
	return 1;
}

int
pyxser_ValidateDocumentC14N(xmlDocPtr doc)
{
	xmlDtdPtr dtd = pyxser_GetPyxserDTDC14N();
	xmlValidCtxtPtr cvp;
	if ((cvp = xmlNewValidCtxt()) == NULL) {
		return 0;
	}
#ifdef PYXSER_DEBUG
#warning USING DEBUG!
	cvp->userData = (void *) stderr;
	cvp->error = (xmlValidityErrorFunc) px_printf;
	cvp->warning = (xmlValidityWarningFunc) px_printf;
#else
	cvp->userData = (void *) NULL;
	cvp->error = (xmlValidityErrorFunc) pyxser_validity_exception;
	cvp->warning = (xmlValidityWarningFunc) pyxser_validity_exception;
#endif
	if (!xmlValidateDtd(cvp, doc, dtd)) {
        xmlFreeValidCtxt(cvp);
		return 0;
	}
	xmlFreeValidCtxt(cvp);
	return 1;
}

int
pyxser_ValidateDocumentXSD(xmlDocPtr doc)
{
    xmlSchemaPtr scm = (xmlSchemaPtr)NULL;
    xmlSchemaValidCtxtPtr ctx = (xmlSchemaValidCtxtPtr)NULL;
    xmlSchemaParserCtxtPtr pyxser_xsd_parser_object = (xmlSchemaParserCtxtPtr)NULL;
    xmlSchemaPtr pyxser_xsd_object = (xmlSchemaPtr)NULL;
    int ctrl = 0;
    if (pyxser_xsd_doc == (xmlDocPtr)NULL) {
        pyxser_xsd_doc = (xmlDocPtr)xmlReadMemory(pyxser_xsd_body,
                                                  strlen(pyxser_xsd_body),
                                                  NULL,
                                                  schema_encoding,
                                                  XML_PARSE_RECOVER);
    }
	if (pyxser_xsd_parser_object == (xmlSchemaParserCtxtPtr)NULL) {
		pyxser_xsd_parser_object = xmlSchemaNewDocParserCtxt(pyxser_xsd_doc);
        xmlSchemaSetParserErrors(pyxser_xsd_parser_object,
                                 (xmlSchemaValidityErrorFunc) fprintf,
                                 (xmlSchemaValidityWarningFunc) fprintf,
                                 stderr);
	}
    if (pyxser_xsd_object == (xmlSchemaPtr)NULL) {
        pyxser_xsd_object = xmlSchemaParse(pyxser_xsd_parser_object);
    }
    scm = pyxser_xsd_object;
    if (scm != NULL && doc != NULL) {
        ctx = xmlSchemaNewValidCtxt(scm);
        if (ctx != (xmlSchemaValidCtxtPtr)NULL) {
            xmlSchemaSetValidOptions(ctx, 0);
            xmlSchemaSetValidErrors(ctx,
                                    (xmlSchemaValidityErrorFunc) fprintf,
                                    (xmlSchemaValidityWarningFunc) fprintf,
                                    stderr);
            ctrl = xmlSchemaValidateDoc(ctx, doc);
            if (ctrl == 0) {
                ctrl = 1;
            } else {
                ctrl = 0;
            }
            xmlSchemaFreeValidCtxt(ctx);
        }
        xmlSchemaFree(pyxser_xsd_object);
    }
    if (pyxser_xsd_parser_object != NULL) {
        xmlSchemaFreeParserCtxt(pyxser_xsd_parser_object);
    }
    return ctrl;
}

int
pyxser_ValidateDocumentXSDC14N(xmlDocPtr doc)
{
    xmlSchemaPtr scm;
    xmlSchemaValidCtxtPtr ctx = (xmlSchemaValidCtxtPtr)NULL;
    xmlSchemaParserCtxtPtr pyxser_xsd_parser_c14n_object = (xmlSchemaParserCtxtPtr)NULL;
    xmlSchemaPtr pyxser_xsd_c14n_object = (xmlSchemaPtr)NULL;
    int ctrl = 0;
    if (pyxser_xsd_c14n_doc == (xmlDocPtr)NULL) {
        pyxser_xsd_c14n_doc = (xmlDocPtr)xmlReadMemory(pyxser_xsd_body,
                                                       strlen(pyxser_xsd_body),
                                                       NULL,
                                                       schema_encoding,
                                                       XML_PARSE_RECOVER);
    }
	if (pyxser_xsd_parser_c14n_object == (xmlSchemaParserCtxtPtr)NULL) {
		pyxser_xsd_parser_c14n_object = xmlSchemaNewDocParserCtxt(pyxser_xsd_c14n_doc);
        xmlSchemaSetParserErrors(pyxser_xsd_parser_c14n_object,
                                 (xmlSchemaValidityErrorFunc) fprintf,
                                 (xmlSchemaValidityWarningFunc) fprintf,
                                 stderr);
	}
    if (pyxser_xsd_c14n_object == (xmlSchemaPtr)NULL) {
        pyxser_xsd_c14n_object = xmlSchemaParse(pyxser_xsd_parser_c14n_object);
    }
    scm = pyxser_xsd_c14n_object;
    if (scm != NULL && doc != NULL) {
        ctx = xmlSchemaNewValidCtxt(scm);
        if (ctx != (xmlSchemaValidCtxtPtr)NULL) {
            xmlSchemaSetValidOptions(ctx, 0);
            xmlSchemaSetValidErrors(ctx,
                                    (xmlSchemaValidityErrorFunc) fprintf,
                                    (xmlSchemaValidityWarningFunc) fprintf,
                                    stderr);
            ctrl = xmlSchemaValidateDoc(ctx, doc);
            if (ctrl == 0) {
                ctrl = 1;
            } else {
                ctrl = 0;
            }
            xmlSchemaFreeValidCtxt(ctx);
        }
        xmlSchemaFree(pyxser_xsd_c14n_object);
    }
    if (pyxser_xsd_parser_c14n_object != NULL) {
        xmlSchemaFreeParserCtxt(pyxser_xsd_parser_c14n_object);
    }
    return ctrl;
}

int
pyxser_C14NRenderAllNodes(void * user_data,
                          xmlNodePtr node,
                          xmlNodePtr parent)
{
    return 1;
}

/* pyserx_tools.h ends here */
