#!/bin/sh
#
#

export CFLAGS='-Wall -Wextra -Wshadow -pedantic -std=c99 -g3 -pg'

/usr/bin/env python-static ./setup.py --verbose clean --all
/usr/bin/env python-static ./setup.py --verbose build --force --debug
/usr/bin/env python-static ./setup.py --verbose install

