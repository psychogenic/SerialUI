/*
 * LinuxMain.cpp
 *
 *  Created on: Apr 27, 2019
 *      Author: Pat Deegan
 *
 *  Part of the SerialUI project.
 *  Copyright (C) 2019 Pat Deegan, https://psychogenic.com
 *  More information on licensing and usage at 
 *  https://devicedruid.com/
 */


#include "includes/SerialUIPlatform.h"

#ifdef SERIALUI_PLATFORM_LINUX

#include "includes/platform/linux/LinuxMain.h"

int main(int argc, char **ppArgv)  {
	devicedruidmain(argc, ppArgv);
}

#endif /* SERIALUI_PLATFORM_LINUX */


