/*
 *
 * SUIConfig.h -- SerialUI compile time configuration.
 * Copyright (C) 2013-2017 Pat Deegan.  All rights reserved.
 *
 * http://www.flyingcarsandstuff.com/projects/SerialUI/
 *
 * Please let me know if you use SerialUI in your projects, and
 * provide a URL if you'd like me to link to it from the SerialUI
 * home.
 *
 *
 * This program library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later
 * version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 * See file LICENSE.txt for further informations on licensing
 * terms.
 *
 *  ************************* OVERVIEW *************************
 *  This file is used to determine compile-time settings, for
 *  optimizations and determining which resource files to use
 *  (e.g. for language strings).
 *
 *  See below for available flags.
 */
#ifndef SERIAL_UI_CONFIGURATION_H_
#define SERIAL_UI_CONFIGURATION_H_


#include "sova/SovA.h"


/* *********************** DEFAULT VALUES *********************** */

/*
 * SUI_SERIALUI_PROGMEM_STRING_ABS_MAXLEN
 * There is a MAXIMUM LENGTH to the progmem strings (127 characters,
 * by default, set here).
 *
 * You can change this max length if you need to, but as strings
 * need to be in RAM while they are being transmitted, ensure you
 * don't eat up all the RAM by making the max too large.
 *
 */
#define SUI_SERIALUI_PROGMEM_STRING_ABS_MAXLEN				127

/*
 * SUI_SERIALUI_USERCHECK_MAXDELAY_DEFAULT_MS
 * User check timeout, in ms. Used by checkForUser*(), as the
 * default.
 */
#define SUI_SERIALUI_USERCHECK_MAXDELAY_DEFAULT_MS			5000

/*
 * SUI_SERIALUI_USERPRESENCE_MAXTIMEOUT_DEFAULT_MS
 * Maximum user idle time, in ms.
 */
#define SUI_SERIALUI_USERPRESENCE_MAXTIMEOUT_DEFAULT_MS		30000

/*
 * SUI_SERIALUI_READBYTES_TIMEOUT_DEFAULT_MS
 * The default readBytesUntil()/readBytesToEOL() time-out, for user input, in ms.
 */
#define SUI_SERIALUI_READBYTES_TIMEOUT_DEFAULT_MS			25000



/*
 * SUI_SERIAL_UI_READ_CHAR_TERMINATOR_DEFAULT
 * The default readBytesUntil()/readBytesToEOL() terminator char.
 * Must be a single char 'c' -- not a string "something bad".
 */
#define SUI_SERIAL_UI_READ_CHAR_TERMINATOR_DEFAULT			'\n'



/* ****************** COMPILE CONFIGURATION DEFINES ****************** */




/*
 * SUI_SERIALUI_ECHO_ON
 * Define SUI_SERIALUI_ECHO_ON to echo user input back out as it is sent.
 * Provides more natural feedback when a user (rather than a program) is
 * present.
 */
#define SUI_SERIALUI_ECHO_ON


/*
 * SUI_SERIALUI_SHOW_PROMPTS
 * Define SUI_SERIALUI_SHOW_PROMPTS to output prompts, e.g.
 *  >
 * Provides visual feedback to users, notifying them the
 * SerialUI is ready for input.
 */
#define SUI_SERIALUI_SHOW_PROMPTS

/*
 * SUI_SERIALUI_SHOW_PROMPT_SUBMENU_NAME
 * When defined, this will show the sub-menu name before
 * the prompt, when available. e.g.:
 *
 *   enable>
 *
 * rather than just
 *   >
 * when in the 'enable' sub-menu.
 */
#define SUI_SERIALUI_SHOW_PROMPT_SUBMENU_NAME


/*
 * SUI_STRINGS_LANGUAGE_XX
 *
 * Define ONE OF the available SUI_STRINGS_LANGUAGE_XX
 * to select the language of system strings.
 *
 * Only SUI_STRINGS_LANGUAGE_EN and
 * SUI_STRINGS_LANGUAGE_FR are available for now.
 */
// #define SUI_STRINGS_LANGUAGE_FR
#define SUI_STRINGS_LANGUAGE_EN


