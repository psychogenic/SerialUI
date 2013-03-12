/*
 *
 * SUIMenu.h -- SerialUI Menu, part of SerialUI a serial User Interface
 * for Arduino-compatible devices.
 * Copyright (C) 2013 Pat Deegan.  All rights reserved.
 *
 *
 * http://www.flyingcarsandstuff.com/projects/SerialUI
 *
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
 *
 * ************************* OVERVIEW *************************
 *
 * SUI::Menu objects are used to add commands and submenus.
 * Get a handle to the top level menu from the SerialUI object,
 * then call
 *  addCommand(),
 *  subMenu(),
 * and other methods as defined below.
 *
 * Full details found in SerialUI.h and examples/SuperBlinker.
 */

#ifndef SUIMenu_h
#define SUIMenu_h

#include "SUIConfig.h"
#include "SUIPlatform.h"


/*
 * SUI_SERIALUI_KEYHELP_SEP_REPEATS_MAX
 * Maximum amount of SUI_SERIALUI_KEYHELP_SEP repeats between key and help,
 * used to format help output.
 */
#define SUI_SERIALUI_KEYHELP_SEP_REPEATS_MAX		20


/*
 * SUI_MENU_EXPANDITEMLIST_AMOUNT_DEFAULT
 * Increment to use when creating/expanding the memory held for the list
 * of menu items (reduces the number of calls to malloc/realloc).
 */
#define SUI_MENU_EXPANDITEMLIST_AMOUNT_DEFAULT		3


// A few return response strings,
// not translated, in case the other side is a program
// expecting OK/ERROR
#define SUI_SERIALUI_MESSAGE_OK					"OK"
#define SUI_SERIALUI_MESSAGE_ERROR_GENERIC		"ERROR"
#define SUI_SERIALUI_MESSAGE_ERROR_PREFIX		"ERROR:"


// SUI is the namespace in which we keep all our goodies.
namespace SUI
{

	/*
	 * MenuItem struct is used internally
	 * to hold menu item data.
	 */
	class Menu; // forward decl
	typedef void (*MenuCommand_Callback)(void);

	typedef struct MenuItemStruct {
		PGM_P key;
		PGM_P help;
		uint8_t key_size;
		Menu * subMenu;
		MenuCommand_Callback command;

		MenuItemStruct();
		MenuItemStruct(PGM_P key_pstr, PGM_P help_pstr,
				Menu * submenu_ptr, MenuCommand_Callback cmd_ptr);


	} MenuItem;

	class SerialUI; // forward decl

	/*
	 * Menu class
	 *
	 * Represents a single menu--i.e. a set of keys to access
	 * sub-menus and execute commands.
	 *
	 */
	class Menu {


		public:
			/*
			 * addCommand(KEY, CALLBACK [, HELP])
			 *
			 * Use addCommand() to add a command menu item to a menu.
			 * The parameters are:
			 *
			 *  KEY: the (SUI_DeclareString-created) string to use as the command
			 *
			 *  CALLBACK: the name of the void(void) callback function (as described
			 *            in "Callbacks", above).
			 *
			 *  HELP: optional (SUI_DeclareString-created) string to display for
			 *        this item when menu help (?) is invoked.
			 *
			 * Return:  returns boolean true on success, false if command could not
			 *          be added.
			 */
			bool addCommand(PGM_P key_str, MenuCommand_Callback callback, PGM_P help_str=NULL);

#ifdef SUI_MENU_ENABLE_SUBMENUS
			/*
			 * subMenu(KEY [, HELP])
			 *
			 * Use subMenu to create a sub-menu accessible by KEY.  The params are:
			 *
			 *  KEY: The (SUI_DeclareString-created) string to use to enter the sub-menu
			 *       from the current menu.
			 *
			 *  HELP: optional (SUI_DeclareString-created) string to display for
			 *        this item when menu help (?) is invoked.
			 *
			 *  Return: returns a SUI::Menu pointer, which will be NULL if the subMenu
			 *          could not be created.
			 *
			 */
			Menu * subMenu(PGM_P key_str, PGM_P help_str=NULL);
#endif

