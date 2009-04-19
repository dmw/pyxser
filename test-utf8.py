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
from guppy import hpy

def display_heap(hp):
    print hp
    c = 10
    more = hp.more
    while c <= len(hp):
        print more
        more = more.more
        c += 10


if __name__ == "__main__":
    h = hpy()
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
    test.dyn_prop3 = {'saludo1': u'hólà', 'saludo2': u'chäó', 'saludo4': 'goodbye', 'saludo5': thisc}
    test.dyn_prop4 = u'sómé tèxtè ïñ Unicodè'
    test.dyn_prop5 = u'Añother Texé Iñ ÜnìcóDËc'
    test.dyn_prop6 = 1.5
    test.dyn_prop7 = 1000

    try:
        print "First Object:\n" + repr(test) + "\n\n"

        serialized = pyxser.serialize(obj = test, enc = "utf-8", depth = 2)
        print "Serilized Object:\n", serialized.encode("latin1");
        print "Serilized Object Validation:\n", pyxser.validate(serialized, enc = "utf-8")
        unserialized = pyxser.unserialize(obj = serialized, enc = "utf-8")
        print "Unserialized Object (d.2):\n" + repr(unserialized) + "\n\n"

        serialized = pyxser.serialize(obj = test, enc = "utf-8", depth = 3)
        print "Serilized Object:\n" + serialized.encode("latin1");
        print "Serilized Object Validation:\n", pyxser.validate(serialized, enc = "utf-8")
        unserialized = pyxser.unserialize(obj = serialized, enc = "utf-8")
        print "Unserialized Object (d.3):\n" + repr(unserialized) + "\n\n"

        serialized = pyxser.serialize(obj = test, enc = "utf-8", depth = 4)
        print "Serilized Object:\n" + serialized.encode("latin1")
        print "Serilized Object Validation:\n", pyxser.validate(serialized, enc = "utf-8")
        unserialized = pyxser.unserialize(obj = serialized, enc = "utf-8")
        print "Unserialized Object (d.4):\n" + repr(unserialized) + "\n\n"

        serialized = pyxser.serialize(obj = test, enc = "utf-8", depth = 5)
        print "Serilized Object:\n" + serialized.encode("latin1")
        print "Serilized Object Validation:\n", pyxser.validate(serialized, enc = "utf-8")
        unserialized = pyxser.unserialize(obj = serialized, enc = "utf-8")
        print "Unserialized Object (d.5):\n" + repr(unserialized) + "\n\n"

        serialized = pyxser.serialize(obj = test, enc = "utf-8", depth = 0)
        print "Serilized Object:\n" + serialized.encode("latin1")
        print "Serilized Object Validation:\n", pyxser.validate(serialized, enc = "utf-8")
        unserialized = pyxser.unserialize(obj = serialized, enc = "utf-8")
        print "Unserialized Object (d.0):\n" + repr(unserialized) + "\n\n"

        pyxser.xmlcleanup()

    except Exception, e:
        print "-" * 60
        traceback.print_exc(file=sys.stdout)
        print "-" * 60

    try:
        print "---8<--- C14N ---8<---"
        serialized = pyxser.serialize_c14n(obj = test, depth = 0, exc = 0, com = 0)
        print "Serilized Object:\n" + serialized.encode("latin1")
        print "First Object:\n" + repr(test) + "\n\n"
        print "Serilized Object Validation:\n", pyxser.validate(obj = serialized, enc = "utf-8")
        unserialized = pyxser.unserialize(obj = serialized, enc = "utf-8")
        print "Unserialized Object:\n" + repr(unserialized) + "\n\n"

        serialized = pyxser.serialize_c14n_strict(obj = test, depth = 0, exc = 0, com = 0)
        print "Serilized Object:\n" + serialized.encode("latin1")
        print "Serilized Object Validation:\n", pyxser.validate_c14n(obj = serialized, enc = "utf-8")
        print "First Object:\n" + repr(test) + "\n\n"

        pyxser.xmlcleanup()

    except Exception, e:
        print "-" * 60
        traceback.print_exc(file=sys.stdout)
        print "-" * 60

    print pyxser.getdtd()
    print pyxser.getdtd_c14n()

    hps = h.heapu()
    display_heap(hps)




