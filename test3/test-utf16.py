#!/usr/bin/env python
# -*- coding: utf-8 -*-
##
##     Copyright (c) 2009 Daniel Molina Wegener <dmw@coder.cl>
##
##     This file is part of pyxser.
##
##     pyxser is free software: you can redistribute it and/or modify
##     it under the terms of the GNU Lesser General Public License as
##     published by the Free Software Foundation, either version 3 of
##     the License, or (at your option) any later version.
##
##     pyxser is distributed in the hope that it will be useful,
##     but WITHOUT ANY WARRANTY; without even the implied warranty of
##     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
##     GNU General Public License for more details.
##
##     You should have received a copy of the GNU General Public License
##     along with pyxser.  If not, see <http://www.gnu.org/licenses/>.
##
##     <!DOCTYPE pyxs:object
##               PUBLIC "-//coder.cl//DTD pyxser 1.0//EN"
##               "http://projects.coder.cl/pyxser/dtd/pyxser-1.0.dtd">
##
## $Id$
##


import sys
import traceback

import pyxser
import testpkg.sample
import inspect

def sel_filter(v):
    r = ((not callable(v[1])) and (not v[0].startswith("__")))
    return r

def sel(o):
    print repr(o)
    r = dict(filter(sel_filter, inspect.getmembers(o)))
    print repr(r)
    return r

class TestTea():
    prop_x = None
    def __init__(self, w):
        self.prop_x = w


def display_heap(hp):
    print hp
    c = 10
    more = hp.more
    while c <= len(hp):
        print more
        more = more.more
        c += 10

def fallback_to_string(o):
    try:
        return str(o)
    except:
        return ""

test_typemap_map = { 'str': fallback_to_string }

def test_normal(test):
    try:
        print "-----8<----------8<----------8<----------8<----------8<----------8<----- test_normal()"
        print "First Object:\n" + repr(test) + "\n\n"

        serialized = pyxser.serialize(obj = test, enc = "utf-16", depth = 2)
        print "Serilized Object:\n", serialized
        print "Serilized Object Validation:\n", \
              pyxser.validate(obj = serialized, enc = "utf-16"), \
              pyxser.validate_dtd(obj = serialized, enc = "utf-16")
        unserialized = pyxser.unserialize(obj = serialized, enc = "utf-16")
        print "Unserialized Object (d.2):\n" + repr(unserialized) + "\n\n"

        serialized = pyxser.serialize(obj = test, enc = "utf-16", depth = 3)
        print "Serilized Object:\n" + serialized
        print "Serilized Object Validation:\n", \
              pyxser.validate(obj = serialized, enc = "utf-16"), \
              pyxser.validate_dtd(obj = serialized, enc = "utf-16")
        unserialized = pyxser.unserialize(obj = serialized, enc = "utf-16")
        print "Unserialized Object (d.3):\n" + repr(unserialized) + "\n\n"

        serialized = pyxser.serialize(obj = test, enc = "utf-16", depth = 4)
        print "Serilized Object:\n" + serialized
        print "Serilized Object Validation:\n", \
              pyxser.validate(obj = serialized, enc = "utf-16"), \
              pyxser.validate_dtd(obj = serialized, enc = "utf-16")
        unserialized = pyxser.unserialize(obj = serialized, enc = "utf-16")
        print "Unserialized Object (d.4):\n" + repr(unserialized) + "\n\n"

        serialized = pyxser.serialize(obj = test, enc = "utf-16", depth = 5)
        print "Serilized Object:\n" + serialized
        print "Serilized Object Validation:\n", \
              pyxser.validate(obj = serialized, enc = "utf-16"), \
              pyxser.validate_dtd(obj = serialized, enc = "utf-16")
        unserialized = pyxser.unserialize(obj = serialized, enc = "utf-16")
        print "Unserialized Object (d.5):\n" + repr(unserialized) + "\n\n"

        serialized = pyxser.serialize(obj = test, enc = "utf-16", depth = 0)
        print "Serilized Object:\n" + serialized
        print "Serilized Object Validation:\n", \
              pyxser.validate(obj = serialized, enc = "utf-16"), \
              pyxser.validate_dtd(obj = serialized, enc = "utf-16")
        unserialized = pyxser.unserialize(obj = serialized, enc = "utf-16", cinit = False)
        print "Unserialized Object (d.6):\n" + repr(unserialized) + "\n\n"

        pyxser.xmlcleanup()

    except Exception, e:
        print "-" * 60
        print e
        traceback.print_exc(file=sys.stdout)
        print "-" * 60

