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


static xmlNodePtr pyxser_GlobalTupleSerialization(PyObject *o,
												  char *name,
												  PyListObject *dupItems,
												  xmlDocPtr docPtr);

static xmlNodePtr pyxser_GlobalListSerialization(PyObject *o,
												 char *name,
												 PyListObject *dupItems,
												 xmlDocPtr docPtr);

static xmlNodePtr pyxser_GlobalDictSerialization(PyObject *o,
												 char *name,
												 PyListObject *dupItems,
												 xmlDocPtr docPtr);


/* Tuples */
xmlNodePtr
pyxser_SerializeTuple(PyObject *o, char *name,
					  PyListObject *dupItems, xmlDocPtr docPtr)
{
	return pyxser_GlobalTupleSerialization(o, name, dupItems, docPtr);
}

xmlNodePtr
pyxser_SerializeExactTuple(PyObject *o, char *name,
						   PyListObject *dupItems, xmlDocPtr docPtr)
{
	return pyxser_GlobalTupleSerialization(o, name, dupItems, docPtr);
}

/* Lists */
xmlNodePtr
pyxser_SerializeList(PyObject *o, char *name,
					 PyListObject *dupItems, xmlDocPtr docPtr)
{
	return pyxser_GlobalListSerialization(o, name, dupItems, docPtr);
}

xmlNodePtr
pyxser_SerializeExactList(PyObject *o, char *name,
						  PyListObject *dupItems, xmlDocPtr docPtr)
{
	return pyxser_GlobalListSerialization(o, name, dupItems, docPtr);
}
/* Dictionaries */

xmlNodePtr
pyxser_SerializeDict(PyObject *o, char *name,
					 PyListObject *dupItems, xmlDocPtr docPtr)
{
	return pyxser_GlobalDictSerialization(o, name, dupItems, docPtr);
}

xmlNodePtr
pyxser_SerializeExactDict(PyObject *o, char *name,
						  PyListObject *dupItems, xmlDocPtr docPtr)
{
	return pyxser_GlobalDictSerialization(o, name, dupItems, docPtr);
}

