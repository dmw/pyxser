
pxyser, Python XML Serialization
===

The pyxser name stands for Python XML Serialization and is a
Python object to XML serializer and deserializer. In other words,
it can convert a Python object into XML and also, convert that XML
back into the original Python object. It uses a preorder traversal
tree algorithm over the object tree and the XML tree. The algorithm
is , and uses some compiler construction techniques as memoization
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

*** See the INSTALLING file for installing instructions. ***
*** See the AUTHORS file to get information about the authors ***

The pyxser web page is:
http://coder.cl/products/pyxser/

Best regards,
Daniel Molina Wegener
