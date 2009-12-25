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

#include "include/pyxser_tools.h"
#include "include/pyxser.h"

static const char type_main[] = "__main__";

inline void
pyxser_SetupXmlRootElement(xmlNodePtr *rootNode, const char *objnam)
{
	xmlAttrPtr pyxserNsXml = (xmlAttrPtr)NULL;
	xmlAttrPtr pyxserVersion = (xmlAttrPtr)NULL;
	xmlAttrPtr pyxserType = (xmlAttrPtr)NULL;

    pyxserNsXml = xmlNewProp(*rootNode,
                             BAD_CAST pyxser_xml_attr_xml_ns,
                             BAD_CAST pyxser_xml_attr_ns);

    pyxserVersion = xmlNewProp(*rootNode,
                               BAD_CAST pyxser_xml_attr_version,
                               BAD_CAST pyxser_xml_version);

    pyxserType = xmlNewProp(*rootNode,
                            BAD_CAST pyxser_xml_attr_type,
                            BAD_CAST objnam);
}


void
pyxser_CreateDocument(xmlDocPtr doc, xmlDocPtr *docptr,
                      xmlNodePtr *rootNode,
                      xmlNodePtr *xmlRootNode,
                      const char *enc,
                      PyObject *o,
                      char *objnam,
                      xmlNodePtr *currentNode)
{
    doc = xmlNewDoc(BAD_CAST pyxser_xml_version);
    doc->encoding = BAD_CAST xmlStrdup((BAD_CAST enc));
    doc->charset = xmlParseCharEncoding(enc);
    *docptr = doc;
    *rootNode = xmlNewDocNode(doc, pyxser_GetDefaultNs(),
                              BAD_CAST pyxser_xml_element_object,
                              NULL);
    pyxser_SetupXmlRootElement(rootNode, objnam);
    pyxser_AddModuleAttr(o, *rootNode);
    currentNode = rootNode;
    xmlRootNode = rootNode;
    xmlDocSetRootElement(doc, *xmlRootNode);
}