static xmlNodePtr
pyxser_GlobalListSerialization(PyObject *o, char *name,
							   PyListObject *dupItems, xmlDocPtr docPtr)
{
	xmlNodePtr newElementNode = (xmlNodePtr)NULL;
	xmlAttrPtr typeAttr = (xmlAttrPtr)NULL;
	xmlAttrPtr namePtr = (xmlAttrPtr)NULL;
	xmlNodePtr newSerializedNode = (xmlNodePtr)NULL;
	xmlNodePtr currentSubNode = (xmlNodePtr)NULL;
	PyObject *classPtr = (PyObject *)NULL;
	PyObject *className = (PyObject *)NULL;
	PyObject *item = (PyObject *)NULL;
	char *objectName = (char *)NULL;
	char *typeName = (char *)NULL;
	PythonTypeSerialize currentSerialization;
	long tupleSize = 0;
	long counter = 0, d = 0;
	int serialized = 0;
	int phase;
	char *nptr = (char *)NULL;
	if (PYTHON_IS_NOT_NONE(o)) {
		classPtr = PyObject_GetAttrString(o, pyxser_attr_class);
		if (PYTHON_IS_NOT_NONE(classPtr)) {
			className = PyObject_GetAttrString(classPtr, pyxser_attr_name);
			if (PYTHON_IS_NOT_NONE(className)) {
				nptr = PyString_AS_STRING(className);
				if (nptr != (char *)NULL) {
					newElementNode = xmlNewDocNode(docPtr, pyxser_GetDefaultNs(),
												   BAD_CAST pyxser_xml_element_collection,
												   NULL);
					typeAttr = xmlNewProp(newElementNode,
										  BAD_CAST pyxser_xml_attr_type,
										  BAD_CAST nptr);
					namePtr = xmlNewProp(newElementNode,
										 BAD_CAST pyxser_xml_attr_name,
										 BAD_CAST name);
					tupleSize = PyList_GET_SIZE(o);
					for (counter = 0; counter < tupleSize; counter++) {
						item = PyList_GetItem(o, counter);
						if (PYTHON_IS_NOT_NONE(item)) {
							d = 0;
							currentSerialization = serxConcreteTypes[d];
							phase = PYTHON_SERX_PHASE_NONE;
							serialized = 0;
							while (currentSerialization.available == 1
								   && phase == PYTHON_SERX_PHASE_NONE) {
								if (currentSerialization.checker(item)
									&& serialized == 0) {
									newSerializedNode = currentSerialization.serializer(
										item,
										(char *)pyxser_xml_element_item,
										(PyListObject *)NULL,
										docPtr);
									if (newSerializedNode != (xmlNodePtr)NULL) {
										xmlAddChild(newElementNode, newSerializedNode);
										phase = PYTHON_SERX_PHASE_CONCRETE;
										serialized = 1;
									}
								}
								currentSerialization = serxConcreteTypes[++d];
								if (currentSerialization.available == 0
									&& phase == PYTHON_SERX_PHASE_NONE
									&& serialized == 0) {
									if ((pyxser_PyListContains(dupItems, item)) == PYXSER_FOUND) {
										typeName = pyxser_GetClassName(item);
										if (typeName != (char *)NULL) {
											newSerializedNode = xmlNewDocNode(docPtr, pyxser_GetDefaultNs(),
																			  BAD_CAST pyxser_xml_element_object,
																			  NULL);
											namePtr = xmlNewProp(newSerializedNode,
																 BAD_CAST pyxser_xml_attr_type,
																 BAD_CAST typeName);
											pyxser_AddReference(item, newSerializedNode);
											xmlAddChild(newElementNode, newSerializedNode);
										}
									} else {
										phase = PYTHON_SERX_PHASE_OBJECT;
										objectName = pyxser_GetClassName(item);
										currentSubNode = xmlNewDocNode(docPtr, pyxser_GetDefaultNs(),
																	   BAD_CAST pyxser_xml_element_object,
																	   NULL);
										namePtr = xmlNewProp(currentSubNode,
															 BAD_CAST pyxser_xml_attr_type,
															 BAD_CAST objectName);
										pyxser_AddModuleAttr(item, currentSubNode);
										newSerializedNode = pyxser_SerializeXml(item,
																				&docPtr,
																				&currentSubNode,
																				&currentSubNode,
																				dupItems);
										xmlAddChild(newElementNode, currentSubNode);
									}
								}
								phase = PYTHON_SERX_PHASE_NONE;
							}
						}
					}
				}
				Py_DECREF(className);
			}
			Py_DECREF(classPtr);
		} /* PYTHON_IS_NOT_NONE(classPtr) */
	} /* PYTHON_IS_NOT_NONE(o) */
	return newElementNode;
}