/* ******* Optional error checking and functionality ********
 * With everything on, meaning:
 *
 *	- SUI_INCLUDE_EXTRA_SAFETYCHECKS (default: disabled)
 *	- SUI_INCLUDE_DEBUG	(default: disabled)
 *	- SUI_SERIALUI_ECHO_WARNINGS (default: disabled)
 *	- SUI_MENU_INCLUDE_DESTRUCTION_CLEANUP (default: disabled)
 *	- SUI_MENU_ENABLE_SUBMENUS  (default: enabled)
 *	- SUI_DYNAMIC_MEMORY_ALLOCATION_ENABLE (default: enabled)
 *
 * You'll use up about 2.1k extra in compiled code space as compared
 * to having all the above disabled.
 *
 * You can disable each of these, by commenting out the #defines,
 * when you know all is going according to plan and have determined
 * you don't need that specific functionality.
 *
 */



/*
 * SUI_INCLUDE_EXTRA_SAFETYCHECKS
 * Define SUI_INCLUDE_EXTRA_SAFETYCHECKS to include sanity checks.
 *
 * Disable this (undefine it/comment it out) if you know everything is
 * working and you're tight on flash space.
 */
// #define SUI_INCLUDE_EXTRA_SAFETYCHECKS


/*
 * SUI_SERIALUI_ECHO_WARNINGS
 * Define SUI_SERIALUI_ECHO_WARNINGS to output warnings (mainly related to
 * memory allocation issues).
 *
 * Disable this (undefine it/comment it out) if you know everything is
 * working and you're tight on flash space.
 */
// #define SUI_SERIALUI_ECHO_WARNINGS



/*
 * SUI_MENU_ENABLE_SUBMENUS
 *
 * Normally, SerialUI supports both command and submenu type menu items, along with
 * menu hierarchy navigation.  This only takes up about 600 bytes but, if space is
 * really super tight and you only plan to have a one-level menu system, then you
 * may undefine SUI_MENU_ENABLE_SUBMENUS and save that space.
 */
#define SUI_MENU_ENABLE_SUBMENUS


/*
 * SUI_ENABLE_MODES
 *
 * Leave SUI_ENABLE_MODES defined to allow your device to be accessible by 
 * third party programs in "program mode".  
 * 
 * Druid4Arduino is such a program (it is a dynamically reconfigured UI that 
 * provides a GUI for any SerialUI-based device).
 *
 * The program-mode modifies the output to make it easier for parsers to 
 * interact with the system, without changing anything from the Serial 
 * channel user's experience.
 *
 * Undefining SUI_ENABLE_MODES will make using Druid (and probably any other
 * program designed to interact with SerialUI) impossible, but will save a bit
 * of flash space.
 */ 
#define SUI_ENABLE_MODES

/*
 * SUI_INCLUDE_DEBUG
 * Define SUI_INCLUDE_DEBUG to include extra debug output.
 *
 * If SUI_INCLUDE_DEBUG is defined, you may use SerialUI::debug(const char*) to
 * output debug messages (wrap these calls in an #ifdef, or it'll puke when you
 * undefine SUI_INCLUDE_DEBUG).
 *
 * Disabled by default.
 */
//#define SUI_INCLUDE_DEBUG
// define SUI_INCLUDE_FREERAMFUNCS

/*
 * SUI_MENU_INCLUDE_DESTRUCTION_CLEANUP
 *
 * Destruction/clearing of menu items would be useful if you are dynamically modifying
 * the menu structure.  This functionality takes up space that is usually unneeded, so
 * you'll have to define SUI_MENU_INCLUDE_DESTRUCTION_CLEANUP here to include the
 * functionality.
 *
 * Disabled by default.
 */
// #define SUI_MENU_INCLUDE_DESTRUCTION_CLEANUP


/*
 * SUI_DYNAMIC_MEMORY_ALLOCATION_ENABLE
 *
 * By default, memory is allocated dynamically (on an as-needed basis).  This saves
 * on RAM, using the least amount possible, and allows for flexibility.
 *
 * But, it means including functionality for malloc/free/etc.  This takes up space
 * on the flash.  If you want to save that program space, at the cost of RAM and
 * some hard-coded limits (for number of items, submenus and key length), undefine
 * SUI_DYNAMIC_MEMORY_ALLOCATION_ENABLE.
 *
 * With SUI_DYNAMIC_MEMORY_ALLOCATION_ENABLE unset, you must ensure you respect
 * the limits imposed by the various
 *  SUI_STATIC_MEMORY_*
 * defines below.
 *
 * Note: you only save about 0.5k in program size by disabling
 * dynamic memory management.  Not much, sometimes enough :)
 */
#define SUI_DYNAMIC_MEMORY_ALLOCATION_ENABLE