xmlNodePtr
pyxser_SerializeXml(PyxSerializationArgsPtr args)
{
    PyObject *o = *args->o;
    PyObject **oold;
    xmlDocPtr *docptr = args->docptr;
    xmlDocPtr *docptrold = args->docptr;
    xmlNodePtr *rootNode = args->rootNode;
    xmlNodePtr *rootNodeOld = args->rootNode;
    xmlNodePtr *currentNode = args->currentNode;
    xmlNodePtr *currentNodeOld = args->currentNode;

    PyListObject *dupSrcItems = *args->dupSrcItems;
    PyObject *sel = *args->selector;
    PyObject *arglist = (PyObject *)NULL;
    const char *enc = args->enc;
    int *depth = args->depth;
    int *depthcnt = args->depthcnt;

	PyListObject *dupItems = dupSrcItems;
	PyObject *lstItems = (PyObject *)NULL;
	PyObject *item = (PyObject *)o;
	PyObject *itemOld = (PyObject *)NULL;
	PyObject *objKeys = Py_None;
	PyObject *currentKey = Py_None;

    xmlDocPtr doc = (xmlDocPtr)NULL;
	xmlNodePtr newSerializedNode = (xmlNodePtr)NULL;
	xmlNodePtr *xmlRootNode = (xmlNodePtr *)NULL;

	char *objnam = (char *)NULL;
	long listIterator = 0;
	long listSize = 0;

	if (PYTHON_IS_NONE(dupItems)) {
		dupItems = (PyListObject *)PyList_New(0);
	}
	if (PYTHON_IS_NONE(o)) {
		return (xmlNodePtr)NULL;
	}
	if ((pyxser_PyListContains(dupItems, o)) == PYXSER_NOT_FOUND) {
		PyList_Append((PyObject *)dupItems, o);
		objnam = pyxser_GetClassName(o);
		if (objnam == (char *)NULL) {
			return (xmlNodePtr)NULL;
		}
		if (docptr != (xmlDocPtr *)NULL
            && *docptr == (xmlDocPtr)NULL
            && rootNode != (xmlNodePtr *)NULL
            && *rootNode == (xmlNodePtr)NULL) {
            pyxser_CreateDocument(doc, docptr, rootNode, xmlRootNode,
                                  enc, o, objnam, currentNode);
		}
		pyxser_AddIdentifier(o, *currentNode);

        if (PYTHON_IS_NONE(sel)) {
            lstItems = PyObject_GetAttrString(o, pyxser_attr_dict);
        } else {
            /* selector must return NULL if there are errors... */
            arglist = Py_BuildValue("(O)", o);
            lstItems = PyObject_CallObject(sel, arglist);
            Py_XDECREF(arglist);
        }

		if (PYTHON_IS_NONE(lstItems)
			|| (PYTHON_IS_NOT_NONE(lstItems)
				&& (long)(PyDict_Size((PyObject *)lstItems)) <= 0)) {
            PyErr_Clear();
			return (xmlNodePtr)NULL;
		}
		objKeys = PyDict_Keys(lstItems);
		if (PYTHON_IS_NONE(objKeys)
			|| (PYTHON_IS_NOT_NONE(objKeys)
				&& (long)(PyList_Size((PyObject *)objKeys)) <= 0)) {
            PYXSER_FREE_OBJECT(lstItems);
            PYXSER_FREE_OBJECT(objKeys);
            PyErr_Clear();
			return (xmlNodePtr)NULL;
		}
		listIterator = 0;
		listSize = (long)(PyList_Size((PyObject *)objKeys));
		while (listIterator < listSize) {
			currentKey = PyList_GetItem(objKeys, listIterator);
			item = PyDict_GetItem(lstItems, currentKey);
            Py_XINCREF(currentKey);
            Py_XINCREF(item);
			if (PYTHON_IS_NONE(item)) {
				listIterator++;
                Py_XDECREF(currentKey);
                Py_XDECREF(item);
				continue;
			}
            (*depthcnt)++;
            if (*depthcnt <= *depth) {

                oold = &o;
                docptrold = docptr;
                currentNodeOld = currentNode;
                rootNodeOld = rootNode;

                args->o = &item;
                args->item = &item;
                args->ck = &currentKey;
                args->dupSrcItems = &dupItems;
                args->rootNode = rootNode;
                args->currentNode = currentNode;
                args->dupSrcItems = &dupItems;
                itemOld = item;

                newSerializedNode = pyxser_RunSerialization(args);

                args->docptr = docptrold;
                args->rootNode = rootNodeOld;
                args->currentNode = currentNodeOld;
                args->o = oold;
                args->item = oold;
                item = itemOld;

            }
            Py_XDECREF(currentKey);
            Py_XDECREF(item);
            (*depthcnt)--;
			listIterator++;
		}
	} else {
		newSerializedNode = pyxser_AddReference(o, *currentNode);
	}
    PYXSER_FREE_OBJECT(objKeys);
    PYXSER_FREE_OBJECT(lstItems);
	return *rootNode;
}

