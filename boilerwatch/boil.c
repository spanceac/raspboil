#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wiringPi.h>
#include <time.h>

#define BOILER_START digitalWrite(23, HIGH);
#define BOILER_STOP digitalWrite(23, LOW);
#define LIGHTS_ON digitalWrite(24, HIGH);
#define LIGHTS_OFF digitalWrite(24, LOW);
int action = 0, boiler_started = 0;
time_t start_timestamp;

void start_boiler(void)
{
	printf("Boiler starting \n");
	BOILER_START; //switch relay on
	LIGHTS_ON; //turn on the lights
	start_timestamp = time(NULL);
	boiler_started = 1;
	return;
}
void stop_boiler(void)
{
	printf("Boiler stopping \n");
	BOILER_STOP;
	LIGHTS_OFF;
	boiler_started = 0;
	return;
}

void check_for_state_time(void)
{
	FILE *fd;
	char todo;
	fd = fopen("/dev/shm/now","r");
	todo = fgetc(fd);
	fclose(fd);
	if (todo == '0')
	{
		action = 0;
	}
	else if (todo == '1')
	{
		action = 1;
	}
	else if (todo == '2')
	{
		action = 2;
	}
	return;
}

void check_boiler_indicator(void)
{
	FILE *fd;
	char temperature_reached;
	fd = fopen("/dev/shm/water_temperature","r");
	temperature_reached = fgetc(fd);
	fclose(fd);
	if(temperature_reached == '1')
		stop_boiler();
	return;
}
void clear_shm(void)
{
	FILE *fd;
	fd = fopen("/dev/shm/now","w");
	fputs("",fd);
	fclose(fd);
	fd = fopen("/dev/shm/water_temperature","w");
	fputs("",fd);
	fclose(fd);
}
void check_time_overflows(void)
{
	/* if boiler was started for more than 1.5 hours but no one stopped him 
	it is clear that there is a problem  so we should stop*/
	time_t current_time;
	current_time = time(NULL);
	if((current_time - start_timestamp) >= 5400) //5400 seconds
	{
		printf("!!!! Time overflow, STOP boiler\n");
		stop_boiler();
		start_timestamp = 0;
	}
	
}

int main(void)
{
	wiringPiSetupGpio();
	// GPIO 23 -> relay, GPIO 24 -> lights
	pinMode(23, OUTPUT);
	pinMode(24, OUTPUT);
	BOILER_STOP;
	LIGHTS_OFF;
	char test = 'p';
	FILE *water_fd;
	while(1)
	{
		system("/usr/bin/python /home/pi/bin/mail.py"); //checks for mail
		system("/usr/bin/python /home/pi/bin/check_time.py"); //checks if now is time to start
		check_for_state_time();
		if ((action == 1) && (boiler_started == 0)) //start boiler only if not started already
		{
			action = 0;
			start_boiler();
		}
		else if(action == 2)
		{
			action = 0;
			stop_boiler();
		}
		if(boiler_started)
		{
			system("/home/pi/bin/imageproc"); //video check if temperature is reached
			water_fd = fopen("/dev/shm/water_temperature","r");
			if(water_fd != NULL)
			{
				test = fgetc(water_fd);
				if(test == 0xff) //nothing in video, GTFO
				{
					printf("Error in imageproc 1 \n");
					clear_shm();
					stop_boiler();
					exit(1);
				}
			}
			else //error in video, GTFO
			{
				printf("Error in imageproc 2 \n");
				clear_shm();
				stop_boiler();
				exit(1);
			}
			fclose(water_fd);
			check_boiler_indicator();
			check_time_overflows(); //a kind of watchdog
		}
		clear_shm();
		sleep(10);
		fflush(stdout);
	}
}
