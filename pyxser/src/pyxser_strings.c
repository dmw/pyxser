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

#include <string.h>

#include <Python.h>

#include <libxml/parser.h>
#include <libxml/tree.h>
#include "include/pyxser_tools.h"


static xmlChar *pyxser_ConvertInput(const char *in, const char *encoding);


xmlNodePtr
pyxser_SerializeUnicode(PyxSerializationArgsPtr args)
{

    PyObject *o = *args->o;
    char *name = args->name;
    char *enc = args->enc;
    xmlDocPtr doc = *args->docptr;

	xmlNodePtr nen = (xmlNodePtr)NULL;
	xmlNodePtr ntxt = (xmlNodePtr)NULL;
	xmlAttrPtr typeAttr = (xmlAttrPtr)NULL;
	xmlAttrPtr nameAttr = (xmlAttrPtr)NULL;

	PyObject *classPtr = (PyObject *)NULL;
	PyObject *className = (PyObject *)NULL;
	PyObject *unic = (PyObject *)NULL;

	size_t data_size = 0;
	char *sptr = (char *)NULL;
	char *nptr = (char *)NULL;
	char sz_attr[30];

	if (PYTHON_IS_NONE(o)) {
        return (xmlNodePtr)NULL;
    }

    classPtr = PyObject_GetAttrString(o, pyxser_attr_class);

    if (PYTHON_IS_NONE(classPtr)) {
        PyErr_Clear();
        return (xmlNodePtr)NULL;
    }

    className = PyObject_GetAttrString(classPtr,
                                       pyxser_attr_name);

    if (PYTHON_IS_NONE(className)) {
        PyErr_Clear();
        PYXSER_FREE_OBJECT(classPtr);
        return (xmlNodePtr)NULL;
    }

    unic = PyUnicode_Encode(PyUnicode_AS_UNICODE(o),
                            PyUnicode_GET_DATA_SIZE(o), enc,
                            pyxser_xml_encoding_mode);
    data_size = PyString_Size(unic);

    if (PYTHON_IS_NONE(unic)) {
        PYXSER_FREE_OBJECT(className);
        PYXSER_FREE_OBJECT(classPtr);
        return (xmlNodePtr)NULL;
    }

    nptr = PyString_AS_STRING(className);
    sptr = PyString_AS_STRING(unic);
    if (sptr != (char *)NULL
        && nptr != (char *)NULL) {
        nen = xmlNewDocNode(doc,
                            pyxser_GetDefaultNs(),
                            BAD_CAST pyxser_xml_attr_item,
                            NULL);
        ntxt = xmlNewDocText(doc, BAD_CAST sptr);
        typeAttr = xmlNewProp(nen,
                              BAD_CAST pyxser_xml_attr_type,
                              BAD_CAST nptr);
        if (name != (char *)NULL) {
            nameAttr = xmlNewProp(nen,
                                  BAD_CAST pyxser_xml_attr_name,
                                  BAD_CAST name);
        }
        memset(sz_attr, 0, 30);
        sprintf(sz_attr, "%ld", (long)data_size);
        typeAttr = xmlNewProp(nen,
                              BAD_CAST pyxser_xml_attr_size,
                              BAD_CAST sz_attr);
        xmlAddChild(nen, ntxt);
    }
    PYXSER_FREE_OBJECT(unic);
    PYXSER_FREE_OBJECT(className);
    PYXSER_FREE_OBJECT(classPtr);
	return nen;
}

xmlNodePtr
pyxser_SerializeExactUnicode(PyxSerializationArgsPtr args)
{
	return pyxser_SerializeUnicode(args);
}

