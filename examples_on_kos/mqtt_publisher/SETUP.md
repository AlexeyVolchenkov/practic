## Steps to setup a 'Mosquitto publisher' example on Raspberry Pi 4

1. Install required packages on your host
```
# sudo apt install mosquitto mosquitto-clients
```
2. Set your computer network interface to have a static IPv4 address 10.0.2.2/24 and connect it to the Raspberry Pi 4 board
3. Ensure you have mosquitto broker running, it could be started with
```
# sudo /etc/init.d/mosquitto start
```
4. Start mosquitto subscriber
```
# mosquitto_sub -d -t "datetime"
```
5. Power up RPi4