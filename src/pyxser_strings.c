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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif /* !HAVE_CONFIG_H */

#include <string.h>

#include <Python.h>

#include <libxml/parser.h>
#include <libxml/tree.h>
#include "include/pyxser_tools.h"


xmlNodePtr
pyxser_SerializeUnicode(PyObject *o, char *name,
						PyListObject *dupItems, xmlDocPtr doc)
{
	xmlNodePtr newElementNode = (xmlNodePtr)NULL;
#if defined(LIBXML_UNICODE_ENABLED)
	xmlNodePtr newTextNode = (xmlNodePtr)NULL;
	xmlAttrPtr typeAttr = (xmlAttrPtr)NULL;
	xmlAttrPtr nameAttr = (xmlAttrPtr)NULL;
	PyObject *classPtr = (PyObject *)NULL;
	PyObject *className = (PyObject *)NULL;
	PyObject *unic = (PyObject *)NULL;
	size_t data_size = 0;
	char *sptr = (char *)NULL;
	char *nptr = (char *)NULL;
	char sz_attr[30];
	if (PYTHON_IS_NOT_NONE(o)) {
		classPtr = PyObject_GetAttrString(o, pyxser_attr_class);
		if (PYTHON_IS_NOT_NONE(classPtr)) {
			className = PyObject_GetAttrString(classPtr,
											   pyxser_attr_name);
			if (PYTHON_IS_NOT_NONE(className)) {
				data_size = PyUnicode_GET_SIZE(o);
				unic = PyUnicode_AsUnicodeEscapeString(o);
				if (PYTHON_IS_NOT_NONE(unic)) {
					nptr = PyString_AS_STRING(className);
					sptr = PyString_AS_STRING(unic);
					if (sptr != (char *)NULL
						&& nptr != (char *)NULL) {
						newElementNode = xmlNewDocNode(doc,
													   pyxser_GetDefaultNs(),
													   BAD_CAST pyxser_xml_attr_item,
													   NULL);
						newTextNode = xmlNewDocText(doc, BAD_CAST sptr);
						typeAttr = xmlNewProp(newElementNode,
											  BAD_CAST pyxser_xml_attr_type,
											  BAD_CAST nptr);
						nameAttr = xmlNewProp(newElementNode,
											  BAD_CAST pyxser_xml_attr_name,
											  BAD_CAST name);
						memset(sz_attr, 0, 30);
						sprintf(sz_attr, "%ld", (long)data_size);
						typeAttr = xmlNewProp(newElementNode,
											  BAD_CAST pyxser_xml_attr_size,
											  BAD_CAST sz_attr);
						xmlAddChild(newElementNode, newTextNode);
					}
					Py_DECREF(unic);
				}
			}
			Py_DECREF(className);
		}
		Py_DECREF(classPtr);
	}
#else
#error We need unicode enabled libxml2
#endif /* LIBXML_UNICODE_ENABLED */
	return newElementNode;
}

xmlNodePtr
pyxser_SerializeExactUnicode(PyObject *o, char *name,
							 PyListObject *dupItems, xmlDocPtr doc)
{
	return pyxser_SerializeUnicode(o, name, (PyListObject *)NULL, doc);
}

