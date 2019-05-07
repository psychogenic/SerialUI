/*
 * SUILinuxSupport.cpp
 *
 *  Created on: Apr 27, 2019
 *      Author: Pat Deegan
 *
 *  SUILinuxSupport is part of the SerialUI project.
 *  Copyright (C) 2018-2019 Pat Deegan, psychogenic.com
 */

#include "includes/SerialUIPlatform.h"

#ifdef SERIALUI_PLATFORM_LINUX

#include <sys/time.h> // timeval/gettimeofday
#include <unistd.h>

// #include <stdint.h>
// #include <stdlib.h>
// #include <string.h>
//
#include <sys/types.h>



unsigned long int millis(void) {
	return micros()/1000;
}
unsigned long int micros(void) {
	static struct timeval startTime;
	static bool initDone = false;

	struct timeval nowTime;

	if (! initDone)
	{
		initDone = true;
		gettimeofday(&startTime, NULL);

	}
	gettimeofday(&nowTime, NULL);


	return (nowTime.tv_sec * 1000000 + (nowTime.tv_usec)) -
			(startTime.tv_sec * 1000000 + (startTime.tv_usec));

}
void delay(unsigned long int d) {
	for (int i=0; i<50; i++) {
		delayMicroseconds(d*20);
	}
}
void delayMicroseconds(unsigned long int us) {
	usleep(us);
}



unsigned long random(unsigned long minOrMax, unsigned long max)
{
	unsigned long diff;
	if (max && minOrMax)
	{
		diff = max - minOrMax;

		return ((random() % diff) + minOrMax);
	}
	if (minOrMax)
		return random() % minOrMax;

	return random();
}


#endif /* SERIALUI_PLATFORM_LINUX */