def test_selector(test):
    try:
        print "-----8<----------8<----------8<----------8<----------8<----------8<----- test_selector()"
        print "First Object:\n" + repr(test) + "\n\n"

        x = sel(test)
        print repr(x)

        serialized = pyxser.serialize(obj = test, enc = "utf-16", depth = 2, selector = sel)
        print "Serilized Object:\n", serialized
        print "Serilized Object Validation:\n", \
              pyxser.validate(obj = serialized, enc = "utf-16"), \
              pyxser.validate_dtd(obj = serialized, enc = "utf-16")
        unserialized = pyxser.unserialize(obj = serialized, enc = "utf-16")
        print "Unserialized Object (d.2):\n" + repr(unserialized) + "\n\n"

        serialized = pyxser.serialize(obj = test, enc = "utf-16", depth = 3, selector = sel)
        print "Serilized Object:\n" + serialized
        print "Serilized Object Validation:\n", \
              pyxser.validate(obj = serialized, enc = "utf-16"), \
              pyxser.validate_dtd(obj = serialized, enc = "utf-16")
        unserialized = pyxser.unserialize(obj = serialized, enc = "utf-16")
        print "Unserialized Object (d.3):\n" + repr(unserialized) + "\n\n"

        serialized = pyxser.serialize(obj = test, enc = "utf-16", depth = 4, selector = sel)
        print "Serilized Object:\n" + serialized
        print "Serilized Object Validation:\n", \
              pyxser.validate(obj = serialized, enc = "utf-16"), \
              pyxser.validate_dtd(obj = serialized, enc = "utf-16")
        unserialized = pyxser.unserialize(obj = serialized, enc = "utf-16")
        print "Unserialized Object (d.4):\n" + repr(unserialized) + "\n\n"

        serialized = pyxser.serialize(obj = test, enc = "utf-16", depth = 5, selector = sel)
        print "Serilized Object:\n" + serialized
        print "Serilized Object Validation:\n", \
              pyxser.validate(obj = serialized, enc = "utf-16"), \
              pyxser.validate_dtd(obj = serialized, enc = "utf-16")
        unserialized = pyxser.unserialize(obj = serialized, enc = "utf-16")
        print "Unserialized Object (d.5):\n" + repr(unserialized) + "\n\n"

        serialized = pyxser.serialize(obj = test, enc = "utf-16", depth = 0, selector = sel)
        print "Serilized Object:\n" + serialized
        print "Serilized Object Validation:\n", \
              pyxser.validate(obj = serialized, enc = "utf-16"), \
              pyxser.validate_dtd(obj = serialized, enc = "utf-16")
        unserialized = pyxser.unserialize(obj = serialized, enc = "utf-16", cinit = False)
        print "Unserialized Object (d.0):\n" + repr(unserialized) + "\n\n"

        pyxser.xmlcleanup()

    except Exception, e:
        print "-" * 60
        print e
        traceback.print_exc(file=sys.stdout)
        print "-" * 60

def test_normal_c14n(test):
    try:
        print "-----8<----------8<----------8<----------8<----------8<----------8<----- test_normal_c14n()"
        print "First Object:\n" + repr(test) + "\n\n"
        serialized = pyxser.serialize_c14n(obj = test, depth = 0, com = 0)
        print "Serilized Object:\n" + serialized
        print "First Object:\n" + repr(test) + "\n\n"
        print "Serilized Object Validation:\n", \
              pyxser.validate_c14n(obj = serialized, enc = "utf-16"), \
              pyxser.validate_c14n_dtd(obj = serialized, enc = "utf-16")
        unserialized = pyxser.unserialize(obj = serialized, enc = "utf-16")
        print "Unserialized Object:\n" + repr(unserialized) + "\n\n"

        serialized = pyxser.serialize_c14n_strict(obj = test, depth = 0, com = 0)
        print "Serilized Object:\n" + serialized
        print "Serilized Object Validation:\n", pyxser.validate_c14n(obj = serialized, enc = "utf-16")
        print "First Object:\n" + repr(test) + "\n\n"

        pyxser.xmlcleanup()

    except Exception, e:
        print "-" * 60
        traceback.print_exc(file=sys.stdout)
        print "-" * 60

