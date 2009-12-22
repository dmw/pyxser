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

#include <Python.h>
#include <datetime.h>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <libxml/c14n.h>

#include "include/pyxser_tools.h"
#include "include/pyxser.h"

PyObject *
pyxser_TypeMapSearchAndGet(PyObject *tmap, PyObject *tval,
                           xmlNodePtr node)
{
    xmlNodePtr ron;
    PyObject *stype = (PyObject *)NULL;
    PyObject *cb = (PyObject *)NULL;
    PyObject *args = (PyObject *)NULL;
    PyObject *res = (PyObject *)NULL;
    if (PYTHON_IS_NONE(tmap)) {
        return (PyObject *)NULL;
    }
    if (PYTHON_IS_NONE(tval)) {
        return (PyObject *)NULL;
    }
    char *tmapc = PyString_AS_STRING(tval);
    cb = PyDict_GetItem(tmap, tval);
    PyErr_Clear();
    if (PYTHON_IS_NONE(cb)) {
        return (PyObject *)NULL;
    }
	if (node != (xmlNodePtr)NULL) {
		for (ron = node->children;
			 ron;
			 ron = ron->next) {
			if (ron->type == XML_TEXT_NODE
				&& ron->content != BAD_CAST NULL) {
                stype = PyString_FromString((char *)ron->content);
                PyErr_Clear();
                if (PYTHON_IS_NONE(stype)) {
                    return (PyObject *)NULL;
                }
                args = Py_BuildValue("(O)", stype);
                res = PyObject_CallObject(cb, args);
                Py_XDECREF(args);
                Py_XDECREF(stype);
                PyErr_Clear();
            }
        }
    }
    return res;
}

/* pyserx_typem.h ends here */