xmlNodePtr
pyxser_RunSerialization(PyxSerializationArgsPtr args)
{
	PythonTypeSerialize cs;

    PyObject *item = *args->item;
    PyObject *o = *args->o;
    PyObject **oold = args->o;
    PyObject *currentKey = *args->ck;
    PyObject *className;

    xmlNodePtr txtNode = (xmlNodePtr)NULL;
    xmlDocPtr *docptr = args->docptr;
    xmlNodePtr currentNode = *args->currentNode;
    xmlNodePtr *currentNodeOld = args->currentNode;
    xmlNodePtr rootNode = *args->currentNode;
    xmlNodePtr *rootNodeOld = args->currentNode;

    int *depthcnt = args->depthcnt;

	xmlNodePtr csn;
	xmlNodePtr newSerializedNode = (xmlNodePtr)NULL;
	xmlAttrPtr pyxserType = (xmlAttrPtr)NULL;
	xmlAttrPtr pxsnam = (xmlAttrPtr)NULL;

	char *objnam = (char *)NULL;
	int c = 0;

    if (PYTHON_IS_NOT_NONE(args->typemap)) {
        className = PyObject_GetAttrString(o, pyxser_attr_class);
        className = PyObject_GetAttrString(className, pyxser_attr_name);

        txtNode = pyxser_TypeMapSearchAndGetNode(args->typemap, className,
                                                 o, *docptr);
    }

    if (txtNode != (xmlNodePtr)NULL) {
        args->o = &item;
        txtNode = pyxser_TypeMapSearchAndGet(args, txtNode);
        currentNode = *currentNodeOld;
        args->o = oold;
        args->item = oold;
        if (txtNode != (xmlNodePtr)NULL) {
            xmlAddChild(currentNode, txtNode);
            return txtNode;
        }
    } else {
        cs = serxConcreteTypes[c];
        while (cs.available == 1) {
            if (cs.checker(item)) {
                args->o = &item;
                args->name = PyString_AS_STRING(currentKey);
                newSerializedNode = cs.serializer(args);
                currentNode = *currentNodeOld;
                args->o = oold;
                args->item = oold;
                if (newSerializedNode != (xmlNodePtr)NULL) {
                    xmlAddChild(currentNode, newSerializedNode);
                    break;
                }
            }
            cs = serxConcreteTypes[++c];
            if (cs.available == 0) {
                objnam = pyxser_GetClassName(item);
                csn = xmlNewDocNode(*docptr,
                                    pyxser_GetDefaultNs(),
                                    BAD_CAST pyxser_xml_element_object,
                                    NULL);
                pyxser_AddModuleAttr(o, csn);
                pyxserType = xmlNewProp(csn,
                                        BAD_CAST pyxser_xml_attr_type,
                                        BAD_CAST objnam);
                if (PYTHON_IS_NOT_NONE(currentKey)) {
                    pxsnam = xmlNewProp(csn,
                                        BAD_CAST pyxser_xml_attr_name,
                                        BAD_CAST PyString_AS_STRING(currentKey));
                }
                xmlAddChild(currentNode, csn);
                (*depthcnt)++;

                args->o = &item;
                args->item = &item;
                args->currentNode = &csn;
                args->rootNode = &rootNode;

                newSerializedNode = pyxser_SerializeXml(args);

                args->currentNode = currentNodeOld;
                args->rootNode = rootNodeOld;
                args->o = oold;
                args->item = oold;

                (*depthcnt)--;
                c = 0;
                break;
            }
        }
    }
	return newSerializedNode;
}

int
pyxser_ModuleNotMain(const char *mod)
{
	if (strncmp(type_main, mod, strlen(type_main)) == 0) {
		return 0;
	}
	return 1;
}


inline PyObject *
pyxser_UnserializeElement(PyObject *ct, PyObject **current,
                          PyDictObject **dups, PyObject *cacheCurrent,
                          xmlNodePtr cacheCurrentNode, xmlNodePtr ron,
                          const char *n_id,
                          PyxSerDeserializationArgsPtr obj)
{
    PyObject *unser = (PyObject *)NULL;
    PyObject *ndict = (PyObject *)NULL;
    char *attr_name = (char *)NULL;
    int ctrl = 0;
    unser = PyObject_CallFunctionObjArgs(ct, NULL);
    PyErr_Clear();
    if (PYTHON_IS_NONE(unser)) {
        ndict = PyDict_New();
        unser = PyInstance_NewRaw(ct, ndict);
    }

    attr_name = pyxser_ExtractPropertyName(pyxser_xml_attr_name,
                                           ron);
    if (PYTHON_IS_NOT_NONE(unser)) {
        if (attr_name != (char *)NULL) {
            ctrl = PyObject_SetAttrString(*current,
                                          attr_name, unser);
            PYXSER_XMLFREE(attr_name);
            Py_XDECREF(unser);
        }
        pyxser_AddAvailableObject((PyObject *)*dups,
                                  (char *)n_id, unser);
        cacheCurrent = *(obj->current);
        cacheCurrentNode = ron;
        *(obj->current) = unser;
        *(obj->currentNode) = ron;
        unser = pyxser_UnserializeBlock(obj);
        *(obj->current) = cacheCurrent;
        *(obj->currentNode) = cacheCurrentNode;
        ron = cacheCurrentNode;
    }
    Py_XDECREF(ndict);
    return unser;
}