xmlNodePtr
pyxser_SerializeString(PyxSerializationArgsPtr args)
{

    PyObject *o = *args->o;
    char *name = args->name;
    xmlDocPtr doc = *args->docptr;

	xmlNodePtr nen = (xmlNodePtr)NULL;
	xmlNodePtr ntxt = (xmlNodePtr)NULL;
	xmlAttrPtr typeAttr = (xmlAttrPtr)NULL;
	xmlAttrPtr nameAttr = (xmlAttrPtr)NULL;

	PyObject *classPtr = (PyObject *)NULL;
	PyObject *className = (PyObject *)NULL;

	char *sptr = (char *)NULL;
	char *nptr = (char *)NULL;

	if (PYTHON_IS_NONE(o)) {
        return (xmlNodePtr)NULL;
    }

    sptr = PyString_AsString(o);
    classPtr = PyObject_GetAttrString(o, pyxser_attr_class);

    if (PYTHON_IS_NONE(classPtr) || sptr == (char *)NULL) {
        PyErr_Clear();
        return (xmlNodePtr)NULL;
    }

    className = PyObject_GetAttrString(classPtr,
                                       pyxser_attr_name);

    if (PYTHON_IS_NONE(className)) {
        PyErr_Clear();
        PYXSER_FREE_OBJECT(classPtr);
        return (xmlNodePtr)NULL;
    }

    nptr = PyString_AS_STRING(className);
    if (sptr != (char *)NULL
        && nptr != (char *)NULL) {
        nen = xmlNewDocNode(doc,
                            pyxser_GetDefaultNs(),
                            BAD_CAST pyxser_xml_attr_item,
                            NULL);
        ntxt = xmlNewDocText(doc, BAD_CAST sptr);
        typeAttr = xmlNewProp(nen,
                              BAD_CAST pyxser_xml_attr_type,
                              BAD_CAST nptr);
        if (name != (char *)NULL) {
            nameAttr = xmlNewProp(nen,
                                  BAD_CAST pyxser_xml_attr_name,
                                  BAD_CAST name);
        }
        xmlAddChild(nen, ntxt);
    }
    PYXSER_FREE_OBJECT(className);
    PYXSER_FREE_OBJECT(classPtr);
	return nen;
}

xmlNodePtr
pyxser_SerializeExactString(PyxSerializationArgsPtr args)
{
	return pyxser_SerializeString(args);
}

xmlNodePtr
pyxser_SerializeBuffer(PyxSerializationArgsPtr args)
{
	return pyxser_SerializePrimitiveType(args);
}


PyObject *
pyxunser_SerializeString(PyxSerDeserializationArgsPtr obj)
{
	xmlNodePtr node = *(obj->currentNode);
	xmlNodePtr ron;
	PyObject *str = Py_None;
	PyObject *res = Py_None;
	if (node != (xmlNodePtr)NULL) {
		for (ron = node->children;
			 ron;
			 ron = ron->next) {
			if (ron->type == XML_TEXT_NODE
				&& ron->content != BAD_CAST NULL) {
				str = PyString_FromString((char *)ron->content);
				res = str;
			}
		}
	}
	return res;
}

PyObject *
pyxunser_SerializeExactString(PyxSerDeserializationArgsPtr obj)
{
	return pyxunser_SerializeString(obj);
}

#define PYXSER_MAX_LENGTH				8192
PyObject *
pyxunser_SerializeUnicode(PyxSerDeserializationArgsPtr obj)
{
	xmlNodePtr node = *(obj->currentNode);
	xmlNodePtr ron;
	xmlChar *propSize = (xmlChar *)NULL;
	PyObject *unic = NULL;
	PyObject *res = NULL;
	Py_ssize_t tsz = 0;
    char *enc = obj->encoding;

    if (node == (xmlNodePtr)NULL) {
        return res;
    }

    propSize = xmlGetProp(node, BAD_CAST pyxser_xml_attr_size);
    if (propSize != (xmlChar *)NULL) {
        for (ron = node->children;
             ron;
             ron = ron->next) {
            tsz = (Py_ssize_t)strtol((const char *)propSize,
                                     (char **)NULL, 10);
            if (ron->type == XML_TEXT_NODE) {
                unic = PyUnicode_Decode((char *)ron->content,
                                        tsz,
                                        enc,
                                        pyxser_xml_encoding_mode);
                res = unic;
                break;
            }
        }
        PYXSER_XMLFREE(propSize);
    }
	return res;
}

PyObject *
pyxunser_SerializeExactUnicode(PyxSerDeserializationArgsPtr obj)
{
	return pyxunser_SerializeUnicode(obj);
}

PyObject *
pyxunser_SerializeBuffer(PyxSerDeserializationArgsPtr obj)
{
    return Py_None;
}

static xmlChar *
pyxser_ConvertInput(const char *in, const char *encoding)
{
    xmlChar *out;
    int ret;
    int size;
    int out_size;
    int temp;
    xmlCharEncodingHandlerPtr handler;

    if (in == 0)
        return 0;

    handler = xmlFindCharEncodingHandler(encoding);

    if (!handler) {
        return 0;
    }

    size = (int) strlen(in) + 1;
    out_size = size * 2 - 1;
    out = (unsigned char *) xmlMalloc((size_t) out_size);

    if (out != 0) {
        temp = size - 1;
        ret = handler->input(out, &out_size, (const xmlChar *) in, &temp);
        if ((ret < 0) || (temp - size + 1)) {
            PYXSER_XMLFREE(out);
            out = 0;
        } else {
            out = (unsigned char *) xmlRealloc(out, out_size + 1);
            out[out_size] = 0;  /*null terminating out */
        }
    }

    return out;
}


/* pyserx_strings.c ends here */