			/*
			 * name()
			 * Returns: the menu name (progmem, i.e. SUI_DeclareString-created) string.
			 */
			PGM_P name();
			/*
			 * setName(NAME)
			 * Sets the menu name to (progmem,  i.e. SUI_DeclareString-created) string.
			 */
			void setName(PGM_P namestr);


			/*
			 * Access the menu's parent menu.
			 *
			 * Returns: pointer to Menu, or NULL if this is the top level menu.
			 */
			Menu * parent();



			/*
			 * returnOK()
			 * Return (prints out) the standard OK response string.
			 * For use in callbacks.
			 */
			void returnOK();

			/*
			 * returnMessage(MESSAGE)
			 * Return (prints out) MESSAGE, which is a
			 * progmem,  i.e. SUI_DeclareString-created, string.
			 */
			void returnMessage(PGM_P message) ;


			/*
			 * returnError([ERRORMESSAGE])
			 * Return (prints out) an error message.
			 *
			 * ERRORMESSAGE is an optional pointer to a null terminated string (char*).
			 * This is the only SerialUI method that takes
			 * a regular char* argument, so you don't have to
			 * preset all the error messages as program memory strings.
			 *
			 * Output will be:
			 *   ERROR: ERRORMESSAGE
			 * when error message is passed, or simply:
			 *   ERROR
			 * otherwise.
			 */
			void returnError(const char * errmsg=NULL);



			/*
			 * showName()
			 * Convenience function to print out the menu's name.  Equivalent to:
			 * 	returnMessage(name());
			 */
			void showName() ;



#ifdef SUI_MENU_INCLUDE_DESTRUCTION_CLEANUP
			/*
			 * Destruction/clearing of menu items would be useful if you are dynamically modifying
			 * the menu structure.  This functionality takes up space that is mostly unneeded, so
			 * you'll have to define SUI_MENU_INCLUDE_DESTRUCTION_CLEANUP above to include the
			 * functionality.
			 *
			 */
			~Menu();
			void clear();
#endif

		private:
			SerialUI * sui_driver;
			PGM_P menu_name;
			uint8_t num_menu_items;
			uint8_t max_menu_items;
			uint8_t expand_list_amount;

			MenuItem * item_list;

			Menu * parent_menu;


			// Real-life user interaction seems to indicate that
			// being maximally efficient -- i.e. figuring out the maximum
			// key length for *this* menu and only reading (up to) that
			// number of characters, doesn't account for the human tendency
			// to get... lost.
			// So instead of tracking the max key length per menu, we'll track
			// it *globally* (as a class static var) and allow you to enter:
			// 	really_long_mistake
			// even if the current Menu only accepts very short, e.g.
			//	on
			//	off
			// keys.  That way, "really_long_mistake" won't generate
			// 6 error messages, breaking it up into "off"-sized chunks.
			static size_t max_key_len;

#ifndef SUI_DYNAMIC_MEMORY_ALLOCATION_ENABLE
			// we're not using dynamic memory, so we need a few
			// arrays for menu items and such, with hard-coded lengths
			MenuItem item_staticlist[SUI_STATIC_MEMORY_NUM_ITEMS_MAXIMUM];
			char key_staticstr[SUI_STATIC_MEMORY_KEY_LENGTH_MAXIMUM + 1];
 #ifdef SUI_MENU_ENABLE_SUBMENUS
			// we also want sub-menus... this is a bit tough without
			// malloc and company, especially since we have an "incomplete type" at
			// this stage.  So we use a static block of memory, to hold all the sub-menus
			// the SUI_STATIC_MEMORY_NUM_SUBMENUS_TOTAL_MAXIMUM define (in SUIConfig.h)
			// sets the total number of possible sub-menus in the ENTIRE UI (not per-menu)!
			static Menu submenu_staticlist[SUI_STATIC_MEMORY_NUM_SUBMENUS_TOTAL_MAXIMUM];
			static uint8_t submenu_static_idx;
 #endif
#endif




