/* -*- mode: c; indent-tabs-mode: nil; tab-width: 4; c-file-style: "python" -*- */
/* vim:set ft=c ff=unix ts=4 sw=4 enc=latin1 expandtab: */
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
static const char pyxser_complex_format[] = "%lf:%lf";

xmlNodePtr
pyxser_SerializeInt(PyxSerializationArgsPtr args)
{
	return pyxser_SerializePrimitiveType(args);
}

xmlNodePtr
pyxser_SerializeExactInt(PyxSerializationArgsPtr args)
{
	return pyxser_SerializePrimitiveType(args);
}

xmlNodePtr
pyxser_SerializeBoolean(PyxSerializationArgsPtr args)
{
	return pyxser_SerializePrimitiveType(args);
}

xmlNodePtr
pyxser_SerializeLong(PyxSerializationArgsPtr args)
{
	return pyxser_SerializePrimitiveType(args);
}

xmlNodePtr
pyxser_SerializeFloat(PyxSerializationArgsPtr args)
{
	return pyxser_SerializePrimitiveType(args);
}

xmlNodePtr
pyxser_SerializeExactFloat(PyxSerializationArgsPtr args)
{
	return pyxser_SerializePrimitiveType(args);
}

xmlNodePtr
pyxser_SerializeComplex(PyxSerializationArgsPtr args)
{
    PyObject *o = *args->o;
    char *name = args->name;
    xmlDocPtr doc = *args->docptr;

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
    sprintf(sptr, pyxser_complex_format, cx.real, cx.imag);
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
	return newElementNode;
}

xmlNodePtr
pyxser_SerializeExactComplex(PyxSerializationArgsPtr args)
{
	return pyxser_SerializeComplex(args);
}

PyObject *
pyxunser_SerializeInt(PyxSerDeserializationArgsPtr obj)
{
	xmlNodePtr node = *(obj->currentNode);
	xmlNodePtr ron;
	PyObject *res = Py_None;
	if (node == (xmlNodePtr)NULL) {
        return res;
    }
    for (ron = node->children;
         ron;
         ron = ron->next) {
        if (ron->type == XML_TEXT_NODE) {
            res = PyInt_FromString((char *)ron->content, NULL, 10);
            break;
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
	if (node == (xmlNodePtr)NULL) {
        return res;
    }
    for (ron = node->children;
         ron;
         ron = ron->next) {
        if (ron->type == XML_TEXT_NODE
            && ron->content != BAD_CAST NULL) {
            res = PyInt_FromString((char *)ron->content, NULL, 10);
            break;
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
	if (node == (xmlNodePtr)NULL) {
        return res;
    }
    for (ron = node->children;
         ron;
         ron = ron->next) {
        if (ron->type == XML_TEXT_NODE) {
            if (strncmp((char *)ron->content,
                        pyxser_true_char_value,
                        strlen(pyxser_true_char_value)) == 0) {
                Py_INCREF(Py_True);
                return Py_True;
            } else if (strncmp((char *)ron->content,
                               pyxser_false_char_value,
                               strlen(pyxser_false_char_value)) == 0) {
                Py_INCREF(Py_False);
                return Py_False;
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
	if (node == (xmlNodePtr)NULL) {
        return res;
    }
    for (ron = node->children;
         ron;
         ron = ron->next) {
        if (ron->type == XML_TEXT_NODE) {
            res = PyInt_FromString((char *)ron->content, NULL, 10);
            break;
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
	if (node == (xmlNodePtr)NULL) {
        return res;
    }
    for (ron = node->children;
         ron;
         ron = ron->next) {
        if (ron->type == XML_TEXT_NODE
            && ron->content != BAD_CAST NULL) {
            str = PyString_FromString((char *)ron->content);
            if (PYTHON_IS_NOT_NONE(str)) {
                res = PyFloat_FromString(str, NULL);
                Py_DECREF(str);
                break;
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
	if (node == (xmlNodePtr)NULL) {
        return res;
    }
    for (ron = node->children;
         ron;
         ron = ron->next) {
        if (ron->type == XML_TEXT_NODE
            && ron->content != BAD_CAST NULL) {
            str = PyString_FromString((char *)ron->content);
            if (PYTHON_IS_NOT_NONE(str)) {
                res = PyFloat_FromString(str, NULL);
                Py_DECREF(str);
                break;
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
	if (node == (xmlNodePtr)NULL) {
        return res;
    }
    for (ron = node->children;
         ron;
         ron = ron->next) {
        if (ron->type == XML_TEXT_NODE
            && ron->content != BAD_CAST NULL) {
            sscanf((char *)ron->content, pyxser_complex_format,
                   &cx.real, &cx.imag);
            cxo = PyComplex_FromCComplex(cx);
            if (PYTHON_IS_NOT_NONE(cxo)) {
                res = cxo;
                break;
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
