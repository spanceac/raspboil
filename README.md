raspboil
========

Raspboil is a project that uses a Raspberry Pi for a water boiler control via e-mail and image processing.
The project uses the wiringPi library and openCV.
The instructions assume that you already have Raspbian installed on your card.
You will also need to create an e-mail address at gmail for your Raspberry Pi.
Project page: http://hackingbeaver.com/?p=97

To install  wiringPi:
http://wiringpi.com/download-and-install/

To install and use openCV:
<pre><code>sudo apt-get update
sudo apt-get install cmake
sudo apt-get install libopencv-dev
</code></pre>

Building and installing the main daemon:
<pre><code>mkdir /home/pi/bin
cd boilerwatch
gcc boil.c -o boil -lwiringPi
cp boil /home/pi/bin
cd ..
</code></pre>

Building and installing the image processing binary:
<pre><code>cd imageprocessing
cmake .
make
cp imageproc /home/pi/bin
cd ..
</code></pre>

Installing the python scripts and the init script:
<pre><code>cp mail.py /home/pi/bin
cp check_time.py /home/pi/bin
sudo cp boiler_script /etc/init.d/
sudo update-rc.d boiler_script defaults
</code></pre>

After following these steps the raspboil project will be auto-started at system startup by the init script.

To use the project, replace the username and password in mail.py at line 113 with the gmail username and password that you created for your Raspberry Pi system. Also add your e-mail address from which the Raspberry Pi will receive mails in mail.py at line 12.