			/* *****************  CONSTRUCTION  ************** */

			/*
			 * Menu constructors.
			 * You shouldn't need to use this, normally.  Just call
			 * 	SerialUI::topLevelMenu()
			 * and use Menu::subMenu() to create sub-menus properly.
			 */
			Menu(SerialUI * SUIDriver, PGM_P name, uint8_t num_items_hint=0, Menu* parent_menu_ptr=NULL);
			Menu();

			/*
			 * init
			 * Performs Menu initialisation, used internally when malloc'ing Menus.
			 */
			void init(SerialUI * SUIdrv, PGM_P name, uint8_t num_items_hint, Menu * parent);




			/* *****************  REQUEST HANDLING  ************** */

			/*
			 * handleRequest
			 * Handles a request from the user on this menu.
			 *
			 * Returns: the currently active menu (can be this menu, or a submenu if that's what
			 * the user selected).
			 */
			Menu * handleRequest();

			/*
			 * showHelp()
			 * Built-in command to show menu help, called by user with
			 * SUI_SERIALUI_HELP_KEY ("?" by default).
			 */
			void showHelp();

#ifdef SUI_MENU_ENABLE_SUBMENUS
			/*
			 * upLevel()
			 * Built-in command to go up a level in the hierarchy, called by user
			 * with SUI_SERIALUI_UP_KEY (".." by default).
			 */
			Menu * upLevel();
#endif

			/*
			 * unknownCommand(KEY)
			 * Show error response when KEY command is unrecognized.
			 */
			void unknownCommand(char * key);


			/*
			 * enter
			 * Executed on entering this Menu.
			 */
			void enter();


			/*
			 * addMenuItem(MENUITEM)
			 * Adds a menu item to the list, expanding it as necessary.
			 *
			 * Returns boolean true on success, false otherwise.
			 */
			bool addMenuItem(MenuItem * itm);

			// DEADBEEF: bool createNewItem();

			/*
			 * expandItemList([BYAMOUNT])
			 * Expands the capacity of our menu item list, by BYAMOUNT
			 * (or, if not passed, by SUI_MENU_EXPANDITEMLIST_AMOUNT_DEFAULT)
			 * slots.
			 *
			 * Returns boolean true on success, false otherwise.
			 */
			bool expandItemList(uint8_t by_amount=0);


			/* ******************* MENU ITEM KEY MANIP ******************* */

			/*
			 * mallocReadKey()
			 * Allocates space for any of the keys in this menu, and reads it in
			 * from the SerialUI input.
			 *
			 * Returns a pointer to a malloc'ed char* on success (must be freed), NULL otherwise.
			 */
			char * mallocReadKey();

			/*
			 * itemForKey(KEY)
			 * Finds the first MenuItem matching key, if possible.
			 *
			 * NOTE: This allows shortening the command passed.  E.g. if your menu has
			 * keys:
			 * 		allow
			 * 		deny
			 * 		disable
			 * 		enable
			 * Then the user can enter:
			 * 	"a" to activate "allow"
			 * 	"de" to activate "deny"
			 * 	"di" to activate "disable"
			 * 	"e" to activate "enable"
			 * Entering a key that leads to a collision because it it too short, like:
			 * 	"d" -- matches both "deny" and "disable"
			 * will cause the first matching menu item entry (in order of addition to the
			 * menu) to be activated.
			 *
			 * Returns a MenuItem pointer if found, NULL otherwise.
			 */
			MenuItem * itemForKey(char * key);
			

			/*
			 * printHelpKey(MENUITEM)
			 * Convenience function for pretty-printing the help menu keys.
			 */
			void printHelpKey(MenuItem * menuitem);


			/*
			 * We allow the SerialUI class a peek inside the Menu class, so
			 * it can construct and enter Menus.
			 */
			friend class SerialUI;


	};



} /* end namespace SUI */


#endif