static xmlNodePtr
pyxser_GlobalTupleSerialization(PyObject *o, char *name, PyListObject *dupItems, xmlDocPtr docPtr)
{
	xmlNodePtr newElementNode = (xmlNodePtr)NULL;
	xmlAttrPtr typeAttr = (xmlAttrPtr)NULL;
	xmlAttrPtr namePtr = (xmlAttrPtr)NULL;
	xmlNodePtr newSerializedNode = (xmlNodePtr)NULL;
	xmlNodePtr currentSubNode = (xmlNodePtr)NULL;
	xmlAttrPtr pyxserType = (xmlAttrPtr)NULL;
	PyObject *classPtr = (PyObject *)NULL;
	PyObject *className = (PyObject *)NULL;
	PyObject *item = (PyObject *)NULL;
	char *objectName = (char *)NULL;
	char *typeName = (char *)NULL;
	PythonTypeSerialize currentSerialization;
	long tupleSize = 0;
	long counter = 0, d = 0;
	int serialized = 0;
	int phase;
	char *nptr = (char *)NULL;
	if (PYTHON_IS_NOT_NONE(o)) {
		classPtr = PyObject_GetAttrString(o, pyxser_attr_class);
		if (PYTHON_IS_NOT_NONE(classPtr)) {
			className = PyObject_GetAttrString(classPtr, pyxser_attr_name);
			if (PYTHON_IS_NOT_NONE(className)) {
				nptr = PyString_AS_STRING(className);
				if (nptr != (char *)NULL) {
					newElementNode = xmlNewDocNode(docPtr, pyxser_GetDefaultNs(),
												   BAD_CAST pyxser_xml_element_collection,
												   NULL);
					typeAttr = xmlNewProp(newElementNode,
										  BAD_CAST pyxser_xml_attr_type,
										  BAD_CAST nptr);
					namePtr = xmlNewProp(newElementNode,
										 BAD_CAST pyxser_xml_attr_name,
										 BAD_CAST name);
					tupleSize = PyTuple_GET_SIZE(o);
					for (counter = 0; counter < tupleSize; counter++) {
						item = PyTuple_GetItem(o, counter);
						if (PYTHON_IS_NOT_NONE(item)) {
							d = 0;
							currentSerialization = serxConcreteTypes[d];
							phase = PYTHON_SERX_PHASE_NONE;
							serialized = 0;
							while (currentSerialization.available == 1
								   && phase == PYTHON_SERX_PHASE_NONE) {
								if (currentSerialization.checker(item)
									&& serialized == 0) {
									newSerializedNode = currentSerialization.serializer(
										item,
										(char *)pyxser_xml_element_item,
										(PyListObject *)NULL, docPtr);
									if (newSerializedNode != (xmlNodePtr)NULL) {
										xmlAddChild(newElementNode, newSerializedNode);
										phase = PYTHON_SERX_PHASE_CONCRETE;
										serialized = 1;
									}
								}
								currentSerialization = serxConcreteTypes[++d];
								if (currentSerialization.available == 0
									&& phase == PYTHON_SERX_PHASE_NONE
									&& serialized == 0) {
									if ((pyxser_PyListContains(dupItems, item)) == PYXSER_FOUND) {
										typeName = pyxser_GetClassName(item);
										if (typeName != (char *)NULL) {
											newSerializedNode = xmlNewDocNode(docPtr, pyxser_GetDefaultNs(),
																			  BAD_CAST pyxser_xml_element_object,
																			  NULL);
											namePtr = xmlNewProp(newSerializedNode,
																 BAD_CAST pyxser_xml_attr_type,
																 BAD_CAST typeName);
											pyxser_AddReference(item, newSerializedNode);
											xmlAddChild(newElementNode, newSerializedNode);
										}
									} else {
										phase = PYTHON_SERX_PHASE_OBJECT;
										objectName = pyxser_GetClassName(item);
										currentSubNode = xmlNewDocNode(docPtr, pyxser_GetDefaultNs(),
																	   BAD_CAST pyxser_xml_element_object,
																	   NULL);
										pyxserType = xmlNewProp(currentSubNode,
																BAD_CAST pyxser_xml_attr_type,
																BAD_CAST objectName);
										pyxser_AddModuleAttr(item, currentSubNode);
										newSerializedNode = pyxser_SerializeXml(item,
																				&docPtr,
																				&currentSubNode,
																				&currentSubNode,
																				dupItems);
										xmlAddChild(newElementNode, currentSubNode);
									}
								}
								phase = PYTHON_SERX_PHASE_NONE;
							}
						}
					}
				}
				Py_DECREF(className);
			}
			Py_DECREF(classPtr);
		}
	}
	return newElementNode;
}

