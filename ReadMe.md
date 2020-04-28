yum install alsa-lib-devel
yum install bluez-libs-devel

g++ -g -o alsatest main.cpp -lasound
scp music.wav pi@domain.duckdns.org:/home/pi/projects/laboratoire5