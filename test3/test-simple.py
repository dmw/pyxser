#!/usr/bin/env python3.2
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


import pyxser
import testpkg.sample

if __name__ == "__main__":
    another = testpkg.sample.TestAnotherObject()
    another.first_element = '123'
    another.second_element = '456'
    another.third_element = ['hola', 'como', 'estas']
    another.fourth_element = ('hola', 'como', 'estas',)
    another.fifth_element = {'hola': 'chao', 'hi': 'bye'}

    ser = pyxser.serialize(obj = another, enc = 'utf-8', depth = 100)
    print(ser)

    pyxser.xmlcleanup()

