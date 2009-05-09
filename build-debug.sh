#!/bin/sh
#
#

export LC_ALL=C
export CFLAGS='-Wall -Wextra -Wshadow -pedantic -std=c99 -g3 -pg'

/usr/bin/env python ./setup.py --verbose clean --all
/usr/bin/env python ./setup.py --verbose build --force --debug
/usr/bin/env python ./setup.py --verbose install

