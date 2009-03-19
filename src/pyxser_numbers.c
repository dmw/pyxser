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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif /* !HAVE_CONFIG_H */

#include <Python.h>

#include <libxml/parser.h>
#include <libxml/tree.h>

#include "include/pyxser.h"
#include "include/pyxser_tools.h"

static const char pyxser_true_char_value[] = "True";
static const char pyxser_false_char_value[] = "False";

xmlNodePtr
pyxser_SerializeInt(PyObject *o, char *name,
					PyListObject *dupItems, xmlDocPtr doc)
{
	return pyxser_SerializePrimitiveType(o, name,
										 dupItems, doc);
}

xmlNodePtr
pyxser_SerializeExactInt(PyObject *o, char *name,
						 PyListObject *dupItems, xmlDocPtr doc)
{
	return pyxser_SerializePrimitiveType(o, name,
										 dupItems, doc);
}

xmlNodePtr
pyxser_SerializeBoolean(PyObject *o, char *name,
						PyListObject *dupItems, xmlDocPtr doc)
{
	return pyxser_SerializePrimitiveType(o, name,
										 dupItems, doc);
}

xmlNodePtr
pyxser_SerializeLong(PyObject *o, char *name,
					 PyListObject *dupItems, xmlDocPtr doc)
{
	return pyxser_SerializePrimitiveType(o, name,
										 dupItems, doc);
}

xmlNodePtr
pyxser_SerializeFloat(PyObject *o, char *name,
					  PyListObject *dupItems, xmlDocPtr doc)
{
	return pyxser_SerializePrimitiveType(o, name,
										 dupItems, doc);
}

xmlNodePtr
pyxser_SerializeExactFloat(PyObject *o, char *name,
						   PyListObject *dupItems, xmlDocPtr doc)
{
	return pyxser_SerializePrimitiveType(o, name,
										 dupItems, doc);
}

xmlNodePtr
pyxser_SerializeComplex(PyObject *o, char *name,
						PyListObject *dupItems, xmlDocPtr doc)
{
	return pyxser_SerializePrimitiveType(o, name,
										 dupItems, doc);
}

xmlNodePtr
pyxser_SerializeExactComplex(PyObject *o, char *name,
							 PyListObject *dupItems, xmlDocPtr doc)
{
	return pyxser_SerializePrimitiveType(o, name,
										 dupItems, doc);
}

PyObject *
pyxunser_SerializeInt(PythonUnserializationArgumentsPtr obj)
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
pyxunser_SerializeExactInt(PythonUnserializationArgumentsPtr obj)
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
pyxunser_SerializeBoolean(PythonUnserializationArgumentsPtr obj)
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
	}
	return res;
}

PyObject *
pyxunser_SerializeLong(PythonUnserializationArgumentsPtr obj)
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
pyxunser_SerializeFloat(PythonUnserializationArgumentsPtr obj)
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
				str = PyString_FromString((char *)ron->content);
				if (PYTHON_IS_NOT_NONE(str)) {
					res = PyFloat_FromString(str, NULL);
				}
			}
		}
	}
	return res;
}

PyObject *
pyxunser_SerializeExactFloat(PythonUnserializationArgumentsPtr obj)
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
				str = PyString_FromString((char *)ron->content);
				if (PYTHON_IS_NOT_NONE(str)) {
					res = PyFloat_FromString(str, NULL);
				}
			}
		}
	}
	return res;
}

PyObject *
pyxunser_SerializeComplex(PythonUnserializationArgumentsPtr obj)
{
	xmlNodePtr node = *(obj->currentNode);
	if (node != (xmlNodePtr)NULL) {
		return Py_None;
	}
	return Py_None;
}

PyObject *
pyxunser_SerializeExactComplex(PythonUnserializationArgumentsPtr obj)
{
	xmlNodePtr node = *(obj->currentNode);
	if (node != (xmlNodePtr)NULL) {
		return Py_None;
	}
	return Py_None;
}

/* pyserx_numbers.c ends here */