def test_unicode(test):
    try:
        print "-----8<----------8<----------8<----------8<----------8<----------8<----- test_unicode()"
        print "First Object:\n" + repr(test) + "\n\n"

        serialized = pyxser.u_serialize(obj = test, enc = "utf-16", depth = 2)
        print "Serilized Object:\n", serialized.encode("utf-16")
        print "Serilized Object Validation:\n", \
              pyxser.u_validate(obj = serialized, enc = "utf-16"), \
              pyxser.u_validate_dtd(obj = serialized, enc = "utf-16")
        unserialized = pyxser.u_unserialize(obj = serialized, enc = "utf-16")
        print "Unserialized Object (d.2):\n" + repr(unserialized) + "\n\n"

        serialized = pyxser.u_serialize(obj = test, enc = "utf-16", depth = 3)
        print "Serilized Object:\n" + serialized.encode("utf-16")
        print "Serilized Object Validation:\n", \
              pyxser.u_validate(obj = serialized, enc = "utf-16"), \
              pyxser.u_validate_dtd(obj = serialized, enc = "utf-16")
        unserialized = pyxser.u_unserialize(obj = serialized, enc = "utf-16")
        print "Unserialized Object (d.3):\n" + repr(unserialized) + "\n\n"

        serialized = pyxser.u_serialize(obj = test, enc = "utf-16", depth = 4)
        print "Serilized Object:\n" + serialized.encode("utf-16")
        print "Serilized Object Validation:\n", \
              pyxser.u_validate(obj = serialized, enc = "utf-16"), \
              pyxser.u_validate_dtd(obj = serialized, enc = "utf-16")
        unserialized = pyxser.u_unserialize(obj = serialized, enc = "utf-16")
        print "Unserialized Object (d.4):\n" + repr(unserialized) + "\n\n"

        serialized = pyxser.u_serialize(obj = test, enc = "utf-16", depth = 5)
        print "Serilized Object:\n" + serialized.encode("utf-16")
        print "Serilized Object Validation:\n", \
              pyxser.u_validate(obj = serialized, enc = "utf-16"), \
              pyxser.u_validate_dtd(obj = serialized, enc = "utf-16")
        unserialized = pyxser.u_unserialize(obj = serialized, enc = "utf-16")
        print "Unserialized Object (d.5):\n" + repr(unserialized) + "\n\n"

        serialized = pyxser.u_serialize(obj = test, enc = "utf-16", depth = 0)
        print "Serilized Object:\n" + serialized.encode("utf-16")
        print "Serilized Object Validation:\n", \
              pyxser.u_validate(obj = serialized, enc = "utf-16"), \
              pyxser.u_validate_dtd(obj = serialized, enc = "utf-16")
        unserialized = pyxser.u_unserialize(obj = serialized, enc = "utf-16", cinit = False)
        print "Unserialized Object (d.0):\n" + repr(unserialized) + "\n\n"

        pyxser.xmlcleanup()

    except Exception, e:
        print "-" * 60
        traceback.print_exc(file=sys.stdout)
        print "-" * 60

def test_unicode_c14n(test):
    try:
        print "-----8<----------8<----------8<----------8<----------8<----------8<----- test_unicode_c14n()"
        serialized = pyxser.u_serialize_c14n(obj = test, depth = 0, com = 1)
        print "Serilized Object:\n" + serialized.encode("utf-16")
        print "First Object:\n" + repr(test) + "\n\n"
        print "Serilized Object Validation:\n", \
              pyxser.u_validate_c14n(obj = serialized, enc = "utf-16"), \
              pyxser.u_validate_c14n_dtd(obj = serialized, enc = "utf-16")

        unserialized = pyxser.u_unserialize(obj = serialized, enc = "utf-16")
        print "Unserialized Object:\n" + repr(unserialized) + "\n\n"

        serialized = pyxser.u_serialize_c14n_strict(obj = test, depth = 0, com = 0)
        print "Serilized Object:\n" + serialized.encode("utf-16")
        print "Serilized Object Validation:\n", pyxser.u_validate_c14n(obj = serialized, enc = "utf-16")
        print "First Object:\n" + repr(test) + "\n\n"

        pyxser.xmlcleanup()

    except Exception, e:
        print "-" * 60
        traceback.print_exc(file=sys.stdout)
        print "-" * 60


