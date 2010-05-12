#!/usr/bin/env python
#
#

from sqlalchemy import Table, Column, Integer, String, MetaData, ForeignKey
from sqlalchemy.orm import mapper
from sqlalchemy.ext.declarative import declarative_base
from sqlalchemy.orm import sessionmaker
from sqlalchemy import create_engine
import pyxser

Base = declarative_base()

class User(Base):
    __tablename__ = 'users'
    id = Column(Integer, primary_key=True)
    name = Column(String)
    fullname = Column(String)
    password = Column(String)

    def __init__(self, name, fullname, password):
        self.name = name
        self.fullname = fullname
        self.password = password

    def get_set(self):
        return (self.name, self.fullname, self.password)

    def __repr__(self):
        return "<User('%s','%s', '%s')>" % self.get_set()

def main():
    engine = create_engine('sqlite:///:memory:', echo=True)
    metadata = MetaData()
    metadata.create_all(engine)
    session = sessionmaker(bind=engine)
    ed_user = User('ed', 'Ed Jones', 'edspassword')
    ed_ser = pyxser.serialize(obj = ed_user, enc = 'utf-8', depth = 0)
    print ed_ser
    ed_unser = pyxser.unserialize(obj = ed_ser, enc = "utf-8")
    print repr(ed_unser)

if __name__ == "__main__":
    main()

