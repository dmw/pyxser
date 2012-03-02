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
    extern const char pyxser_xsd_c14n_body[];
    extern const char pyxser_xsd_body[];
    extern xmlDtdPtr pyxser_dtd_object;
    extern xmlDtdPtr pyxser_dtd_c14n_object;
    extern xmlDocPtr pyxser_xsd_doc;
    extern xmlDocPtr pyxser_xsd_c14n_doc;

    char *pyxser_GetClassName(PyObject *obj);

    PyObject *pyxser_SearchModuleType(PyObject *mod, const char *name);

    PyObject *pyxser_GetNonCallableAttributes(PyObject *obj);

    int pyxser_PyListContains(PyListObject *lst, PyObject *o);

    xmlNodePtr pyxser_AddReference(PyObject *o, xmlNodePtr currentNode);

    void pyxser_AddIdentifier(PyObject *o, xmlNodePtr currentNode);

    int pyxser_CheckBuiltInModule(PyObject *o);

    void pyxser_AddModuleAttr(PyObject *o, xmlNodePtr currentNode);

    xmlNsPtr pyxser_GetDefaultNs(void);

    xmlNsPtr pyxser_GetXsiNs(void);

    xmlNsPtr pyxser_GetXsdNs(void);

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

    xmlDtdPtr pyxser_GetPyxserDTDC14N(void);

    xmlSchemaPtr pyxser_GetPyxserXSD(void);

    xmlSchemaPtr pyxser_GetPyxserXSDC14N(void);

    int pyxser_ValidateDocument(xmlDocPtr doc);

    int pyxser_ValidateDocumentC14N(xmlDocPtr doc);

    int pyxser_ValidateDocumentXSD(xmlDocPtr doc);

    int pyxser_ValidateDocumentXSDC14N(xmlDocPtr doc);

    int pyxser_C14NRenderAllNodes(void * user_data,
                                  xmlNodePtr node,
                                  xmlNodePtr parent);

    PyObject *pyxser_GetStringFromUnicode(PyObject *unic);

    xmlNodePtr pyxser_SerializeWeakref(PyxSerializationArgsPtr args);

/* numbers */
    int pyxserInt_Check(PyObject *o);
    int pyxserInt_CheckExact(PyObject *o);
    int pyxserBool_Check(PyObject *o);
    int pyxserLong_CheckExact(PyObject *o);
    int pyxserFloat_Check(PyObject *o);
    int pyxserFloat_CheckExact(PyObject *o);
    int pyxserComplex_Check(PyObject *o);
    int pyxserComplex_CheckExact(PyObject *o);

/* strings */
    int pyxserString_Check(PyObject *o);
    int pyxserString_CheckExact(PyObject *o);
    int pyxserUnicode_Check(PyObject *o);
    int pyxserUnicode_CheckExact(PyObject *o);
#if PY_MAJOR_VERSION < 3
    int pyxserBuffer_Check(PyObject *o);
#endif /* PY_MAJOR_VERSION < 3 */

/* tuples */
    int pyxserTuple_Check(PyObject *o);
    int pyxserTuple_CheckExact(PyObject *o);

/* lists */
    int pyxserList_Check(PyObject *o);
    int pyxserList_CheckExact(PyObject *o);

/* dictionaries */
    int pyxserDict_Check(PyObject *o);
    int pyxserDict_CheckExact(PyObject *o);

/* files */
    int pyxserFile_Check(PyObject *o);
    int pyxserFile_CheckExact(PyObject *o);

/* weakref */
    int pyxserWeakref_Check(PyObject *o);

/* sequences */
    int pyxserSequence_Check(PyObject *o);

/* instrospection */
    int pyxserInstance_Check(PyObject *o);
    int pyxserFunction_Check(PyObject *o);
    int pyxserMethod_Check(PyObject *o);
    int pyxserModule_Check(PyObject *o);
    int pyxserModule_CheckExact(PyObject *o);
    int pyxserSeqIter_Check(PyObject *o);
    int pyxserSlice_Check(PyObject *o);
    int pyxserCell_Check(PyObject *o);
    int pyxserGen_Check(PyObject *o);
    int pyxserDate_CheckExact(PyObject *o);
    int pyxserAnySet_Check(PyObject *o);
    int pyxserAnySet_CheckExact(PyObject *o);
    int pyxserFrozenSet_CheckExact(PyObject *o);

/* numbers */
    int pyxunserInt_Check(xmlNodePtr node);
    int pyxunserInt_CheckExact(xmlNodePtr node);
    int pyxunserBool_Check(xmlNodePtr node);
    int pyxunserLong_CheckExact(xmlNodePtr node);
    int pyxunserFloat_Check(xmlNodePtr node);
    int pyxunserFloat_CheckExact(xmlNodePtr node);
    int pyxunserComplex_Check(xmlNodePtr node);
    int pyxunserComplex_CheckExact(xmlNodePtr node);

/* strings */
    int pyxunserString_Check(xmlNodePtr node);
    int pyxunserString_CheckExact(xmlNodePtr node);
    int pyxunserUnicode_Check(xmlNodePtr node);
    int pyxunserUnicodeExact_CheckExact(xmlNodePtr node);
#if PY_MAJOR_VERSION < 3
    int pyxunserBuffer_Check(xmlNodePtr node);
#endif /* PY_MAJOR_VERSION < 3 */

/* tuples */
    int pyxunserTuple_Check(xmlNodePtr node);
    int pyxunserTuple_CheckExact(xmlNodePtr node);

/* lists */
    int pyxunserList_Check(xmlNodePtr node);
    int pyxunserList_CheckExact(xmlNodePtr node);

/* dictionaries */
    int pyxunserDict_Check(xmlNodePtr node);
    int pyxunserDict_CheckExact(xmlNodePtr node);

/* files */
    int pyxunserFile_Check(xmlNodePtr node);
    int pyxunserFile_CheckExact(xmlNodePtr node);

/* sequences */
    int pyxunserSequence_Check(xmlNodePtr node);

/* instrospection */
    int pyxunserInstance_Check(xmlNodePtr node);
    int pyxunserFunction_Check(xmlNodePtr node);
    int pyxunserMethod_Check(xmlNodePtr node);
    int pyxunserModule_Check(xmlNodePtr node);
    int pyxunserModule_CheckExact(xmlNodePtr node);
    int pyxunserSeqIter_Check(xmlNodePtr node);
    int pyxunserSlice_Check(xmlNodePtr node);
    int pyxunserCell_Check(xmlNodePtr node);
    int pyxunserGen_Check(xmlNodePtr node);
    int pyxunserDate_CheckExact(xmlNodePtr node);
    int pyxunserAnySet_Check(xmlNodePtr node);
    int pyxunserAnySet_CheckExact(xmlNodePtr node);
    int pyxunserFrozenSet_CheckExact(xmlNodePtr node);

    PyObject *pyxser_PyInstance_NewRaw(PyObject *class);

#ifdef __cplusplus
};
#endif /* !__cplusplus */

#endif /* PYXSER_TOOLS_H */
/* pyserx_tools.h ends here */


