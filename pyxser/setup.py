#!/usr/bin/env python

##
## $Id$
##

import sys
import commands
import distutils.sysconfig
import distutils.ccompiler
import distutils.file_util
import distutils.command.install
from distutils.core import setup
from distutils.core import setup, Extension
from distutils.command.install import INSTALL_SCHEMES

vi = sys.version_info

if not (vi[0] == 2 \
        and (vi[1] == 4 \
             or vi[1] == 5 \
             or vi[1] == 6 \
             or vi[1] == 7 \
             or vi[1] == 8)):
    print "Unsupported Python version"
    sys.exit()

outversion = commands.getoutput("pkg-config --version")
if outversion == None or len(outversion) == 0:
    print "pkg-config not found"
    sys.exit(0)

for scheme in INSTALL_SCHEMES.values():
    scheme['data'] = scheme['purelib']

def pkgconfig(*packages, **kw):
    flag_map = {'-I': 'include_dirs', '-L': 'library_dirs', '-l': 'libraries'}
    items = commands.getoutput("pkg-config --silence-errors --libs --cflags %s " % ' '.join(packages))
    if not (len(items) > 0):
        print("Please install the required development packages, read the INSTALLING file")
        sys.exit(0)
    if items.find("not found") >= 0:
        print("Please install the required development packages, read the INSTALLING file")
        sys.exit(0)
    for token in items.split():
        kw.setdefault(flag_map.get(token[:2]), []).append(token[2:])
    return kw

pylibdir = distutils.sysconfig.get_python_lib(plat_specific=1, standard_lib=0)
pyxser_dtd_file_dir = pylibdir + "/pyxser/xml"
pyxser_dtd_file = pylibdir + "pyxser/xml/pyxser-1.0.dtd"
pyxser_dtd_c14n_file = pylibdir + "pyxser/xml/pyxser-1.0-c14n.dtd"
pyxser_xsd_file = pylibdir + "pyxser/xml/pyxser-1.0.xsd"
pyxser_xsd_c14n_file = pylibdir + "pyxser/xml/pyxser-1.0-c14n.xsd"
pyxser_dtd_location = '\"' + pylibdir + "/pyxser/xml/pyxser-1.0.dtd" + '\"'
pyxser_dtd_c14n_location = '\"' + pylibdir + "/pyxser/xml/pyxser-1.0-c14n.dtd" + '\"'
pyxser_xsd_location = '\"' + pylibdir + "/pyxser/xml/pyxser-1.0.xsd" + '\"'
pyxser_xsd_c14n_location = '\"' + pylibdir + "/pyxser/xml/pyxser-1.0-c14n.xsd" + '\"'

pyxser_params = pkgconfig("libxml-2.0")

if not isinstance(pyxser_params, dict):
    print("*** configuration dictionary not created, creating one...")
    pyxser_params = {}

if not pyxser_params.has_key("library_dirs"):
    print("*** library_dirs reconfigured...")
    pyxser_params["library_dirs"] = []

if not pyxser_params.has_key("include_dirs"):
    print("*** include_dirs reconfigured...")
    pyxser_params["include_dirs"] = []

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
                                  './src/pyxser_collections.c',
                                  './src/pyxser_serializer.c',
                                  './src/pyxser_typem.c',
                                  './src/pyxser_xsd.c',
                                  ],
                       define_macros = [("PYXSER_DTD_FILE", pyxser_dtd_location),
                                        ("PYXSER_DTD_C14N_FILE", pyxser_dtd_c14n_location),
                                        ("PYXSER_XSD_FILE", pyxser_xsd_location),
                                        ("PYXSER_XSD_C14N_FILE", pyxser_xsd_c14n_location)],
                       **pyxser_params)

setup (name = 'pyxser',
       version = '1.5r',
       description = 'Python XML Serialization Extension',
       author = 'Daniel Molina Wegener',
       author_email = 'dmw@coder.cl',
       url = 'http://coder.cl/products/pyxser/',
       download_url = 'https://sourceforge.net/projects/pyxser/files/pyxser-1.5r/pyxser-1.5r.tar.gz/download',
       data_files = [(pyxser_dtd_file_dir, ['xsd/pyxser-1.0.dtd',
                                            'xsd/pyxser-1.0-c14n.dtd',
                                            'xsd/pyxser-1.0.xsd',
                                            'xsd/pyxser-1.0-c14n.xsd'])],
       license = '''GNU LESSER GENERAL PUBLIC LICENSE, Version 3, 29 June 2007''',
       long_description = '''
       pyxser --- a single xml serialization/deserialization extension for python.
       ''',
       ext_modules = [pyxser_mod])