/*
 * SUI_STATIC_MEMORY_*
 * Hard-coded limits, used when dynamic memory allocations are (disabled* (see above).
 *
 * SUI_STATIC_MEMORY_NUM_SUBMENUS_TOTAL_MAXIMUM: total sub-menus for the ENTIRE system
 * 	(not per-menu!).
 *
 * SUI_STATIC_MEMORY_NUM_ITEMS_MAXIMUM: maximum number of menu items per Menu.
 *
 * SUI_STATIC_MEMORY_KEY_LENGTH_MAXIMUM: longest key (command/sub-menu name)
 * possible.
 */
#define SUI_STATIC_MEMORY_NUM_SUBMENUS_TOTAL_MAXIMUM	5
#define SUI_STATIC_MEMORY_NUM_ITEMS_MAXIMUM				6
#define SUI_STATIC_MEMORY_KEY_LENGTH_MAXIMUM			15



/*
 * SUI_ENABLE_STREAMPROMPTING
 *
 * Enable the "file upload" stream prompt.
 *
 */
#define SUI_ENABLE_STREAMPROMPTING


/*
 * SUI_ENABLE_USER_PRESENCE_HEARTBEAT
 *
 * Enable the "heartbeat" function to be called, if specified, while handling requests.
 */
#define SUI_ENABLE_USER_PRESENCE_HEARTBEAT

/*
 * SUI_USER_PRESENCE_HEARTBEAT_PERIOD_DEFAULT_MS
 * Default period for heartbeat calls, if not set with
 * SerialUI.setUserPresenceHeartbeatPeriod(NNN)
 */
#define SUI_USER_PRESENCE_HEARTBEAT_PERIOD_DEFAULT_MS		500

/*
 * SUI_NOUSER_HEARTBEAT_ENABLE
 *
 * Enable the "heartbeat" even when no user present.
 *
 * Disabled by default, uncomment to enable
 */
// #define SUI_NOUSER_HEARTBEAT_ENABLE



/*
 * SUI_ENABLE_STATE_TRACKER
 *
 * Enables the variable "state tracking" which will notify Druid4Arduino of
 * changes to certain (global) variables
 */
#define SUI_ENABLE_STATE_TRACKER
#define SUI_STATE_TRACKER_MAX_VARIABLES		8

// SUI_STATE_TRACK_ALWAYSFULL -- define this to
// always receive full tracking details from pings
//define SUI_STATE_TRACK_ALWAYSFULL



/*
 * SUI_SERIALUI_USERCHECK_BLOCKFORINPUTDELAY_MS
 *
 * Used when a call to checkForUser(TIMEOUTMS) is made with some
 * positive TIMEOUTMS value.
 *
 * When TIMEOUTMS == 0, the call returns immediately with a boolean
 * indicating presence of user.  With TIMEOUTMS > 0, the check will
 * simply:
 * 	* either return true, if a user is around
 * 	* wait for SUI_SERIALUI_USERCHECK_BLOCKFORINPUTDELAY_MS and check
 * 	  again, until we hit the TIMOUTMS total.
 */
#define SUI_SERIALUI_USERCHECK_BLOCKFORINPUTDELAY_MS		1






/* **************** end user/compile-time config ****************** */











/* *********************** Platform and auto-detection thereof **************** */

/*
 * SUI_PLATFORM_X
 *
 * As of version 2, SerialUI does what it can to auto-detect the
 * platform you're building for.  If you're not using the Arduino IDE,
 * and instead prefer a custom makefile or other build system, define
 * one of:
 *
 * 	ARDUINO_ARCH_AVR
 * 	ARDUINO_ARCH_XMEGA
 * 	ARDUINO_ARCH_ESP8266
 * 	ARDUINO_ARCH_NRF51822
 * 	ARDUINO_ARCH_SAM
 *
 * like -DARDUINO_ARCH_AVR for all runs of gcc
 *
*/

// Auto-detection stuff...

// most standard arduinos
#if defined(SOVA_PLATFORM_ARDUINO_AVR)

	#define SUI_ARCH_AVR

	#ifdef SOVA_PLATFORM_AUTODETECTED
		#define SUI_PLATFORM_AUTODETECTED
		#define SUI_PLATFORM_ARDUINO_AVR
	#endif
	#ifndef SUI_BUILDFOR_ARDUINO_STANDARD
		#define SUI_BUILDFOR_ARDUINO_STANDARD
	#endif
#endif


