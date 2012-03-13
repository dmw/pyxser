
pyxser, Python XML Serialization
===

The pyxser name stands for Python XML Serialization and is a
Python object to XML serializer and deserializer. In other words,
it can convert a Python object into XML and also, convert that XML
back into the original Python object. It uses a preorder traversal
tree algorithm over the object tree and the XML tree. The algorithm
is O(n), and uses some compiler construction techniques as memoization
to create unique instances of each element in the object tree. It
supports cross referenced and circular referenced object trees, so
the serializer do not crash with those kinds of objects. Each
serialized object before being deserialized is validated against
the pyxser XML schema. It also uses mutually recursive functions
to serialize and deserialize the object tree, but it has some
features, like the depth argument, which allows you to stop
serializing or deserializing the object tree at certain point.
The pyxser module is self documented, if you want to take a
look on the documentation, just run pydoc pyxser. The current
version is 1.5.2-r2. It supports Python 2.4 to Python 2.7 and is
available through easy_install.

A sample of a serialized object is the XML as follows:

```xml
<?xml version="1.0" encoding="utf-8"?>
<pyxs:obj xmlns:pyxs="http://projects.coder.cl/pyxser/model/"
          version="1.0"
          type="TestAnotherObject"
          module="testpkg.sample"
          id="id-136688140">
  <pyxs:prop type="str" name="first_element">456</pyxs:prop>
  <pyxs:prop type="str" name="second_element">789</pyxs:prop>
</pyxs:obj>
```

This project pyxser supports cross referenced objects and nested
referenced object, because it defines the a ref attribute as IDREF
which is used to link internal nodes in the document tree.

* Serialization of cross references.
* Serialization of circular references.
* Preserves object references.
* Custom serializations.
* Custom deserializations.
* Custom serialization depth limit.
* Object attribute selection callback.
* Standards based serialization.
* Standards based XML validation using pyxser XML Schema.
* C14N based serialization, as optional kind of output.
* Model based XML serialization, represented on XML Schema and XML DTD.


You should see the INSTALLING file to see installing instructions.
Also you can install it using pip:

```shell

pip install pyxser
pydoc pyxser

```

Or easy_install if you want:

```shell

easy_install pyxser
pydoc pyxser

```

Once you have installed pyxser, you can start serializing objects
using code similar to this:

```python

import pyxser as pyx

class TestClass(object):
    a = None
    b = None
    c = None

    def __init__(self, a, b, c):
        self.a = a
        self.b = b
        self.c = c

tst = TestClass("var_a", "var_b", "var_c")
ser = pyx.serialize(obj=tst, enc="utf-8")
print(ser)

```

This will give you an output similar to this one:

```xml
<?xml version="1.0" encoding="utf-8"?>
<pyxs:obj xmlns:pyxs="http://projects.coder.cl/pyxser/model/" version="1.0" type="TestClass" module="__main__" objid="id3074097420">
  <pyxs:prop type="str" name="a">var_a</pyxs:prop>
  <pyxs:prop type="str" name="c">var_c</pyxs:prop>
  <pyxs:prop type="str" name="b">var_b</pyxs:prop>
</pyxs:obj>
```

Related Webpages
---

The pyxser main web page is located here [coder.cl](http://coder.cl/products/pyxser/)


Authors
---

* Copyright (c) 2009 [Daniel Molina Wegener](https://github.com/dmw) [coder.cl](http://coder.cl)

