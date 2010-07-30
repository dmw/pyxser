#!/usr/bin/env python
#
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

type_map = { '__builtin__': bi_converter,
             'weakref': bi_converter,
             'set': bi_converter
             }

def main():
    engine = create_engine('sqlite:///:memory:', echo=True)
    metadata = MetaData()
    metadata.create_all(engine)
    session = sessionmaker(bind=engine)
    ed_user = s.User('ed', 'Ed Jones', 'edspassword')
    serialized = pyxser.serialize(obj = ed_user, enc = 'utf-8')
    print serialized
    ed_unser = pyxser.unserialize(obj = serialized, enc = "utf-8", typemap = type_map)
    print repr(ed_unser)

if __name__ == "__main__":
    main()