xmlNodePtr
pyxser_SerializeString(PyObject *o, char *name,
					   PyListObject *dupItems, xmlDocPtr doc)
{
	xmlNodePtr newElementNode = (xmlNodePtr)NULL;
	xmlNodePtr newTextNode = (xmlNodePtr)NULL;
	xmlAttrPtr typeAttr = (xmlAttrPtr)NULL;
	xmlAttrPtr nameAttr = (xmlAttrPtr)NULL;
	PyObject *classPtr = (PyObject *)NULL;
	PyObject *className = (PyObject *)NULL;
	char *sptr = (char *)NULL;
	char *nptr = (char *)NULL;
	if (PYTHON_IS_NOT_NONE(o)) {
		sptr = PyString_AsString(o);
		classPtr = PyObject_GetAttrString(o, pyxser_attr_class);
		if (PYTHON_IS_NOT_NONE(classPtr)
			&& sptr != (char *)NULL) {
			className = PyObject_GetAttrString(classPtr,
											   pyxser_attr_name);
			if (PYTHON_IS_NOT_NONE(className)) {
				nptr = PyString_AS_STRING(className);
				if (sptr != (char *)NULL
					&& nptr != (char *)NULL) {
					newElementNode = xmlNewDocNode(doc,
												   pyxser_GetDefaultNs(),
												   BAD_CAST pyxser_xml_attr_item,
												   NULL);
					newTextNode = xmlNewDocText(doc, BAD_CAST sptr);
					typeAttr = xmlNewProp(newElementNode,
										  BAD_CAST pyxser_xml_attr_type,
										  BAD_CAST nptr);
					nameAttr = xmlNewProp(newElementNode,
										  BAD_CAST pyxser_xml_attr_name,
										  BAD_CAST name);
					xmlAddChild(newElementNode, newTextNode);
				}
				Py_DECREF(className);
			}
			Py_DECREF(classPtr);
		}
	}
	return newElementNode;
}

xmlNodePtr
pyxser_SerializeExactString(PyObject *o, char *name, PyListObject *dupItems, xmlDocPtr doc)
{
	return pyxser_SerializeString(o, name, (PyListObject *)NULL, doc);
}

xmlNodePtr
pyxser_SerializeBuffer(PyObject *o, char *name, PyListObject *dupItems, xmlDocPtr doc)
{
	return pyxser_SerializePrimitiveType(o, name, (PyListObject *)NULL, doc);
}


PyObject *
pyxunser_SerializeString(PythonUnserializationArgumentsPtr obj)
{
	xmlNodePtr node = *(obj->currentNode);
	xmlNodePtr runOverNode;
	PyObject *str = Py_None;
	PyObject *res = Py_None;
	if (node != (xmlNodePtr)NULL) {
		for (runOverNode = node->children;
			 runOverNode;
			 runOverNode = runOverNode->next) {
			if (runOverNode->type == XML_TEXT_NODE
				&& runOverNode->content != BAD_CAST NULL) {
				str = PyString_FromString((char *)runOverNode->content);
				res = str;
			}
		}
	}
	return res;
}

PyObject *
pyxunser_SerializeExactString(PythonUnserializationArgumentsPtr obj)
{
	return pyxunser_SerializeString(obj);
}

#define PYXSER_MAX_LENGTH				8192
PyObject *
pyxunser_SerializeUnicode(PythonUnserializationArgumentsPtr obj)
{
	xmlNodePtr node = *(obj->currentNode);
	xmlNodePtr runOverNode;
	xmlChar *propSize = (xmlChar *)NULL;
	PyObject *unic = Py_None;
	PyObject *res = Py_None;
	Py_ssize_t tsz = 0;
	if (node != (xmlNodePtr)NULL) {
		propSize = xmlGetProp(node, BAD_CAST pyxser_xml_attr_size);
		if (propSize != (xmlChar *)NULL) {
			for (runOverNode = node->children;
				 runOverNode;
				 runOverNode = runOverNode->next) {
				tsz = (Py_ssize_t)strtol((const char *)propSize, (char **)NULL, 10);
				if (runOverNode->type == XML_TEXT_NODE) {
					unic = PyUnicode_DecodeUnicodeEscape((const char *)runOverNode->content,
														 (strlen((const char *)runOverNode->content)),
														 "strict");
					res = unic;
				}
			}
		}
	}
	return res;
}

PyObject *
pyxunser_SerializeExactUnicode(PythonUnserializationArgumentsPtr obj)
{
	return pyxunser_SerializeUnicode(obj);
}

PyObject *
pyxunser_SerializeBuffer(PythonUnserializationArgumentsPtr obj)
{
	if (obj != NULL) {
		// XXX: must change to base 64 encoded buffer.
		return Py_None;
	}
	return Py_None;
}

/* pyserx_strings.c ends here */
