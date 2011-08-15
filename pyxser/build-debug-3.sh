#!/bin/sh
#
#

export LC_ALL=C
export CFLAGS='-Wall -Wextra -Wshadow -pedantic -std=c99 -g3 -pg -O1'

/usr/bin/env python3.2 ./setup.py --verbose clean --all 2>&1
/usr/bin/env python3.2 ./setup.py --verbose build --force --debug 2>&1 | sed '/parameter/d'
/usr/bin/env python3.2 ./setup.py --verbose install --force 2>&1

