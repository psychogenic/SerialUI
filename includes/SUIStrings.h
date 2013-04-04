/*
 *
 * SUIStrings.h -- SerialUI system strings selection.
 * Copyright (C) 2013 Pat Deegan.  All rights reserved.
 *
 *
 * http://www.flyingcarsandstuff.com/projects/SerialUI
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
 *
 * ************************* OVERVIEW *************************
 *
 * Select the appropriate language strings file, here.
 * See includes/SUIStrings_en.h for details on language resource files.
 *
 */

#ifndef SUIStringsINC_h
#define SUIStringsINC_h

#include "SUIConfig.h"

#ifdef SUI_STRINGS_LANGUAGE_EN
#include "SUIStrings_en.h"
#endif

#ifdef SUI_STRINGS_LANGUAGE_FR
#include "SUIStrings_fr.h"
#endif

/* Strings common to all languages,  these are relied on by SerialGUI, and best left un-changed */
#define SUI_STRINGS_MODE_USER				"m:usr"
#define SUI_STRINGS_MODE_PROGRAM			"m:pgm"


// prompts
#define SUI_SERIALUI_PROMPT						"> "
#define SUI_SERIALUI_HELP_KEY					"?"
#define SUI_SERIALUI_UP_KEY						".."
#define SUI_SERIALUI_EXIT_KEY					"quit"


#define SUI_SERIALUI_MOREDATA_STRING_PROMPT_PROG	"~S~:"
#define SUI_SERIALUI_MOREDATA_NUMERIC_PROMPT_PROG	"~N~:"
#define SUI_SERIALUI_KEYHELP_COMMAND_PREFIX_PROG	"C:"
#define SUI_SERIALUI_KEYHELP_SUBMENU_PREFIX_PROG	"S:"
#define SUI_SERIALUI_KEYHELP_SEP_PROG				"~H~"
#define SUI_SERIALUI_PROG_ENDOFTRANSMISSION			"~EOT~"

#define SUI_SERIALUI_PROG_STR_SEP_CHAR				'#'


// A few return response strings,
// not translated, in case the other side is a program
// expecting OK/ERROR
#define SUI_SERIALUI_MESSAGE_OK					"OK"
#define SUI_SERIALUI_MESSAGE_ERROR_GENERIC		"ERROR"
#define SUI_SERIALUI_MESSAGE_ERROR_PREFIX		"ERROR:"


#endif

