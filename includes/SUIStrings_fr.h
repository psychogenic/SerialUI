/*
 *
 * SUIStrings_en.h -- SerialUI system strings, english version.
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
 * The system needs a number of strings to function.  They are defined
 * on a per-language basis in their own SUIStrings_XX.h file, with the
 * appropriate language being included by SUIStrings.h.
 *
 * To implement a new language, copy this file to
 * SUIStrings_XX.h (where XX is the code for your language) and modify
 * the string _contents_ appropriately.
 *
 * Edit SUIStrings.h to support your new SUI_STRINGS_LANGUAGE_XX
 * define (XX being the language code, like EN or FR).
 *
 * Finally, modify SUIConfig.h to use the new target language file,
 * rather than SUI_STRINGS_LANGUAGE_EN
 *
 *
 */

#ifndef SerialUI_Strings
#define SerialUI_Strings



#define SUI_SERIALUI_TOP_MENU_NAME			"HAUT"


// built-in commands
#define SUI_SERIALUI_HELP_KEY				"?"
#define SUI_SERIALUI_HELP_HELP				"Enumerer selections disponible (aide)"
#define SUI_SERIALUI_UP_KEY					".."
#define SUI_SERIALUI_UP_HELP				"Monter de niveau"
#define SUI_SERIALUI_EXIT_KEY				"quitter"
#define SUI_SERIALUI_EXIT_HELP				"Quitter SerialUI"

// help-related
#define SUI_SERIALUI_HELP_HINT				"Entrez '?' pour les options disponible"
#define SUI_SERIALUI_SUBMENU_HELP			"Aller au sous-repertoire"

#define SUI_SERIALUI_HELP_TITLE_PREFIX		"*** Aide pour: "


#define SUI_SERIALUI_KEYHELP_COMMAND_PREFIX			"\t* "
#define SUI_SERIALUI_KEYHELP_SUBMENU_PREFIX			"\t+ "
#define SUI_SERIALUI_KEYHELP_SEP					" "

// prompts
#define SUI_SERIALUI_PROMPT					"> "
#define SUI_SERIALUI_MOREDATA_PROMPT		"... "


// errors
#define SUI_SERIALUI_UNKNOWN_SELECTION		" Selection inconnue '"
#define SUI_SERIALUI_UNKNOWN_INMENU			"' dans le menu: "

// TODO: Translate these?
#define SUI_ERRORMSG_SUIGREETING_TOOLONG	"SUI greeting overflow"
#define SUI_ERRORMSG_MENUNAME_TOOLONG		"Menu name overflow"
#define SUI_ERRORMSG_MENUITEM_KEY_TOOLONG 	"Menu item key overflow"
#define SUI_ERRORMSG_MENUITEM_HELP_TOOLONG 	"Menu item help overflow"
#define SUI_ERRORMSG_CANTALLOCATE_SUBMENU	"Couldn't allocate space for submenu"
#define SUI_ERRORMSG_CANTALLOCATE_MENUITEM	"Couldn't allocate space for menuitem"
#define SUI_ERRORMSG_CANTALLOCATE_KEY		"Couldn't allocate space for key"
#define SUI_ERRORMSG_NO_MENUITEMS_SET		"No menu items have been set"


#endif
