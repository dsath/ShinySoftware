serial port setup: 
stty -F /dev/ttyUSB0 9600 cs8 -parenb cstopb > /dev/ttyUSB0


I used:
sudo vim /etc/systemd/logind.conf
then
HandleLidSwitch=suspend >>> HandleLidSwitch=ignore
then
sudo systemctl restart systemd-logind