def test_typemap(test):
    try:
        print "-----8<----------8<----------8<----------8<----------8<----------8<----- test_typemap()"
        print "First Object:\n" + repr(test) + "\n\n"

        serialized = pyxser.serialize(obj = test, enc = "utf-16", depth = 2, typemap = test_typemap_map)
        print "Serilized Object:\n", serialized
        print "Serilized Object Validation:\n", \
              pyxser.validate(obj = serialized, enc = "utf-16"), \
              pyxser.validate_dtd(obj = serialized, enc = "utf-16")
        unserialized = pyxser.unserialize(obj = serialized, enc = "utf-16", typemap = test_typemap_map)
        print "Unserialized Object (d.2):\n" + repr(unserialized) + "\n\n"

        serialized = pyxser.serialize(obj = test, enc = "utf-16", depth = 3, typemap = test_typemap_map)
        print "Serilized Object:\n" + serialized
        print "Serilized Object Validation:\n", \
              pyxser.validate(obj = serialized, enc = "utf-16"), \
              pyxser.validate_dtd(obj = serialized, enc = "utf-16")
        unserialized = pyxser.unserialize(obj = serialized, enc = "utf-16", typemap = test_typemap_map)
        print "Unserialized Object (d.3):\n" + repr(unserialized) + "\n\n"

        serialized = pyxser.serialize(obj = test, enc = "utf-16", depth = 4, typemap = test_typemap_map)
        print "Serilized Object:\n" + serialized
        print "Serilized Object Validation:\n", \
              pyxser.validate(obj = serialized, enc = "utf-16"), \
              pyxser.validate_dtd(obj = serialized, enc = "utf-16")
        unserialized = pyxser.unserialize(obj = serialized, enc = "utf-16", typemap = test_typemap_map)
        print "Unserialized Object (d.4):\n" + repr(unserialized) + "\n\n"

        serialized = pyxser.serialize(obj = test, enc = "utf-16", depth = 5, typemap = test_typemap_map)
        print "Serilized Object:\n" + serialized
        print "Serilized Object Validation:\n", \
              pyxser.validate(obj = serialized, enc = "utf-16"), \
              pyxser.validate_dtd(obj = serialized, enc = "utf-16")
        unserialized = pyxser.unserialize(obj = serialized, enc = "utf-16", typemap = test_typemap_map)
        print "Unserialized Object (d.5):\n" + repr(unserialized) + "\n\n"

        serialized = pyxser.serialize(obj = test, enc = "utf-16", depth = 0, typemap = test_typemap_map)
        print "Serilized Object:\n" + serialized
        print "Serilized Object Validation:\n", \
              pyxser.validate(obj = serialized, enc = "utf-16"), \
              pyxser.validate_dtd(obj = serialized, enc = "utf-16")
        unserialized = pyxser.unserialize(obj = serialized, enc = "utf-16", typemap = test_typemap_map)
        print "Unserialized Object (d.0):\n" + repr(unserialized) + "\n\n"

        pyxser.xmlcleanup()

    except Exception, e:
        print "-" * 60
        print e
        traceback.print_exc(file=sys.stdout)
        print "-" * 60

def main():

    another = testpkg.sample.TestAnotherObject()
    another.first_element = 123
    another.second_element = 456

    other = testpkg.sample.TestAnotherObject()
    other.first_element = complex(5.82, 3.99)
    other.second_element = "cdf"

    thisa = testpkg.sample.TestAnotherObject()
    thisa.first_element = "xyz"
    thisa.second_element = complex(7.81, 2.01)

    thisb = testpkg.sample.TestAnotherObject()
    thisb.first_element = "456"
    thisb.second_element = "789"

    thisc = testpkg.sample.TestAnotherObject()
    thisc.first_element = "XXX"
    thisc.second_element = complex(32.1, 33.1)

    test = testpkg.sample.ParentObject("hola", "chao", 2354345L)
    test.child("hi", 4.5, 2354)
    test.nested("holahola", 345, "hola")
    test.subnested(other)
    another.dyn_prop1 = thisa
    test.dyn_prop1 = [u'holá', u'chaó', another]
    test.dyn_prop2 = (u'hol`', u'sïn', 'trip', other)
    test.dyn_prop3 = {u'sáludó1': u'hólà',
                      u'sáludó2': u'chäó',
                      u'sòludò4': u'goodbye',
                      'saludo5': thisc}
    test.dyn_prop4 = u'sómé tèxtè ïñ Unicodè'
    test.dyn_prop5 = u'Añother Texé Iñ ÜnìcóDËc'
    test.dyn_prop6 = 1.5
    test.dyn_prop7 = 1000

    test_normal(test)
    test_normal_c14n(test)
    test_unicode(test)
    test_unicode_c14n(test)
    test_typemap(test)
    test_selector(test)

    print pyxser.getdtd()
    print pyxser.getdtd_c14n()
    print pyxser.getxsd()
    print pyxser.getxsd_c14n()

    pyxser.xmlcleanup()

if __name__ == "__main__":
    main()

