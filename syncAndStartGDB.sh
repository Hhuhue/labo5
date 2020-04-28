#!/bin/bash
set -e

# Sync executable
bn=$(basename $1)
rpiaddr="mountain-goat.duckdns.org"

rsync -az $1/build/playsound "pi@$rpiaddr:/home/pi/projects/laboratoire5/"
rsync -az $1/server.c "pi@$rpiaddr:/home/pi/projects/laboratoire5/"
rsync -az $1/compilation_server.sh "pi@$rpiaddr:/home/pi/projects/laboratoire5/"