static xmlNodePtr
pyxser_GlobalDictSerialization(PyObject *o, char *name, PyListObject *dupItems, xmlDocPtr docPtr)
{
	xmlNodePtr newElementNode = (xmlNodePtr)NULL;
	xmlAttrPtr typeAttr = (xmlAttrPtr)NULL;
	xmlAttrPtr namePtr = (xmlAttrPtr)NULL;
	xmlAttrPtr keyAttr = (xmlAttrPtr)NULL;
	xmlNodePtr newSerializedNode = (xmlNodePtr)NULL;
	xmlNodePtr currentSubNode = (xmlNodePtr)NULL;
	xmlAttrPtr pyxserType = (xmlAttrPtr)NULL;
	PyObject *classPtr = (PyObject *)NULL;
	PyObject *className = (PyObject *)NULL;
	PyObject *item = (PyObject *)NULL;
	PyObject *keyString = (PyObject *)NULL;
	PyObject *dictKeys = (PyObject *)NULL;
	PyObject *currentKey = (PyObject *)NULL;
	char *keyCharp = (char *)NULL;
	char *objectName = (char *)NULL;
	char *typeName = (char *)NULL;
	PythonTypeSerialize currentSerialization;
	long tupleSize = 0;
	long counter = 0, d = 0;
	int serialized = 0;
	int phase;
	char *nptr = (char *)NULL;
	if (PYTHON_IS_NOT_NONE(o)) {
		classPtr = PyObject_GetAttrString(o, pyxser_attr_class);
		if (PYTHON_IS_NOT_NONE(classPtr)) {
			className = PyObject_GetAttrString(classPtr, pyxser_attr_name);
			if (PYTHON_IS_NOT_NONE(className)) {
				nptr = PyString_AS_STRING(className);
				if (nptr != (char *)NULL) {
					newElementNode = xmlNewDocNode(docPtr, pyxser_GetDefaultNs(),
												   BAD_CAST pyxser_xml_element_collection,
												   NULL);
					typeAttr = xmlNewProp(newElementNode,
										  BAD_CAST pyxser_xml_attr_type,
										  BAD_CAST nptr);
					namePtr = xmlNewProp(newElementNode,
										 BAD_CAST pyxser_xml_attr_name,
										 BAD_CAST name);
					tupleSize = PyDict_Size(o);
					dictKeys = PyDict_Keys(o);
					if (PYTHON_IS_NOT_NONE(dictKeys)) {
						for (counter = 0; counter < tupleSize; counter++) {
							currentKey = PyList_GetItem(dictKeys, counter);
							item = PyDict_GetItem(o, currentKey);
							if (PYTHON_IS_NOT_NONE(item)) {
								d = 0;
								currentSerialization = serxConcreteTypes[d];
								phase = PYTHON_SERX_PHASE_NONE;
								serialized = 0;
								while (currentSerialization.available == 1
									   && phase == PYTHON_SERX_PHASE_NONE) {
									if (currentSerialization.checker(item)
										&& serialized == 0) {
										newSerializedNode = currentSerialization.serializer(
											item,
											(char *)pyxser_xml_element_item,
											(PyListObject *)NULL,
											docPtr);
										if (newSerializedNode != (xmlNodePtr)NULL) {
											keyString = PyList_GetItem(dictKeys, counter);
											keyCharp = PyString_AS_STRING(keyString);
											keyAttr = xmlNewProp(newSerializedNode,
																 BAD_CAST pyxser_xml_attr_key,
																 BAD_CAST keyCharp);
											xmlAddChild(newElementNode, newSerializedNode);
											phase = PYTHON_SERX_PHASE_CONCRETE;
											serialized = 1;
										}
									}
									currentSerialization = serxConcreteTypes[++d];
									if (currentSerialization.available == 0
										&& phase == PYTHON_SERX_PHASE_NONE
										&& serialized == 0) {
										if ((pyxser_PyListContains(dupItems, item)) == PYXSER_FOUND) {
											typeName = pyxser_GetClassName(item);
											if (typeName != (char *)NULL) {
												newSerializedNode = xmlNewDocNode(docPtr, pyxser_GetDefaultNs(),
																				  BAD_CAST pyxser_xml_element_object,
																				  NULL);
												pyxser_AddReference(item, newSerializedNode);
												keyString = PyList_GetItem(dictKeys, counter);
												keyCharp = PyString_AS_STRING(keyString);
												keyAttr = xmlNewProp(newSerializedNode,
																	 BAD_CAST pyxser_xml_attr_key,
																	 BAD_CAST keyCharp);
												xmlAddChild(newElementNode, newSerializedNode);
											}
										} else {
											phase = PYTHON_SERX_PHASE_OBJECT;
											objectName = pyxser_GetClassName(item);
											currentSubNode = xmlNewDocNode(docPtr, pyxser_GetDefaultNs(),
																		   BAD_CAST pyxser_xml_element_object,
																		   NULL);
											pyxser_AddModuleAttr(item, currentSubNode);
											newSerializedNode = pyxser_SerializeXml(item,
																					&docPtr,
																					&currentSubNode,
																					&currentSubNode,
																					dupItems);
											keyString = PyList_GetItem(dictKeys, counter);
											keyCharp = PyString_AS_STRING(keyString);
											keyAttr = xmlNewProp(currentSubNode,
																 BAD_CAST pyxser_xml_attr_key,
																 BAD_CAST keyCharp);
											pyxserType = xmlNewProp(currentSubNode,
																	BAD_CAST pyxser_xml_attr_type,
																	BAD_CAST objectName);
											xmlAddChild(newElementNode, currentSubNode);
										}
									}
									phase = PYTHON_SERX_PHASE_NONE;
								}
							}
						}
						Py_DECREF(dictKeys);
					}
				}
				Py_DECREF(className);
			}
			Py_DECREF(classPtr);
		}
	}
	return newElementNode;
}