// XMEGADUINO
#if defined(SOVA_PLATFORM_XMEGA)
	#define SUI_ARCH_AVR

	#ifdef SOVA_PLATFORM_AUTODETECTED
		#define SUI_PLATFORM_AUTODETECTED
		#define SUI_PLATFORM_XMEGA
		//error "Please #define SUI_PLATFORM_XMEGA in SUIConfig.h because the xmegaduino makefile is buggy and doesn't allow for auto-detection."
	#endif
	#ifndef SUI_BUILDFOR_ARDUINO_STANDARD
		#define SUI_BUILDFOR_ARDUINO_STANDARD
	#endif
#endif



#if defined(SOVA_PLATFORM_ESP8266)
	#ifdef SOVA_PLATFORM_AUTODETECTED
		#define SUI_PLATFORM_AUTODETECTED
		#define SUI_PLATFORM_ESP8266
	#endif
	#define SUI_BUILDFOR_ARDUINO_STANDARD
	#define SUI_ARCH_ESP8266
#endif


// BLE Nano
#if defined(SOVA_PLATFORM_RBLNRF51822)
	#ifdef SOVA_PLATFORM_AUTODETECTED
		#define SUI_PLATFORM_AUTODETECTED
		#define SUI_PLATFORM_RBLNRF51822
	#endif /* SUI_PLATFORM_RBLNRF51822 */
	#define SUI_ARCH_NRF51822
#endif /* RBL - NRF* platform */


// DUE
#if defined(SOVA_PLATFORM_ARDUINO_SAM)
	#ifdef SOVA_PLATFORM_AUTODETECTED
		#define SUI_PLATFORM_ARDUINO_SAM
		#define SUI_PLATFORM_AUTODETECTED
		// is actually just standard Arduino +
		// the SUI_ARCH_SAM flag
		#define SUI_BUILDFOR_ARDUINO_STANDARD
		/* SUI_ARCH_SAM
		*/
		#define SUI_ARCH_SAM
	#endif
#endif

// Default (to AVR standard Arduino)
#if ! ( \
		   defined(SUI_PLATFORM_RBLNRF51822) \
		or defined(SUI_PLATFORM_ARDUINO_SAM) \
		or defined(SUI_PLATFORM_XMEGA) \
		or defined(SUI_PLATFORM_DIGISPARKUSB) \
		or defined(SUI_PLATFORM_ESP8266) \
		or defined(SUI_PLATFORM_ARDUINO_AVR))
		#warning "Could not auto-detect platform... define an appropriate SUI_PLATFORM_ARDUINO_* in SUIConfig.h"
#define SUI_PLATFORM_ARDUINO_AVR
#endif /* One SUI_PLATFORM_* defined */




// Shorthand for Digispark implementation flags
// attempting to reduce code size as
// much as possible (development/currently
// non-functional)
// #define SUI_BUILD_FOR_DIGISPARK

#ifdef SUI_BUILD_FOR_DIGISPARK
#warning "Building for digispark--undef'ing most features"
#undef SUI_INCLUDE_EXTRA_SAFETYCHECKS
#undef SUI_INCLUDE_DEBUG
#undef SUI_SERIALUI_ECHO_WARNINGS
#undef SUI_MENU_INCLUDE_DESTRUCTION_CLEANUP
#undef SUI_MENU_ENABLE_SUBMENUS
#undef SUI_ENABLE_MODES

#undef SUI_BUILDFOR_ARDUINO_STANDARD
#define SUI_PLATFORM_DIGISPARKUSB
#endif



#ifdef SUI_INCLUDE_DEBUG
#warning "SUI_INCLUDE_DEBUG is enabled"
#ifndef SUI_PLATFORM_HARDWARESERIAL_DEFAULT
#define SUI_PLATFORM_HARDWARESERIAL_DEFAULT 	Serial
#endif

#endif





#define SERIAL_UI_VERSION			2
#define SERIAL_UI_SUBVERSION		2
#define SERIAL_UI_PATCHLEVEL		0



#define SUIXSTR(s) SUISTR(s)
#define SUISTR(s) 	#s


#define SERIAL_UI_VERSION_TOSTRING(V, SV)		V "." SV
#define SERIAL_UI_VERSION_STRING	SERIAL_UI_VERSION_TOSTRING(SUIXSTR(SERIAL_UI_VERSION), SUIXSTR(SERIAL_UI_SUBVERSION))

#define SERIALUI_VERSION_AT_LEAST(v, sub)  ( ((SERIAL_UI_SUBVERSION >= sub) and (SERIAL_UI_VERSION >= v)) or \
											(SERIAL_UI_VERSION > v))


#endif /* SERIAL_UI_CONFIGURATION_H_ */
