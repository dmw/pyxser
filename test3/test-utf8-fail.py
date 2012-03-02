#!/usr/bin/env python
# -*- coding: utf-8 -*-
##
##


import pyxser

class ContainerClass(object):

    def __init__(self):
        self.dummy = True

    def add(self, ob):
        if not hasattr(self, 'embedded'):
            self.embedded = list()
        self.embedded.append(ob)


def main():
    xml_str = '<?xml version="1.0" encoding="utf-8"?>' \
              + '<pyxs:obj xmlns:pyxs="http://projects.coder.cl/pyxser/model/" ' \
              + 'version="1.0" type="ContainerClass" module="__main__" '\
              + 'objid="id5642896">' \
              + '<pyxs:prop type="bool" name="dummy">True</pyxs:prop>' \
              + '<pyxs:col type="list" name="embedded">' \
              + '<pyxs:obj type="object" module="__builtin__" ' \
              + 'objid="id1488096"/>' \
              + '<pyxs:obj type="object" module="__builtin__" ' \
              + 'objid="id1488056"/>' \
              + '</pyxs:col></pyxs:obj>'
    unser = pyxser.unserialize(obj = xml_str, enc='utf-8')
    print repr(unser)
    print repr(unser.__dict__)


if __name__ == "__main__":
    main()
