#!/usr/bin/env python
# -*- coding: utf-8; -*-
#


import pyxser
import sys
import traceback
import testpkg.sample as s
import inspect
import sqlalchemy
import sqlalchemy.orm

def bi_converter(o):
    return str("");

def bi_filter(v):
    r = ((not callable(v[1])) \
         and (not (v[0].startswith("_"))) \
         and ((v[0] != "metadata")))
    return r

def bi_selector(o):
    r = dict(filter(bi_filter, inspect.getmembers(o)))
    return r

def main():
    engine = sqlalchemy.create_engine('sqlite:///:memory:', echo=True)
    metadata = sqlalchemy.MetaData()
    metadata.create_all(engine)
    session = sqlalchemy.orm.sessionmaker(bind=engine)
    ed_user = s.User()
    ed_user.fullname = 'Ed Jones'
    ed_user.password = 'password'
    ed_user.name = 'ed'
    serialized = pyxser.serialize(obj = ed_user, enc = 'iso-8859-1', depth = 100)
    print serialized
    ed_unser = pyxser.unserialize(obj = serialized, enc = "iso-8859-1")
    print repr(ed_unser)
    print repr(ed_unser.get_set())

if __name__ == "__main__":
    main()

