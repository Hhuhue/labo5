#!/bin/bash
set -e

# Sync executable
bn=$(basename $1)
rpiaddr="private-driver.duckdns.org"

rsync -az $1/build/playsound "pi@$rpiaddr:/home/pi/projects/laboratoire5/"
rsync -az $1/build/server "pi@$rpiaddr:/home/pi/projects/laboratoire5/"