inline void
pyxser_RunDeserializationMachine(xmlNodePtr ron,
                                 PyObject **current,
                                 PyxSerDeserializationArgsPtr obj)
{
    PyObject *unser;
    char *tvalc;
    PyObject *tval;
	PythonTypeDeserialize *machine =
        (PythonTypeDeserialize *)unserxConcreteTypes;
    char *attr_name;
    int c, ctrl;

    c = 0;
    if (PYTHON_IS_NONE(*current)) {
        return;
    }
    tvalc = pyxser_ExtractPropertyName(pyxser_xml_attr_type,
                                       ron);
    tval = PyString_FromString(tvalc);
    unser = pyxunser_TypeMapSearchAndGet(obj->typemap, tval,
                                         *(obj->currentNode));
    PYXSER_XMLFREE(tvalc);
    Py_XDECREF(tval);
    if (PYTHON_IS_NOT_NONE(unser)) {
        attr_name = pyxser_ExtractPropertyName(
            pyxser_xml_attr_name,
            ron);
        if (attr_name != (char *)NULL) {
            ctrl = PyObject_SetAttrString(*current,
                                          attr_name,
                                          unser);
            PYXSER_XMLFREE(attr_name);
            Py_XDECREF(unser);
            return;
        }
    }
    while (machine[c].available == 1
           && ron != (xmlNodePtr)NULL) {
        if ((machine[c].check(ron)) == 1) {
            unser = machine[c].deserializer(obj);
            if (PYTHON_IS_NONE(unser)) {
                c++;
                continue;
            }
            attr_name = pyxser_ExtractPropertyName(
                pyxser_xml_attr_name,
                ron);
            if (attr_name != (char *)NULL) {
                ctrl = PyObject_SetAttrString(*current,
                                              attr_name,
                                              unser);
                PYXSER_XMLFREE(attr_name);
                Py_XDECREF(unser);
                break;
            }
        }
        c++;
    }
}