PyObject *
pyxunser_SerializeTuple(PythonUnserializationArgumentsPtr obj)
{
	xmlNodePtr node = *(obj->currentNode);
	/* unserialization arguments */
	PyDictObject **dups = obj->dups;
	PyDictObject **modules = obj->modules;
	char *n_type = (char *)NULL;
	char *n_module = (char *)NULL;
	char *n_id = (char *)NULL;
	char *n_ref = (char *)NULL;
	PyObject *cacheMod = (PyObject *)NULL;
	PyObject *currentType = (PyObject *)NULL;
	PythonTypeDeserialize *machine = (PythonTypeDeserialize *)unserxConcreteTypes;
	PyObject *tuple = (PyObject *)NULL;
	PyObject *unser = (PyObject *)NULL;
	PyObject *cacheCurrent = (PyObject *)NULL;
	PyObject *chkMod = (PyObject *)NULL;
	Py_ssize_t tsz = 0;
	xmlNodePtr ron = (xmlNodePtr)NULL;
	xmlNodePtr cacheNode = (xmlNodePtr)NULL;
	xmlNodePtr cacheCurrentNode = (xmlNodePtr)NULL;
	int c = 0, serialized = 0;

	if (node != (xmlNodePtr)NULL) {
		tuple = PyTuple_New(0);
		for (ron = node->children;
			 ron;
			 ron = ron->next) {
			*(obj->currentNode) = ron;
			if (ron->type == XML_ELEMENT_NODE) {
				if ((strncmp((char *)ron->name,
							 (char *)pyxser_xml_element_item,
							 strlen((char *)pyxser_xml_element_item))) == 0) {
					c = 0;
					serialized = 0;
					while (machine[c].available == 1
						   && serialized == 0) {
						if ((machine[c].check(ron)) == 1) {
							unser = machine[c].deserializer(obj);
							if (PYTHON_IS_NOT_NONE(unser)) {
								++tsz;
								_PyTuple_Resize(&tuple, tsz);
								PyTuple_SET_ITEM(tuple, (tsz - 1), unser);
								serialized = 1;
							}
						}
						c++;
					}
				} else if ((strncmp((char *)ron->name,
									(char *)pyxser_xml_element_object,
									strlen((char *)pyxser_xml_element_object))) == 0) {
					cacheNode = ron;
					n_type = (char *)xmlGetProp(ron, BAD_CAST pyxser_xml_attr_type);
					n_module = (char *)xmlGetProp(ron, BAD_CAST pyxser_xml_attr_module);
					n_id = (char *)xmlGetProp(ron, BAD_CAST pyxser_xml_attr_id);
					n_ref = (char *)xmlGetProp(ron, BAD_CAST pyxser_xml_attr_ref);
					unser = pyxser_CheckAvailableObject((PyObject *)*dups, (char *)n_ref);
					if (n_type != (char *)NULL
						&& n_module != (char *)NULL
						&& n_id != (char *)NULL
						&& unser == (PyObject *)NULL) {
						if ((pyxser_ModuleNotMain(n_module)) == 1) {
							cacheMod = pyxser_SearchModule(n_module);
							cacheMod = cacheMod == (PyObject *)NULL ? PyImport_ImportModule(n_module) : cacheMod;
							if (PYTHON_IS_NOT_NONE(cacheMod)) {
								chkMod = pyxser_CacheModule((PyObject *)*modules, n_module);
								if (!PYTHON_IS_NOT_NONE(chkMod)) {
									PyDict_SetItemString((PyObject *)*modules, n_module, cacheMod);
								}
								currentType = (PyObject *)pyxser_SearchModuleType(cacheMod, n_type);
								if (PYTHON_IS_NOT_NONE(currentType)) {
									unser = PyInstance_NewRaw(currentType, PyDict_New());
									if (PYTHON_IS_NOT_NONE(unser)) {
										++tsz;
										_PyTuple_Resize(&tuple, tsz);
										PyTuple_SET_ITEM(tuple, (tsz - 1), unser);
										pyxser_AddAvailableObject((PyObject *)*dups, (char *)n_id, unser);
										cacheCurrent = *(obj->current);
										cacheCurrentNode = ron;
										*(obj->current) = unser;
										*(obj->currentNode) = ron;
										unser = pyxser_UnserializeBlock(obj);
										*(obj->current) = cacheCurrent;
										*(obj->currentNode) = cacheCurrentNode;
										ron = cacheCurrentNode;
									}
								}
							}
						} else {
							currentType = pyxser_SearchObjectInMain(n_type);
							if (PYTHON_IS_NOT_NONE(currentType)) {
								unser = PyInstance_NewRaw(currentType, PyDict_New());
								if (PYTHON_IS_NOT_NONE(unser)) {
									unser = PyInstance_NewRaw(currentType, PyDict_New());
									if (PYTHON_IS_NOT_NONE(unser)) {
										++tsz;
										_PyTuple_Resize(&tuple, tsz);
										PyTuple_SET_ITEM(tuple, (tsz - 1), unser);
										pyxser_AddAvailableObject((PyObject *)*dups, (char *)n_id, unser);
										cacheCurrent = *(obj->current);
										cacheCurrentNode = ron;
										*(obj->current) = unser;
										*(obj->currentNode) = ron;
										unser = pyxser_UnserializeBlock(obj);
										*(obj->current) = cacheCurrent;
										*(obj->currentNode) = cacheCurrentNode;
										ron = cacheCurrentNode;
									}
								}
							}
						}
					} else {
						++tsz;
						_PyTuple_Resize(&tuple, tsz);
						PyTuple_SET_ITEM(tuple, (tsz - 1), unser);
					}
				}
			}
		}
	}
	return tuple;
}

