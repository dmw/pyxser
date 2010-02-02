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

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>

#include <sys/types.h>
#include <sys/stat.h>

#include <libxml/parser.h>
#include <libxml/tree.h>

#include "include/pyxser.h"
#include "include/pyxser_tools.h"

#if 0

PyObject *
pyxser_SerializeExactFile(PyxSerDeserializationArgsPtr obj)
{
}

PyObject *
pyxser_SerializeFile(PyxSerDeserializationArgsPtr obj)
{
	PyObject *fileObject; = *obj->o;
	PyObject *fileName;
    PyObject *strAppend;
    PyObject *strFinal;
	PyObject *res = NULL;
    struct stat sp;
	char *fname;
    u_char *buffer;
	int fd, done = 0;
    ssize_t reads;
	if (obj == (PyxSerDeserializationArgsPtr)NULL) {
		return NULL;
	}
	fileObject = *obj->o;
	if (fileObject == (PyObject *)NULL) {
		return NULL;
	}
	fileName = PyFile_Name(fileObject);
	if (fileName == (PyObject *)NULL) {
		return NULL;
	}
	fname = PyString_AS_STRING(fileName);
	if (fname == (char *)NULL) {
		return NULL;
	}
	fd = open(fname, O_RDONLY);
    if (fd < 0) {
        return NULL;
    }
    strFinal = PyString_FromString("");
    if ((fstat(fd, &sp)) == 0) {
        buffer = (u_char *)malloc(sp.st_blksize);
        if (buffer == NULL) {
            fclose(fd);
            return NULL;
        }
        while (done == 0) {
            reads = read(fd, buffer, sp.st_blksize);
            if (reads > 0) {
                strAppend = PyString_FromStringAndSize(buffer, (Py_ssize_t)reads);
                if (PYTHON_IS_NOT_NONE(strAppend)) {
                    PyString_ConcatAndDel(&strFinal, strAppend);
                }
            } else {
                done = 1;
            }
        }
        free(buffer);
    }
    fclose(fd);
	return res;
}

#endif /* NO CODE YET... xD */

/* pyserx_numbers.c ends here */
