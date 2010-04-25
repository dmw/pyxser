#!/bin/sh
#
#

export LC_ALL=C
export CFLAGS='-Wall -Wextra -Wshadow -pedantic -std=c99'

/usr/bin/env python ./setup.py --verbose clean --all
/usr/bin/env python ./setup.py --verbose build --force
/usr/bin/env python ./setup.py --verbose install --force