PyObject *
pyxunser_SerializeList(PythonUnserializationArgumentsPtr obj)
{
	xmlNodePtr node = *(obj->currentNode);
	/* unserialization arguments */
	PyDictObject **dups = obj->dups;
	PyDictObject **modules = obj->modules;
	char *n_type = (char *)NULL;
	char *n_module = (char *)NULL;
	char *n_id = (char *)NULL;
	char *n_ref = (char *)NULL;
	PyObject *cacheMod = (PyObject *)NULL;
	PyObject *currentType = (PyObject *)NULL;
	PythonTypeDeserialize *machine = (PythonTypeDeserialize *)unserxConcreteTypes;
	PyObject *unser = (PyObject *)NULL;
	PyObject *cacheCurrent = (PyObject *)NULL;
	PyObject *chkMod = (PyObject *)NULL;
	xmlNodePtr ron = (xmlNodePtr)NULL;
	xmlNodePtr cacheCurrentNode = (xmlNodePtr)NULL;
	int c = 0, serialized = 0;
	PyObject *list = (PyObject *)NULL;

	if (node != (xmlNodePtr)NULL) {
		list = PyList_New(0);
		for (ron = node->children;
			 ron;
			 ron = ron->next) {
			*(obj->currentNode) = ron;
			if (ron->type == XML_ELEMENT_NODE) {
				if ((strncmp((char *)ron->name,
							 (char *)pyxser_xml_element_item,
							 strlen((char *)pyxser_xml_element_item))) == 0) {
					c = 0;
					serialized = 0;
					while (machine[c].available == 1
						   && serialized == 0) {
						if ((machine[c].check(ron)) == 1) {
							unser = machine[c].deserializer(obj);
							if (PYTHON_IS_NOT_NONE(unser)) {
								PyList_Append(list, unser);
								serialized = 1;
							}
						}
						c++;
					}
				} else if ((strncmp((char *)ron->name,
									(char *)pyxser_xml_element_object,
									strlen((char *)pyxser_xml_element_object))) == 0) {
					n_type = (char *)xmlGetProp(ron, BAD_CAST pyxser_xml_attr_type);
					n_module = (char *)xmlGetProp(ron, BAD_CAST pyxser_xml_attr_module);
					n_id = (char *)xmlGetProp(ron, BAD_CAST pyxser_xml_attr_id);
					n_ref = (char *)xmlGetProp(ron, BAD_CAST pyxser_xml_attr_ref);
					unser = pyxser_CheckAvailableObject((PyObject *)*dups, (char *)n_ref);
					if (n_type != (char *)NULL
						&& n_module != (char *)NULL
						&& n_id != (char *)NULL
						&& unser == (PyObject *)NULL) {
						if ((pyxser_ModuleNotMain(n_module)) == 1) {
							cacheMod = pyxser_SearchModule(n_module);
							cacheMod = cacheMod == (PyObject *)NULL ? PyImport_ImportModule(n_module) : cacheMod;
							if (PYTHON_IS_NOT_NONE(cacheMod)) {
								chkMod = pyxser_CacheModule((PyObject *)*modules, n_module);
								if (!PYTHON_IS_NOT_NONE(chkMod)) {
									PyDict_SetItemString((PyObject *)*modules, n_module, cacheMod);
								}
								currentType = (PyObject *)pyxser_SearchModuleType(cacheMod, n_type);
								if (PYTHON_IS_NOT_NONE(currentType)) {
									unser = PyInstance_NewRaw(currentType, PyDict_New());
									if (PYTHON_IS_NOT_NONE(unser)) {
										PyList_Append(list, unser);
										pyxser_AddAvailableObject((PyObject *)*dups, (char *)n_id, unser);
										cacheCurrent = *(obj->current);
										cacheCurrentNode = ron;
										*(obj->current) = unser;
										*(obj->currentNode) = ron;
										unser = pyxser_UnserializeBlock(obj);
										*(obj->current) = cacheCurrent;
										*(obj->currentNode) = cacheCurrentNode;
										ron = cacheCurrentNode;
									}
								}
							}
						} else {
							currentType = pyxser_SearchObjectInMain(n_type);
							if (PYTHON_IS_NOT_NONE(currentType)) {
								unser = PyInstance_NewRaw(currentType, PyDict_New());
								if (PYTHON_IS_NOT_NONE(unser)) {
									unser = PyInstance_NewRaw(currentType, PyDict_New());
									if (PYTHON_IS_NOT_NONE(unser)) {
										PyList_Append(list, unser);
										pyxser_AddAvailableObject((PyObject *)*dups, (char *)n_id, unser);
										cacheCurrent = *(obj->current);
										cacheCurrentNode = ron;
										*(obj->current) = unser;
										*(obj->currentNode) = ron;
										unser = pyxser_UnserializeBlock(obj);
										*(obj->current) = cacheCurrent;
										*(obj->currentNode) = cacheCurrentNode;
										ron = cacheCurrentNode;
									}
								}
							}
						}
					} else {
						PyList_Append(list, unser);
					}
				}
			}
		}
	}
	return list;
}

