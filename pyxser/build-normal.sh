#!/bin/sh
#
#

export LC_ALL=C
export CFLAGS='-Wall -Wextra -Wshadow -pedantic -std=c99'

/usr/bin/env python ./setup.py --verbose clean --all 2>&1
/usr/bin/env python ./setup.py --verbose build --force 2>&1 | sed '/parameter/d'
/usr/bin/env python ./setup.py --verbose install --force 2>&1

