#!/bin/sh
#
#

rm -f valgrind.log

VLG_OPTIONS="--trace-children=yes "
VLG_OPTIONS="$VLG_OPTIONS --child-silent-after-fork=yes"
VLG_OPTIONS="$VLG_OPTIONS --track-fds=yes"
VLG_OPTIONS="$VLG_OPTIONS --time-stamp=yes"
VLG_OPTIONS="$VLG_OPTIONS --demangle=yes"
VLG_OPTIONS="$VLG_OPTIONS --num-callers=50"
VLG_OPTIONS="$VLG_OPTIONS --error-limit=yes"
VLG_OPTIONS="$VLG_OPTIONS --show-below-main=yes"
VLG_OPTIONS="$VLG_OPTIONS --smc-check=all"
VLG_OPTIONS="$VLG_OPTIONS --read-var-info=yes"
VLG_OPTIONS="$VLG_OPTIONS --run-libc-freeres=yes"
VLG_OPTIONS="$VLG_OPTIONS --show-emwarns=yes"
VLG_OPTIONS="$VLG_OPTIONS --leak-check=full"
VLG_OPTIONS="$VLG_OPTIONS --leak-resolution=high"
VLG_OPTIONS="$VLG_OPTIONS --show-reachable=yes"
VLG_OPTIONS="$VLG_OPTIONS --undef-value-errors=yes"
VLG_OPTIONS="$VLG_OPTIONS --track-origins=yes"
VLG_OPTIONS="$VLG_OPTIONS --partial-loads-ok=yes"
VLG_OPTIONS="$VLG_OPTIONS --log-file=valgrind.log"


export VLG_OPTIONS

/usr/bin/env valgrind $VLG_OPTIONS $@
