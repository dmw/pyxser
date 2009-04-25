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

#include <libxml/parser.h>
#include <libxml/tree.h>

#include "include/pyxser.h"
#include "include/pyxser_tools.h"

static const char pyxser_true_char_value[] = "True";
static const char pyxser_false_char_value[] = "False";


xmlNodePtr
pyxser_SerializeInt(PyObject *o, char *name,
					PyListObject *dupItems, xmlDocPtr doc,
                    int *depth, int *depthcnt)
{
	return pyxser_SerializePrimitiveType(o, name,
										 dupItems, doc,
                                         depth, depthcnt);
}

xmlNodePtr
pyxser_SerializeExactInt(PyObject *o, char *name,
						 PyListObject *dupItems, xmlDocPtr doc,
                         int *depth, int *depthcnt)
{
	return pyxser_SerializePrimitiveType(o, name,
										 dupItems, doc,
                                         depth, depthcnt);
}

xmlNodePtr
pyxser_SerializeBoolean(PyObject *o, char *name,
						PyListObject *dupItems, xmlDocPtr doc,
                        int *depth, int *depthcnt)
{
	return pyxser_SerializePrimitiveType(o, name,
										 dupItems, doc,
                                         depth, depthcnt);
}

xmlNodePtr
pyxser_SerializeLong(PyObject *o, char *name,
					 PyListObject *dupItems, xmlDocPtr doc,
                     int *depth, int *depthcnt)
{
	return pyxser_SerializePrimitiveType(o, name,
										 dupItems, doc,
                                         depth, depthcnt);
}

xmlNodePtr
pyxser_SerializeFloat(PyObject *o, char *name,
					  PyListObject *dupItems, xmlDocPtr doc,
                      int *depth, int *depthcnt)
{
	return pyxser_SerializePrimitiveType(o, name,
										 dupItems, doc,
                                         depth, depthcnt);
}

xmlNodePtr
pyxser_SerializeExactFloat(PyObject *o, char *name,
						   PyListObject *dupItems, xmlDocPtr doc,
                           int *depth, int *depthcnt)
{
	return pyxser_SerializePrimitiveType(o, name,
										 dupItems, doc,
                                         depth, depthcnt);
}

xmlNodePtr
pyxser_SerializeComplex(PyObject *o, char *name,
						PyListObject *dupItems, xmlDocPtr doc,
                        int *depth, int *depthcnt)
{
	PyObject *classPtr = (PyObject *)NULL;
	PyObject *className = (PyObject *)NULL;
    Py_complex cx;

	xmlNodePtr newElementNode = (xmlNodePtr)NULL;
	xmlNodePtr newTextNode = (xmlNodePtr)NULL;
	xmlAttrPtr typeAttr = (xmlAttrPtr)NULL;
	xmlAttrPtr nameAttr = (xmlAttrPtr)NULL;

	char *nptr = (char *)NULL;
    char sptr[128];

    if (PYTHON_IS_NONE(o)
        || name == (char *)NULL) {
        return (xmlNodePtr)NULL;
    }
    cx = PyComplex_AsCComplex(o);
    memset((void *)sptr, 0, 128);
    sprintf(sptr, "%lf:%lf", cx.real, cx.imag);
	classPtr = PyObject_GetAttrString(o, pyxser_attr_class);
	if (PYTHON_IS_NONE(classPtr)
		|| sptr == (char *)NULL) {
        PyErr_Clear();
		PYXSER_FREE_OBJECT(classPtr);
		return (xmlNodePtr)NULL;
	}
	className = PyObject_GetAttrString(classPtr, pyxser_attr_name);
	if (PYTHON_IS_NONE(className)) {
        PyErr_Clear();
		PYXSER_FREE_OBJECT(classPtr);
		PYXSER_FREE_OBJECT(className);
		return (xmlNodePtr)NULL;
	}
	if (PYTHON_IS_NOT_NONE(className)) {
		nptr = PyString_AS_STRING(className);
		if (sptr != (char *)NULL
			&& nptr != (char *)NULL) {
			newElementNode = xmlNewDocNode(doc, pyxser_GetDefaultNs(),
										   BAD_CAST pyxser_xml_element_prop,
										   NULL);
			newTextNode = xmlNewDocText(doc, BAD_CAST sptr);
			typeAttr = xmlNewProp(newElementNode,
								  BAD_CAST pyxser_xml_attr_type,
								  BAD_CAST nptr);
			if (name != (char *)NULL) {
				nameAttr = xmlNewProp(newElementNode,
									  BAD_CAST pyxser_xml_attr_name,
									  BAD_CAST name);
			}
			xmlAddChild(newElementNode, newTextNode);
		}
	}
    PYXSER_FREE_OBJECT(className);
	PYXSER_FREE_OBJECT(classPtr);
	return newElementNode;
}

xmlNodePtr
pyxser_SerializeExactComplex(PyObject *o, char *name,
							 PyListObject *dupItems, xmlDocPtr doc,
                             int *depth, int *depthcnt)
{
	return pyxser_SerializeComplex(o, name,
                                   dupItems, doc, depth, depthcnt);
}

