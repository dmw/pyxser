#!/usr/bin/env python

import commands
from distutils.core import setup, Extension

def pkgconfig(*packages, **kw):
    flag_map = {'-I': 'include_dirs', '-L': 'library_dirs', '-l': 'libraries'}
    items = commands.getoutput("pkg-config --libs --cflags %s" % ' '.join(packages))
    for token in items.split():
        kw.setdefault(flag_map.get(token[:2]), []).append(token[2:])
    return kw

pyxser_params = {}

pyxser_params = pkgconfig("libxml-2.0")

print "Params: " + repr(pyxser_params)

pyxser_params["library_dirs"] = []

# extra library dirs
pyxser_params["library_dirs"].append("/lib")
pyxser_params["library_dirs"].append("/usr/lib")
pyxser_params["library_dirs"].append("/usr/local/lib")
pyxser_params["library_dirs"].append("/usr/X11R6/lib")
pyxser_params["library_dirs"].append("/opt/lib")

# extra include dirs
pyxser_params["include_dirs"].append("/usr/include")
pyxser_params["include_dirs"].append("/usr/local/include")
pyxser_params["include_dirs"].append("/usr/X11R6/include")
pyxser_params["include_dirs"].append("/opt/include")
pyxser_params["include_dirs"].append(".")
pyxser_params["include_dirs"].append("./src")

pyxser_mod = Extension('pyxser',
                       sources = ['./src/pyxser.c',
                                  './src/pyxser_numbers.c',
                                  './src/pyxser_tools.c',
                                  './src/pyxser_strings.c',
                                  './src/pyxser_collections.c'],
                       **pyxser_params)

setup (name = 'pyxser',
       version = '0.1',
       description = 'Python XML Serialization Package',
       author = 'Daniel Molina Wegener',
       author_email = 'dmw@coder.cl',
       url = 'http://coder.cl/pyxser/',
       long_description = '''
       pyxser --- a single xml serialization/deserialization extension for python.
       ''',
       ext_modules = [pyxser_mod])

