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

#ifndef PYXSER_TYPEM_H
#define PYXSER_TYPEM_H 1

#include <Python.h>
#include <datetime.h>
#include <libxml/parser.h>
#include <libxml/tree.h>

#include "include/pyxser.h"

#ifdef __cplusplus
extern "C" {
#endif /* !__cplusplus */

    xmlNodePtr pyxser_TypeMapSearchAndGetNode(PyObject *tmap, PyObject *tval,
                                              PyObject *targ, xmlDocPtr doc);

    xmlNodePtr pyxser_TypeMapSearchAndGet(PyxSerializationArgsPtr args,
                                          xmlNodePtr added);

    PyObject *pyxunser_TypeMapSearchAndGet(PyObject *tmap, PyObject *tval,
                                           xmlNodePtr node);

#ifdef __cplusplus
};
#endif /* !__cplusplus */

#endif /* PYXSER_TYPEM_H */
/* pyserx_typem.h ends here */