PyObject *
pyxunser_SerializeInt(PyxSerDeserializationArgsPtr obj)
{
	xmlNodePtr node = *(obj->currentNode);
	xmlNodePtr ron;
	PyObject *res = Py_None;
	if (node != (xmlNodePtr)NULL) {
		for (ron = node->children;
			 ron;
			 ron = ron->next) {
			if (ron->type == XML_TEXT_NODE) {
				res = PyInt_FromString((char *)ron->content, NULL, 10);
			}
		}
	}
	return res;
}

PyObject *
pyxunser_SerializeExactInt(PyxSerDeserializationArgsPtr obj)
{
	xmlNodePtr node = *(obj->currentNode);
	xmlNodePtr ron;
	PyObject *res = Py_None;
	if (node != (xmlNodePtr)NULL) {
		for (ron = node->children;
			 ron;
			 ron = ron->next) {
			if (ron->type == XML_TEXT_NODE) {
				res = PyInt_FromString((char *)ron->content, NULL, 10);
			}
		}
	}
	return res;
}

PyObject *
pyxunser_SerializeBoolean(PyxSerDeserializationArgsPtr obj)
{
	xmlNodePtr node = *(obj->currentNode);
	xmlNodePtr ron;
	PyObject *res = Py_None;
	if (node != (xmlNodePtr)NULL) {
		for (ron = node->children;
			 ron;
			 ron = ron->next) {
			if (ron->type == XML_TEXT_NODE) {
				if (strncmp((char *)ron->content,
							pyxser_true_char_value,
							strlen(pyxser_true_char_value)) == 0) {
					return Py_True;
				} else if (strncmp((char *)ron->content,
								   pyxser_false_char_value,
								   strlen(pyxser_false_char_value)) == 0) {
					return Py_False;
				}
			}
		}
	}
	return res;
}

PyObject *
pyxunser_SerializeLong(PyxSerDeserializationArgsPtr obj)
{
	xmlNodePtr node = *(obj->currentNode);
	xmlNodePtr ron;
	PyObject *res = Py_None;
	if (node != (xmlNodePtr)NULL) {
		for (ron = node->children;
			 ron;
			 ron = ron->next) {
			if (ron->type == XML_TEXT_NODE) {
				res = PyInt_FromString((char *)ron->content, NULL, 10);
			}
		}
	}
	return res;
}

PyObject *
pyxunser_SerializeFloat(PyxSerDeserializationArgsPtr obj)
{
	xmlNodePtr node = *(obj->currentNode);
	xmlNodePtr ron;
	PyObject *str = Py_None;
	PyObject *res = Py_None;
	if (node != (xmlNodePtr)NULL) {
		for (ron = node->children;
			 ron;
			 ron = ron->next) {
			if (ron->type == XML_TEXT_NODE) {
				if (ron->content != (xmlChar *)NULL) {
					str = PyString_FromString((char *)ron->content);
					if (PYTHON_IS_NOT_NONE(str)) {
						res = PyFloat_FromString(str, NULL);
                        Py_DECREF(str);
					}
				}
			}
		}
	}
	return res;
}

PyObject *
pyxunser_SerializeExactFloat(PyxSerDeserializationArgsPtr obj)
{
	xmlNodePtr node = *(obj->currentNode);
	xmlNodePtr ron;
	PyObject *str = Py_None;
	PyObject *res = Py_None;
	if (node != (xmlNodePtr)NULL) {
		for (ron = node->children;
			 ron;
			 ron = ron->next) {
			if (ron->type == XML_TEXT_NODE) {
				if (ron->content != (xmlChar *)NULL) {
					str = PyString_FromString((char *)ron->content);
					if (PYTHON_IS_NOT_NONE(str)) {
						res = PyFloat_FromString(str, NULL);
                        Py_DECREF(str);
					}
				}
			}
		}
	}
	return res;
}

PyObject *
pyxunser_SerializeComplex(PyxSerDeserializationArgsPtr obj)
{
    PyObject *cxo = (PyObject *)NULL;
	xmlNodePtr node = *(obj->currentNode);
	xmlNodePtr ron;
	PyObject *res = Py_None;
    Py_complex cx;

	if (node != (xmlNodePtr)NULL) {
		for (ron = node->children;
			 ron;
			 ron = ron->next) {
			if (ron->type == XML_TEXT_NODE) {
				if (ron->content != (xmlChar *)NULL) {
                    sscanf((char *)ron->content, "%lf:%lf", &cx.real, &cx.imag);
                    cxo = PyComplex_FromCComplex(cx);
					if (PYTHON_IS_NOT_NONE(cxo)) {
						res = cxo;
                    }
				}
			}
		}
	}
	return res;
}

PyObject *
pyxunser_SerializeExactComplex(PyxSerDeserializationArgsPtr obj)
{
    return pyxunser_SerializeComplex(obj);
}

/* pyserx_numbers.c ends here */
