#!/bin/sh
#
# ~/.xinitrc
#
# Executed by startx (run your window manager from here)
#

# numlockx &
/usr/bin/xscreensaver -no-splash &

exec window_manager