PyObject *
pyxunser_SerializeDict(PythonUnserializationArgumentsPtr obj)
{
	xmlNodePtr node = *(obj->currentNode);
	/* unserialization arguments */
	PyDictObject **dups = obj->dups;
	PyDictObject **modules = obj->modules;
	char *n_type = (char *)NULL;
	char *n_module = (char *)NULL;
	char *n_id = (char *)NULL;
	char *n_ref = (char *)NULL;
	PyObject *cacheMod = (PyObject *)NULL;
	PyObject *currentType = (PyObject *)NULL;
	PythonTypeDeserialize *machine = (PythonTypeDeserialize *)unserxConcreteTypes;
	PyObject *unser = (PyObject *)NULL;
	PyObject *cacheCurrent = (PyObject *)NULL;
	PyObject *chkMod = (PyObject *)NULL;
	xmlNodePtr ron = (xmlNodePtr)NULL;
	xmlNodePtr cacheCurrentNode = (xmlNodePtr)NULL;
	int c = 0, serialized = 0;
	PyObject *dict = (PyObject *)NULL;
	char *key = (char *)NULL;

	if (node != (xmlNodePtr)NULL) {
		dict = PyDict_New();
		for (ron = node->children;
			 ron;
			 ron = ron->next) {
			*(obj->currentNode) = ron;
			if (ron->type == XML_ELEMENT_NODE) {
				if ((strncmp((char *)ron->name,
							 (char *)pyxser_xml_element_item,
							 strlen((char *)pyxser_xml_element_item)))	== 0) {
					c = 0;
					serialized = 0;
					while (machine[c].available == 1
						   && serialized == 0) {
						if ((machine[c].check(ron))	== 1) {
							unser = machine[c].deserializer(obj);
							if (PYTHON_IS_NOT_NONE(unser)) {
								key = (char *)xmlGetProp(ron, BAD_CAST pyxser_xml_attr_key);
								PyDict_SetItemString(dict, key, unser);
								serialized = 1;
							}
						}
						c++;
					}
				} else if ((strncmp((char *)ron->name,
									(char *)pyxser_xml_element_object,
									strlen((char *)pyxser_xml_element_object))) == 0) {
					n_type = (char *)xmlGetProp(ron, BAD_CAST pyxser_xml_attr_type);
					n_module = (char *)xmlGetProp(ron, BAD_CAST pyxser_xml_attr_module);
					n_id = (char *)xmlGetProp(ron, BAD_CAST pyxser_xml_attr_id);
					n_ref = (char *)xmlGetProp(ron, BAD_CAST pyxser_xml_attr_ref);
					unser = pyxser_CheckAvailableObject((PyObject *)*dups, (char *)n_ref);
					if (n_type != (char *)NULL
						&& n_module != (char *)NULL
						&& n_id != (char *)NULL
						&& unser == (PyObject *)NULL) {
						if ((pyxser_ModuleNotMain(n_module)) == 1) {
							cacheMod = pyxser_SearchModule(n_module);
							cacheMod = cacheMod == (PyObject *)NULL ? PyImport_ImportModule(n_module) : cacheMod;
							if (PYTHON_IS_NOT_NONE(cacheMod)) {
								chkMod = pyxser_CacheModule((PyObject *)*modules, n_module);
								if (!PYTHON_IS_NOT_NONE(chkMod)) {
									PyDict_SetItemString((PyObject *)*modules, n_module, cacheMod);
								}
								currentType = (PyObject *)pyxser_SearchModuleType(cacheMod, n_type);
								if (PYTHON_IS_NOT_NONE(currentType)) {
									unser = PyInstance_NewRaw(currentType, PyDict_New());
									if (PYTHON_IS_NOT_NONE(unser)) {
										key = (char *)xmlGetProp(ron, BAD_CAST pyxser_xml_attr_key);
										PyDict_SetItemString(dict, key, unser);
										pyxser_AddAvailableObject((PyObject *)*dups, (char *)n_id, unser);
										cacheCurrent = *(obj->current);
										cacheCurrentNode = ron;
										*(obj->current) = unser;
										*(obj->currentNode) = ron;
										unser = pyxser_UnserializeBlock(obj);
										*(obj->current) = cacheCurrent;
										*(obj->currentNode) = cacheCurrentNode;
										ron = cacheCurrentNode;
									}
								}
							}
						} else {
							currentType = pyxser_SearchObjectInMain(n_type);
							if (PYTHON_IS_NOT_NONE(currentType)) {
								unser = PyInstance_NewRaw(currentType, PyDict_New());
								if (PYTHON_IS_NOT_NONE(unser)) {
									unser = PyInstance_NewRaw(currentType, PyDict_New());
									if (PYTHON_IS_NOT_NONE(unser)) {
										key = (char *)xmlGetProp(ron, BAD_CAST pyxser_xml_attr_key);
										PyDict_SetItemString(dict, key, unser);
										pyxser_AddAvailableObject((PyObject *)*dups, (char *)n_id, unser);
										cacheCurrent = *(obj->current);
										cacheCurrentNode = ron;
										*(obj->current) = unser;
										*(obj->currentNode) = ron;
										unser = pyxser_UnserializeBlock(obj);
										*(obj->current) = cacheCurrent;
										*(obj->currentNode) = cacheCurrentNode;
										ron = cacheCurrentNode;
									}
								}
							}
						}
					} else {
						key = (char *)xmlGetProp(ron, BAD_CAST pyxser_xml_attr_key);
						PyDict_SetItemString(dict, key, unser);
					}
				}
			}
		}
	}
	return dict;
}

/* pyserx_collections.c ends here */

