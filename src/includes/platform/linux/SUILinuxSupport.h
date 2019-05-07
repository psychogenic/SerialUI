/*
 * SUILinuxSupport.cpp
 *
 *  Created on: Apr 27, 2019
 *      Author: Pat Deegan
 *
 *  SUILinuxSupport is part of the SerialUI project.
 *  Copyright (C) 2018-2019 Pat Deegan, psychogenic.com
 */

#ifndef SERIALUI_SRC_INCLUDES_PLATFORM_LINUX_SUILINUXSUPPORT_H_
#define SERIALUI_SRC_INCLUDES_PLATFORM_LINUX_SUILINUXSUPPORT_H_


#include "../SUIWString.h"
#include "LinuxExtIncludes.h"
#include "LinuxStream.h"


unsigned long int millis(void);
unsigned long int micros(void);
void delay(unsigned long int d);
void delayMicroseconds(unsigned long int us);
unsigned long random(unsigned long minOrMax, unsigned long max);

#endif /* SERIALUI_SRC_INCLUDES_PLATFORM_LINUX_SUILINUXSUPPORT_H_ */
