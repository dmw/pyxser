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
## $Id: test-utf8-prof.py 76 2009-04-26 21:34:09Z damowe $
##


import sys
import traceback

import pyxser
import testpkg.sample

def display_heap(hp):
    print hp
    c = 10
    more = hp.more
    while c <= len(hp):
        print more
        more = more.more
        c += 10

if __name__ == "__main__":
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
        crawl = 0
        while crawl <= 5000:
            try:
                serialized = pyxser.serialize(obj = test, enc = "utf-8", depth = 2)
                pyxser.validate(obj = serialized, enc = "utf-8")
                unserialized = pyxser.unserialize(obj = serialized, enc = "utf-8")
                repr(unserialized);

                serialized = pyxser.serialize(obj = test, enc = "utf-8", depth = 3)
                pyxser.validate(obj = serialized, enc = "utf-8")
                unserialized = pyxser.unserialize(obj = serialized, enc = "utf-8")
                repr(unserialized)

                serialized = pyxser.serialize(obj = test, enc = "utf-8", depth = 4)
                pyxser.validate(obj = serialized, enc = "utf-8")
                unserialized = pyxser.unserialize(obj = serialized, enc = "utf-8")
                repr(unserialized)

                serialized = pyxser.serialize(obj = test, enc = "utf-8", depth = 5)
                pyxser.validate(obj = serialized, enc = "utf-8")
                unserialized = pyxser.unserialize(obj = serialized, enc = "utf-8")
                repr(unserialized)

                serialized = pyxser.serialize(obj = test, enc = "utf-8", depth = 0)
                pyxser.validate(obj = serialized, enc = "utf-8")
                unserialized = pyxser.unserialize(obj = serialized, enc = "utf-8")
                repr(unserialized)

                pyxser.xmlcleanup()

            except Exception, e:
                print "-" * 60
                traceback.print_exc(file=sys.stdout)
                print "-" * 60

            try:
                serialized = pyxser.serialize_c14n(obj = test, depth = 0, exc = 0, com = 0)
                pyxser.validate_c14n(obj = serialized, enc = "utf-8")
                unserialized = pyxser.unserialize(obj = serialized, enc = "utf-8")
                repr(unserialized)

                serialized = pyxser.serialize_c14n_strict(obj = test, depth = 0, exc = 0, com = 0)
                pyxser.validate_c14n(obj = serialized, enc = "utf-8")
                
                pyxser.xmlcleanup()

            except Exception, e:
                print "-" * 60
                traceback.print_exc(file=sys.stdout)
                print "-" * 60

            try:
                serialized = pyxser.u_serialize(obj = test, enc = "utf-8", depth = 2)
                pyxser.u_validate(obj = serialized, enc = "utf-8")
                unserialized = pyxser.u_unserialize(obj = serialized, enc = "utf-8")
                repr(unserialized)

                serialized = pyxser.u_serialize(obj = test, enc = "utf-8", depth = 3)
                pyxser.u_validate(obj = serialized, enc = "utf-8")
                unserialized = pyxser.u_unserialize(obj = serialized, enc = "utf-8")
                repr(unserialized)

                serialized = pyxser.u_serialize(obj = test, enc = "utf-8", depth = 4)
                pyxser.u_validate(obj = serialized, enc = "utf-8")
                unserialized = pyxser.u_unserialize(obj = serialized, enc = "utf-8")
                repr(unserialized)

                serialized = pyxser.u_serialize(obj = test, enc = "utf-8", depth = 5)
                pyxser.u_validate(obj = serialized, enc = "utf-8")
                unserialized = pyxser.u_unserialize(obj = serialized, enc = "utf-8")
                repr(unserialized)

                serialized = pyxser.u_serialize(obj = test, enc = "utf-8", depth = 0)
                pyxser.u_validate(obj = serialized, enc = "utf-8")
                unserialized = pyxser.u_unserialize(obj = serialized, enc = "utf-8")
                repr(unserialized)

                pyxser.xmlcleanup()

            except Exception, e:
                print "-" * 60
                traceback.print_exc(file=sys.stdout)
                print "-" * 60

            try:
                serialized = pyxser.u_serialize_c14n(obj = test, depth = 0, exc = 0, com = 0)
                pyxser.u_validate(obj = serialized, enc = "utf-8")
                unserialized = pyxser.u_unserialize(obj = serialized, enc = "utf-8")
                repr(unserialized)

                serialized = pyxser.u_serialize_c14n_strict(obj = test, depth = 0, exc = 0, com = 0)
                pyxser.u_validate_c14n(obj = serialized, enc = "utf-8")

                pyxser.xmlcleanup()

            except Exception, e:
                print "-" * 60
                traceback.print_exc(file=sys.stdout)
                print "-" * 60

            pyxser.getdtd()
            pyxser.getdtd_c14n()
            pyxser.xmlcleanup()

            if (crawl % 1000) == 0:
                print "cicle: ", crawl
            crawl += 1

    except Exception, e:
        print "-" * 60
        traceback.print_exc(file=sys.stdout)
        print "-" * 60

