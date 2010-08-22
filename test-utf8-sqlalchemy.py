#!/usr/bin/env python
# -*- coding: utf-8; -*-
#


import pyxser
import sys
import traceback
import testpkg.sample as s
import inspect
from sqlalchemy import *
from sqlalchemy.orm import *
from sqlalchemy.ext.declarative import *
from sqlalchemy.orm.state import *
from sqlalchemy.orm.attributes import *
from sqlalchemy.orm.properties import *

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
    engine = create_engine('sqlite:///:memory:', echo=True)
    metadata = MetaData()
    metadata.create_all(engine)
    session = sessionmaker(bind=engine)
    ed_user = s.User()
    ed_user.fullname = 'Ed Jones'
    ed_user.password = 'password'
    ed_user.name = 'ed'
    serialized = pyxser.serialize(obj = ed_user, enc = 'ascii')
    print serialized
    ed_unser = pyxser.unserialize(obj = serialized, enc = "ascii")
    print repr(ed_unser)
    print repr(ed_unser.get_set())

if __name__ == "__main__":
    main()

