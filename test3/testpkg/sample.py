
### $Id$

__all__ = [
    'SubNestedChild',
    'NestedChild',
    'ChildObject',
    'ParentObject',
    'TestAnotherObject']

from sqlalchemy import Table, Column, Integer, String, MetaData, ForeignKey
from sqlalchemy.orm import mapper
from sqlalchemy.ext.declarative import declarative_base
from sqlalchemy.orm import sessionmaker
from sqlalchemy import create_engine

Base = declarative_base()

class TestAnotherObject:
    first_element = "123"
    second_element = "456"
    def __str__(self):
        return repr(self.__dict__)
    def __repr__(self):
        return repr(self.__dict__)

class SubNestedChild:
    subnested1 = None
    def __init__(self, m1):
        self.subnested1 = m1
    def __repr__(self):
        return "\n\t\t\t-> " + repr(self.__dict__)

class NestedChild:
    nested1 = None
    nested2 = None
    nested3 = None
    nested4 = None
    def __init__(self, m1, m2, m3):
        self.nested1 = m1
        self.nested2 = m2
        self.nested3 = m3
    def __repr__(self):
        return "\n\t\t-> " + repr(self.__dict__)

class ChildObject:
    child1 = None
    child2 = None
    child3 = None
    child4 = None
    def __init__(self, m1, m2, m3):
        self.child1 = m1
        self.child2 = m2
        self.child3 = m3
    def __repr__(self):
        return "\n\t-> " + repr(self.__dict__)

class ParentObject:
    parent1 = None
    parent2 = None
    parent3 = None
    def __init__(self, m1, m2, m3):
        self.parent1 = m1
        self.parent2 = m2
    def child(self, m1, m2, m3):
        self.parent3 = ChildObject(m1, m2, m3)
    def nested(self, m1, m2, m3):
        self.parent3.child4 = NestedChild(m1, m2, m3)
    def subnested(self, m1):
        self.parent3.child4.nested4 = SubNestedChild(m1)
    def __repr__(self):
        return "\n-> " + repr(self.__dict__)

class User(Base):
    __tablename__ = 'users'
    id = Column(Integer, primary_key=True)
    name = Column(String)
    fullname = Column(String)
    password = Column(String)

    def __init__(self):
        self.name = None
        self.fullname = None
        self.password = None

    def get_set(self):
        return (self.name, self.fullname, self.password)

    def __repr__(self):
        return "<User('%s','%s', '%s')>" % self.get_set()
