yum install alsa-lib-devel
yum install bluez-libs-devel

g++ -g -o alsatest main.cpp -lasound
scp music.wav pi@domain.duckdns.org:/home/pi/projects/laboratoire5

bluetooth :
gcc -o client client.c -lbluetooth
    exemple possible d'utilisation du client (pas sûr):
./client B8:27:EB:1A:4E:52 2 0
./client B8:27:EB:F8:55:90 2 0 