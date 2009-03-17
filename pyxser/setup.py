#!/usr/bin/env python

import commands
import distutils.sysconfig
import distutils.ccompiler
from distutils.core import setup
from distutils.core import setup, Extension
from distutils.command.install import INSTALL_SCHEMES

for scheme in INSTALL_SCHEMES.values():
    scheme['data'] = scheme['purelib']

def pkgconfig(*packages, **kw):
    flag_map = {'-I': 'include_dirs', '-L': 'library_dirs', '-l': 'libraries'}
    items = commands.getoutput("pkg-config --libs --cflags %s" % ' '.join(packages))
    for token in items.split():
        kw.setdefault(flag_map.get(token[:2]), []).append(token[2:])
    return kw

pylibdir = distutils.sysconfig.get_python_lib(plat_specific=1, standard_lib=0)
pyxser_dtd_location = '\"' + pylibdir + "/pyxser/xml/pyxser-1.0.dtd" + '\"'

pyxser_params = {}
pyxser_params = pkgconfig("libxml-2.0")
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
                       define_macros = [("PYXSER_DTD_FILE", pyxser_dtd_location)],
                       **pyxser_params)

setup (name = 'pyxser',
       version = '0.1',
       description = 'Python XML Serialization Extension',
       author = 'Daniel Molina Wegener',
       author_email = 'dmw@coder.cl',
       url = 'http://projects.coder.cl/pyxser/',
       download_url = 'http://sourceforge.net/project/showfiles.php?group_id=256386',
       data_files = [("pyxser/xml", ["src/pyxser-1.0.dtd"])],
       license = '''GNU LESSER GENERAL PUBLIC LICENSE, Version 3, 29 June 2007''',
       long_description = '''
       pyxser --- a single xml serialization/deserialization extension for python.
       ''',
       ext_modules = [pyxser_mod])


