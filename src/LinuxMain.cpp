/*
 * LinuxMain.cpp
 *
 *  Created on: Apr 27, 2019
 *      Author: malcalypse
 */


#include "includes/SerialUIPlatform.h"

#ifdef SERIALUI_PLATFORM_LINUX

#include "includes/platform/linux/LinuxMain.h"

int main(int argc, char **ppArgv)  {
	devicedruidmain(argc, ppArgv);
}

#endif /* SERIALUI_PLATFORM_LINUX */