PyObject *
pyxser_UnserializeBlock(PyxSerDeserializationArgsPtr obj)
{
	PyObject **current = obj->current;
	PyDictObject **dups = obj->dups;
	PyDictObject **modules = obj->modules;
	PyObject *ct = (PyObject *)NULL;
	PyObject *unser = (PyObject *)NULL;
	PyObject *cacheCurrent = (PyObject *)NULL;
	xmlNodePtr *currentNode = obj->currentNode;
	xmlNodePtr ron = (xmlNodePtr)NULL;
	xmlNodePtr cacheCurrentNode = (xmlNodePtr)NULL;

	char *attr_name = (char *)NULL;
	int ctrl = 0;
	char *n_type = (char *)NULL;
	char *n_module = (char *)NULL;
	char *n_id = (char *)NULL;
	char *n_ref = (char *)NULL;
	int len_element = strlen((char *)pyxser_xml_element_prop);
	int len_collection = strlen((char *)pyxser_xml_element_collection);
	int len_object = strlen((char *)pyxser_xml_element_object);

	for (ron = (*currentNode)->children;
		 ron != (xmlNodePtr)NULL;
		 ron = ron->next) {
		*currentNode = ron;
		if (ron->type == XML_ELEMENT_NODE) {
			if ((strncmp((char *)ron->name,
						 (char *)pyxser_xml_element_prop,
						 len_element)) == 0
				|| (strncmp((char *)ron->name,
							(char *)pyxser_xml_element_collection,
							len_collection)) == 0) {

                pyxser_RunDeserializationMachine(ron, current, obj);

			} else if ((strncmp((char *)ron->name,
								(char *)pyxser_xml_element_object,
								len_object)) == 0) {

				n_type = (char *)xmlGetProp(ron, BAD_CAST pyxser_xml_attr_type);
				n_module = (char *)xmlGetProp(ron, BAD_CAST pyxser_xml_attr_module);
				n_id = (char *)xmlGetProp(ron, BAD_CAST pyxser_xml_attr_id);
				n_ref = (char *)xmlGetProp(ron, BAD_CAST pyxser_xml_attr_ref);
				unser = pyxser_CheckAvailableObject((PyObject *)*dups,
													(char *)n_ref);
				if (n_type != (char *)NULL
					&& n_module != (char *)NULL
					&& n_id != (char *)NULL
					&& unser == (PyObject *)NULL) {
                    ct = pyxser_SearchTypesInModules(n_module, n_type, modules);
					if (PYTHON_IS_NOT_NONE(ct)) {
                        unser = pyxser_UnserializeElement(ct, current, dups, cacheCurrent,
                                                          cacheCurrentNode, ron, n_id, obj);
					} else {
						ct = pyxser_SearchObjectInMain(n_type);
						if (PYTHON_IS_NOT_NONE(ct)) {
                            unser = pyxser_UnserializeElement(ct, current, dups, cacheCurrent,
                                                              cacheCurrentNode, ron, n_id, obj);
                        }
					}
                    PYXSER_XMLFREE(n_type);
                    PYXSER_XMLFREE(n_module);
                    PYXSER_XMLFREE(n_id);
                    PYXSER_XMLFREE(n_ref);
				} else {
					attr_name = pyxser_ExtractPropertyName(pyxser_xml_attr_name, ron);
					if (attr_name != (char *)NULL
						&& PYTHON_IS_NOT_NONE(unser)) {
						ctrl = PyObject_SetAttrString(*current, attr_name, unser);
                        PYXSER_XMLFREE(attr_name);
					}
				}
                PYXSER_XMLFREE(n_type);
                PYXSER_XMLFREE(n_module);
                PYXSER_XMLFREE(n_id);
                PYXSER_XMLFREE(n_ref);
			}
		}
	}
	return *(obj->current);
}

