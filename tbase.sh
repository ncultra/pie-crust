#!/bin/bash

# tbase.sh pid
#
# copyright Michael D. Day 2017
# made available under the GNU GPL, v2



PID=self
PROGRAM=$0
QUIET=0
usage() {
    echo "$PROGRAM"
    echo "  [--pid]=<process id> of maps to read, or leave empty to read self/maps"
    echo "  [--config]=<config file> (not yet in use)"
    echo "  [--gdb]  run under gdb (not yet implemented)"
    
    exit 1
}

until [ -z "$1" ]; do	 
    case "${1:0:2}" in
	"--")
	    case "${1:2:3}" in 
		"pid") PID="${1##--pid=}";;
		"con") CONFIG_FILE="${1##--config=}";;
		"gdb") RUN_GDB=1;;
		"qui") QUIET=1;;
		"hel") usage ;;
	    esac ;;
    esac
    shift;
done
TEXT_BASE="$(head -n1 /proc/$PID/maps | grep 'r-xp' | cut -s -d '-' -f 1)"
FILE="/$(head -n1 /proc/$PID/maps | grep 'r-xp' | cut -s -d '/'  -f 2-)" 
if (( $QUIET == 0 )) ; then 
    echo "pid    text base       file name"
    echo "---------------------------------"
fi
echo "$PID   $TEXT_BASE      $FILE      "
