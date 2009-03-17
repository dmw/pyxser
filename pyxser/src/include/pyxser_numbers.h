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

#ifndef PYXSER_NUMBERS_H
#define PYXSER_NUMBERS_H 1

#include <Python.h>

#include "include/pyxser.h"

#ifdef __cplusplus
extern "C" {
#endif /* !__cplusplus */

xmlNodePtr pyxser_SerializeInt(PyObject *o, char *name,
							   PyListObject *dupItems, xmlDocPtr doc);

xmlNodePtr pyxser_SerializeExactInt(PyObject *o, char *name,
									PyListObject *dupItems, xmlDocPtr doc);

xmlNodePtr pyxser_SerializeBoolean(PyObject *o, char *name,
								   PyListObject *dupItems, xmlDocPtr doc);

xmlNodePtr pyxser_SerializeLong(PyObject *o, char *name,
								PyListObject *dupItems, xmlDocPtr doc);

xmlNodePtr pyxser_SerializeFloat(PyObject *o, char *name,
								 PyListObject *dupItems, xmlDocPtr doc);

xmlNodePtr pyxser_SerializeExactFloat(PyObject *o, char *name,
									  PyListObject *dupItems, xmlDocPtr doc);

xmlNodePtr pyxser_SerializeComplex(PyObject *o, char *name,
								   PyListObject *dupItems, xmlDocPtr doc);

xmlNodePtr pyxser_SerializeExactComplex(PyObject *o, char *name,
										PyListObject *dupItems, xmlDocPtr doc);

PyObject *pyxunser_SerializeInt(PythonUnserializationArgumentsPtr obj);
PyObject *pyxunser_SerializeExactInt(PythonUnserializationArgumentsPtr obj);
PyObject *pyxunser_SerializeBoolean(PythonUnserializationArgumentsPtr obj);
PyObject *pyxunser_SerializeLong(PythonUnserializationArgumentsPtr obj);
PyObject *pyxunser_SerializeFloat(PythonUnserializationArgumentsPtr obj);
PyObject *pyxunser_SerializeExactFloat(PythonUnserializationArgumentsPtr obj);
PyObject *pyxunser_SerializeComplex(PythonUnserializationArgumentsPtr obj);
PyObject *pyxunser_SerializeExactComplex(PythonUnserializationArgumentsPtr obj);

#ifdef __cplusplus
};
#endif /* !__cplusplus */

#endif /* PYXSER_NUMBERS_H */
/* pyserx_numbers.h ends here */