PyObject *
pyxser_UnserializeXml(PyxSerDeserializationArgsPtr obj)
{
	PyObject **doc = obj->doc;
	PyObject **current = obj->current;
	PyObject **tree = obj->tree;
	PyDictObject **dups = obj->dups;
	PyDictObject **modules = obj->modules;
	PyObject *cacheCurrent = (PyObject *)NULL;
	PyObject *ct = (PyObject *)NULL;
    PyObject *ndict = (PyObject *)NULL;

	xmlDocPtr *docptr = obj->docPtr;
	xmlNodePtr *rootNode = obj->rootNode;
	xmlNodePtr *currentNode = obj->currentNode;
	xmlNodePtr cacheCurrentNode = (xmlNodePtr)NULL;

	char *n_type = (char *)NULL;
	char *n_module = (char *)NULL;
	char *n_id = (char *)NULL;
	char *strdoc = (char *)NULL;

    int parseopts = XML_PARSE_RECOVER;
    int validity = 0;

	if (PYTHON_IS_NONE(doc)) {
		PyErr_SetString(PyExc_ValueError, "Invalid XML");
		return NULL;
	}

	strdoc = PyString_AS_STRING(*doc);
	if (strdoc == (char *)NULL) {
		PyErr_SetString(PyExc_ValueError, "Invalid XML");
		return NULL;
	}

	*docptr = xmlReadMemory((const char *)strdoc, PyString_GET_SIZE(*doc),
                            NULL, (const char *)(obj->encoding), parseopts);

    if (*docptr == (xmlDocPtr)NULL) {
		PyErr_SetString(PyExc_ValueError, "Invalid XML");
        return NULL;
    }

    validity = pyxser_ValidateDocumentXSD(*docptr);

    if (validity == 1) {
        if (*docptr != (xmlDocPtr)NULL) {
            *rootNode = xmlDocGetRootElement(*docptr);
            currentNode = rootNode;
            *currentNode = *rootNode;
            obj->currentNode = currentNode;
            n_type = (char *)xmlGetProp(*currentNode,
                                        BAD_CAST pyxser_xml_attr_type);
            n_module = (char *)xmlGetProp(*currentNode,
                                          BAD_CAST pyxser_xml_attr_module);
            n_id = (char *)xmlGetProp(*currentNode,
                                      BAD_CAST pyxser_xml_attr_id);
            if (*dups == (PyDictObject *)NULL) {
                *dups = (PyDictObject *)PyDict_New();
            }
            if (n_type != (char *)NULL
                && n_module != (char *)NULL
                && n_id != (char *)NULL) {
                if ((pyxser_ModuleNotMain(n_module)) == 1) {
                    ct = pyxser_SearchTypesInModules(n_module,
                                                     n_type,
                                                     modules);
                    if (PYTHON_IS_NOT_NONE(ct)) {
                        if (*tree == (PyObject *)NULL) {
                            *tree = PyObject_CallFunctionObjArgs(ct, NULL);
                            PyErr_Clear();
                            if (PYTHON_IS_NONE(*tree)) {
                                ndict = PyDict_New();
                                *tree = PyInstance_NewRaw(ct, ndict);
                            }
                            *current = *tree;
                            obj->current = current;
                            obj->tree = tree;
                            pyxser_UnserializeBlock(obj);
                            *(obj->current) = cacheCurrent;
                            *(obj->currentNode) = cacheCurrentNode;
                            obj->tree = tree;
                            Py_XDECREF(ndict);
                        }
                    }
                } else {
                    ct = pyxser_SearchObjectInMain(n_type);
                    if (PYTHON_IS_NOT_NONE(ct)) {
                        if (*tree == (PyObject *)NULL) {
                            *tree = PyObject_CallFunctionObjArgs(ct, NULL);
                            PyErr_Clear();
                            if (PYTHON_IS_NONE(*tree)) {
                                ndict = PyDict_New();
                                *tree = PyInstance_NewRaw(ct, ndict);
                            }
                            *current = *tree;
                            obj->current = current;
                            obj->tree = tree;
                            pyxser_UnserializeBlock(obj);
                            *(obj->current) = cacheCurrent;
                            *(obj->currentNode) = cacheCurrentNode;
                            Py_XDECREF(ndict);
                        }
                    }
                }
            }
            PYXSER_XMLFREE(n_type);
            PYXSER_XMLFREE(n_module);
            PYXSER_XMLFREE(n_id);
        }
    }
	if (*docptr != (xmlDocPtr)NULL) {
		xmlFreeDoc(*docptr);
	}
    PYXSER_FREE_OBJECT(*dups);
	return *tree;
}

PyObject *
pyxser_SearchModuleType(PyObject *mod, const char *name)
{
	PyObject *dict = (PyObject *)NULL;
	PyObject *item = (PyObject *)NULL;
	PyObject *objKeys = (PyObject *)NULL;
	PyObject *currentKey = (PyObject *)NULL;

	long listIterator = 0;
	long listSize = 0;
	char *keyName = (char *)NULL;

	if (PYTHON_IS_NONE(mod)
		|| name == (const char *)NULL) {
		return Py_None;
	}
	dict = PyObject_GetAttrString(mod, pyxser_attr_dict);
	objKeys = PyDict_Keys(dict);
	if (PYTHON_IS_NOT_NONE(objKeys)
		&& (long)(PyList_Size((PyObject *)objKeys)) > 0
		&& PYTHON_IS_NOT_NONE(dict)) {
		listIterator = 0;
		listSize = (long)(PyList_Size((PyObject *)objKeys));
		while (listIterator < listSize) {
			currentKey = PyList_GetItem(objKeys, listIterator);
			keyName = PyString_AS_STRING(currentKey);
			if ((strncmp(keyName, name, strlen(keyName))) == 0) {
				item = PyDict_GetItem(dict, currentKey);
				break;
			}
			listIterator++;
		}
    }
    Py_XDECREF(objKeys);
    Py_XDECREF(dict);
    PyErr_Clear();
	return item;
}


/* pyserx_tools.h ends here */
