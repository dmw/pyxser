#!/bin/sh
#
#

export LC_ALL=C
export CFLAGS='-Wall -Wextra -Wshadow -pedantic -std=c99 -g3 -pg -D PYXSER_DEBUG -O0'

/usr/bin/env python ./setup.py --verbose clean --all 2>&1
/usr/bin/env python ./setup.py --verbose build --force --debug 2>&1 | sed '/parameter/d'
/usr/bin/env python ./setup.py --verbose develop 2>&1
