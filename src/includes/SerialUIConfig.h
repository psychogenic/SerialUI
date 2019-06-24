/*
 * SerialUIConfig.h
 *
 *  Created on: May 25, 2018
 *      Author: Pat Deegan
 *
 *  SerialUIConfig is part of the SerialUI project.
 *  Copyright (C) 2018-2019 Pat Deegan, psychogenic.com
 */

#ifndef SERIALUIV3_SRC_INCLUDES_SERIALUICONFIG_H_
#define SERIALUIV3_SRC_INCLUDES_SERIALUICONFIG_H_


// SERIAL_UI_ENABLE_USBSERIAL_TERMINALACCESS
// undefine this to save about 2k of flash, at the cost
// of losing console/terminal access (must use device druid GUI)
#define SERIAL_UI_ENABLE_USBSERIAL_TERMINALACCESS


#define SERIALUI_USERCHECK_MAXDELAY_DEFAULT_MS			5000



#define SERIALUI_HANDLEREQUESTS_DELAY_MS				2

#define SERIALUI_USERCHECK_BLOCKFORINPUTDELAY_MS		2

#define SUI_BUILTIN_REQUESTS_MAXSIZE					4


/* These _DEFAULT values should be set
 * by platform (without the _DEFAULT suffix),
 * but we have sane defaults, here, used if the corresponding
 * setting isn't defined by the platform.
 */
#define SERIALUI_MENUSTRUCTURE_NUMITEMS_ATSTARTUP_DEFAULT		15
#define SERIALUI_MENUSTRUCTURE_NUMITEMS_GROWLIST_DEFAULT		5
#define SERIALUI_TRACKING_NUMITEMS_ATSTARTUP_DEFAULT			5
#define SERIALUI_TRACKEDVIEW_MAXNUM_STATES_DEFAULT				4


#define SERIALUI_LANGUAGE_EN

//#define SERIALUI_BUILD_TESTS




#define SERIAL_UI_VERSION              3
#define SERIAL_UI_SUBVERSION           2
#define SERIAL_UI_PATCHLEVEL           2



#define SUIXSTR(s) SUISTR(s)
#define SUISTR(s)       #s


#define SERIAL_UI_VERSION_TOSTRING(V, SV)               V "." SV
#define SERIAL_UI_VERSION_STRING        SERIAL_UI_VERSION_TOSTRING(SUIXSTR(SERIAL_UI_VERSION), SUIXSTR(SERIAL_UI_SUBVERSION))

#define SERIALUI_VERSION_AT_LEAST(v, sub)  ( ((SERIAL_UI_SUBVERSION >= sub) and (SERIAL_UI_VERSION >= v)) or \
                                                                                        (SERIAL_UI_VERSION > v))


#define SERIALUI_AUTH_PASSPHRASE_MINLEN		5
#define SERIALUI_AUTH_PASSPHRASE_MAXLEN		64

// #define SERIALUI_ENABLE_DEBUG_OUTPUT
// #define SERIALUI_WARN_PLATFORM_DETECTION_DEBUG

#endif /* SERIALUIV3_SRC_INCLUDES_SERIALUICONFIG_H_ */
