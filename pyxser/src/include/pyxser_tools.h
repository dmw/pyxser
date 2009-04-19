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

#ifndef PYXSER_TOOLS_H
#define PYXSER_TOOLS_H 1

#include <Python.h>
#include <datetime.h>
#include <libxml/parser.h>
#include <libxml/tree.h>

#include "include/pyxser.h"

#ifdef __cplusplus
extern "C" {
#endif /* !__cplusplus */


    extern const PythonTypeSerialize serxConcreteTypes[];
    extern const PythonTypeDeserialize unserxConcreteTypes[];
    extern xmlDtdPtr pyxser_dtd_object;
    extern xmlDtdPtr pyxser_dtd_c14n_object;

    char *pyxser_GetClassName(PyObject *obj);

    xmlNodePtr pyxser_SerializePrimitiveType(PyObject *o, char *name,
                                             PyListObject *dupItems,
                                             xmlDocPtr doc,
                                             int *depth, int *depthcnt);

    xmlNodePtr pyxser_SerializeXml(PyObject *o, xmlDocPtr *docptr, xmlNodePtr *rootNode,
                                   xmlNodePtr *currentNode, PyListObject *dupSrcItems,
                                   const char *enc, int *depth, int *depthcnt);

    xmlNodePtr pyxser_SerializeXmlNonStd(PyObject *o, xmlDocPtr *docPtr,
                                         xmlNodePtr *rootNode, xmlNodePtr *currentNode,
                                         PyListObject *dupSrcItems);

    xmlNodePtr pyxser_RunSerialization(PyObject *item, PyObject *o, PyObject *currentKey,
                                       PyListObject *dupItems, xmlDocPtr *docptr,
                                       xmlNodePtr *rootNode, xmlNodePtr *currentNode,
                                       int *depth, int *depthcnt);

    PyObject *pyxser_SearchTypesInModules(const char *n_module,
                                          const char *n_type,
                                          PyDictObject **modules);

    PyObject *pyxser_UnserializeXml(PyxSerDeserializationArgsPtr obj);

    PyObject *pyxser_SearchModuleType(PyObject *mod, const char *name);

    PyObject *pyxser_GetNonCallableAttributes(PyObject *obj);

    int pyxser_PyListContains(PyListObject *lst, PyObject *o);

    xmlNodePtr pyxser_AddReference(PyObject *o, xmlNodePtr currentNode);

    void pyxser_AddIdentifier(PyObject *o, xmlNodePtr currentNode);

    void pyxser_AddModuleAttr(PyObject *o, xmlNodePtr currentNode);

    xmlNsPtr pyxser_GetDefaultNs(void);

    char *pyxser_ExtractPropertyName(const char *name, xmlNodePtr node);

    PyObject *pyxser_UnserializeBlock(PyxSerDeserializationArgsPtr obj);

    int pyxser_ModuleNotMain(const char *mod);

    PyObject *pyxser_SearchModule(const char *name);

    PyObject *pyxser_CacheModule(PyObject *d, const char *name);

    PyObject *pyxser_SearchObjectInMain(const char *name);

    char *pyxser_GetObjectIdentifier(xmlNodePtr node);

    PyObject *pyxser_CheckAvailableObject(PyObject *dict, char *id);

    void pyxser_AddAvailableObject(PyObject *dict, char *id, PyObject *o);

    xmlDtdPtr pyxser_GetPyxserDTD(void);

    int pyxser_ValidateDocument(xmlDocPtr doc);

    int pyxser_ValidateDocumentC14N(xmlDocPtr doc);

    int pyxser_C14NRenderAllNodes(void * user_data,
                                  xmlNodePtr node,
                                  xmlNodePtr parent);

#ifdef __cplusplus
};
#endif /* !__cplusplus */

#endif /* PYXSER_H */
/* pyserx.h ends here */

