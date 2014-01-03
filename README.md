raspboil
========

Raspboil is a project that uses a Raspberry Pi for a water boiler control via e-mail and image processing.
The project uses the wiringPi library and openCV.
The instructions assume that you already have Raspbian installed on your card.

To install  wiringPi:
http://wiringpi.com/download-and-install/

To install opencv:
sudo apt-get update
sudo apt-get install cmake
sudo apt-get install libopencv-dev

To build the apps after cloning the repository do the following:
Building and installing the main daemon:
~$ mkdir /home/pi/bin
~$ cd boilerwatch
~$ gcc boil.c -o boil -lwiringPi
~$ cp boil /home/pi/bin
~$ cd ..

Building and installing the image processing binary:
~$ cd imageprocessing
~$ cmake .
~$ make
~$ cp imageproc /home/pi/bin
~$ cd ..

Installing the python scripts and the init script:
~$ cp mail.py /home/pi/bin
~$ cp check_time.py /home/pi/bin
~$ sudo cp boiler_script /etc/init.d/
~$ sudo update-rc.d boiler_script defaults

After following these steps the raspboil project will be auto-started at the system startup by the init script.
