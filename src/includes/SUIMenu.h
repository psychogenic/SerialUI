/*
 *
 * SUIMenu.h -- SerialUI Menu, part of SerialUI a serial User Interface
 * for Arduino-compatible devices.
 * Copyright (C) 2013-2017 Pat Deegan, psychogenic.com. All rights reserved.
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
#include "SUITypes.h"
/*
 * SUI_SERIALUI_KEYHELP_SEP_REPEATS_MAX
 * Maximum amount of SUI_SERIALUI_KEYHELP_SEP repeats between key and help,
 * used to format help output.
 */
#define SUI_SERIALUI_KEYHELP_SEP_REPEATS_MAX		20

/*
 * SUI_MENU_EXPANDITEMLIST_AMOUNT_DEFAULT
 * Increment to use when creating/expanding the memory held for the list
 * of menu items.
 */
#define SUI_MENU_EXPANDITEMLIST_AMOUNT_DEFAULT		4

// SUI is the namespace in which we keep all our goodies.
namespace SUI {

namespace MenuItem {
class Base;
// forward decl
}

class SerialUI;
// forward decl


typedef SovA::Utils::DynCharBuffer ReadKey;



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
	bool addCommand(SOVA_FLASHSTRING_PTR key_str, MenuCommand_Callback callback,SOVA_FLASHSTRING_PTR help_str=NULL);
	

	/* option list requests:
	 * addRequest(&val, KEY, HELP, OPT1, OPT2, [OPT3... OPT6,] [VALIDATION,  CHANGECALLBACK]
	 */
	inline bool addRequest(long int * val,
					SOVA_FLASHSTRING_PTR key_pstr, SOVA_FLASHSTRING_PTR help_pstr,
					SOVA_FLASHSTRING_PTR opt1, SOVA_FLASHSTRING_PTR opt2,
					bool(*vcb)(long int & newVal),
					MenuRequest_Callback cb) {
				return _doAddRequest(val, key_pstr, help_pstr, opt1, opt2, NULL, NULL, NULL, NULL, vcb, cb);
			}


	inline bool addRequest(long int * val,
					SOVA_FLASHSTRING_PTR key_pstr, SOVA_FLASHSTRING_PTR help_pstr,
					SOVA_FLASHSTRING_PTR opt1, SOVA_FLASHSTRING_PTR opt2,
					MenuRequest_Callback cb) {
				return _doAddRequest(val, key_pstr, help_pstr, opt1, opt2, NULL, NULL, NULL, NULL, NULL, cb);
			}

	inline bool addRequest(long int * val,
					SOVA_FLASHSTRING_PTR key_pstr, SOVA_FLASHSTRING_PTR help_pstr,
					SOVA_FLASHSTRING_PTR opt1, SOVA_FLASHSTRING_PTR opt2,
					SOVA_FLASHSTRING_PTR opt3,
					bool(*vcb)(long int & newVal),
					MenuRequest_Callback cb) {
				return _doAddRequest(val, key_pstr, help_pstr, opt1, opt2, opt3, NULL, NULL, NULL, vcb, cb);
			}


	inline bool addRequest(long int * val,
					SOVA_FLASHSTRING_PTR key_pstr, SOVA_FLASHSTRING_PTR help_pstr,
					SOVA_FLASHSTRING_PTR opt1, SOVA_FLASHSTRING_PTR opt2,
					SOVA_FLASHSTRING_PTR opt3,
					MenuRequest_Callback cb) {
				return _doAddRequest(val, key_pstr, help_pstr, opt1, opt2, opt3, NULL, NULL, NULL, NULL, cb);
			}


	inline bool addRequest(long int * val,
					SOVA_FLASHSTRING_PTR key_pstr, SOVA_FLASHSTRING_PTR help_pstr,
					SOVA_FLASHSTRING_PTR opt1, SOVA_FLASHSTRING_PTR opt2,
					SOVA_FLASHSTRING_PTR opt3, SOVA_FLASHSTRING_PTR opt4,
					bool(*vcb)(long int & newVal),
					MenuRequest_Callback cb) {
				return _doAddRequest(val, key_pstr, help_pstr, opt1, opt2, opt3, opt4, NULL, NULL, vcb, cb);
			}


	inline bool addRequest(long int * val,
					SOVA_FLASHSTRING_PTR key_pstr, SOVA_FLASHSTRING_PTR help_pstr,
					SOVA_FLASHSTRING_PTR opt1, SOVA_FLASHSTRING_PTR opt2,
					SOVA_FLASHSTRING_PTR opt3, SOVA_FLASHSTRING_PTR opt4,
					MenuRequest_Callback cb) {
				return _doAddRequest(val, key_pstr, help_pstr, opt1, opt2, opt3, opt4, NULL, NULL, NULL, cb);
			}


	inline bool addRequest(long int * val,
					SOVA_FLASHSTRING_PTR key_pstr, SOVA_FLASHSTRING_PTR help_pstr,
					SOVA_FLASHSTRING_PTR opt1, SOVA_FLASHSTRING_PTR opt2,
					SOVA_FLASHSTRING_PTR opt3, SOVA_FLASHSTRING_PTR opt4,
					SOVA_FLASHSTRING_PTR opt5,
					bool(*vcb)(long int & newVal),
					MenuRequest_Callback cb) {
				return _doAddRequest(val, key_pstr, help_pstr, opt1, opt2, opt3, opt4, opt5, NULL, vcb, cb);
			}


	inline bool addRequest(long int * val,
					SOVA_FLASHSTRING_PTR key_pstr, SOVA_FLASHSTRING_PTR help_pstr,
					SOVA_FLASHSTRING_PTR opt1, SOVA_FLASHSTRING_PTR opt2,
					SOVA_FLASHSTRING_PTR opt3, SOVA_FLASHSTRING_PTR opt4,
					SOVA_FLASHSTRING_PTR opt5,
					MenuRequest_Callback cb) {
				return _doAddRequest(val, key_pstr, help_pstr, opt1, opt2, opt3, opt4, opt5, NULL, NULL, cb);
			}

	inline bool addRequest(long int * val,
					SOVA_FLASHSTRING_PTR key_pstr, SOVA_FLASHSTRING_PTR help_pstr,
					SOVA_FLASHSTRING_PTR opt1, SOVA_FLASHSTRING_PTR opt2,
					SOVA_FLASHSTRING_PTR opt3, SOVA_FLASHSTRING_PTR opt4,
					SOVA_FLASHSTRING_PTR opt5, SOVA_FLASHSTRING_PTR opt6,
					MenuRequest_Callback cb ) {
				return _doAddRequest(val, key_pstr, help_pstr, opt1, opt2, opt3, opt4, opt5, opt6, NULL, cb);
			}

	inline bool addRequest(long int * val,
					SOVA_FLASHSTRING_PTR key_pstr, SOVA_FLASHSTRING_PTR help_pstr,
					SOVA_FLASHSTRING_PTR opt1, SOVA_FLASHSTRING_PTR opt2,
					SOVA_FLASHSTRING_PTR opt3, SOVA_FLASHSTRING_PTR opt4,
					SOVA_FLASHSTRING_PTR opt5, SOVA_FLASHSTRING_PTR opt6,
					bool(*vcb)(long int & newVal),
					MenuRequest_Callback cb) {
				return _doAddRequest(val, key_pstr, help_pstr, opt1, opt2, opt3, opt4, opt5, opt6, vcb, cb);
			}



	bool _doAddRequest(long int * val,
					SOVA_FLASHSTRING_PTR key_pstr, SOVA_FLASHSTRING_PTR help_pstr,
					SOVA_FLASHSTRING_PTR opt1, SOVA_FLASHSTRING_PTR opt2,
					SOVA_FLASHSTRING_PTR opt3, SOVA_FLASHSTRING_PTR opt4,
					SOVA_FLASHSTRING_PTR opt5, SOVA_FLASHSTRING_PTR opt6,
					bool(*vcb)(long int & newVal),
					MenuRequest_Callback cb
					);




			/* range request
			 *
			 * &val KEY HELP MIN MAX [VALIDATION] [CALLBACK]
			 */

	bool addRequest(long int * val,
					SOVA_FLASHSTRING_PTR key_pstr, SOVA_FLASHSTRING_PTR help_pstr,
					long int min_allowed, long int max_allowed,
					bool(*vcb)(long int & newVal),
					MenuRequest_Callback valueChangedCb=NULL);

	inline bool addRequest(long int * val,
					SOVA_FLASHSTRING_PTR key_pstr, SOVA_FLASHSTRING_PTR help_pstr,
					long int min_allowed, long int max_allowed,
					MenuRequest_Callback valueChangedCb=NULL) {
				return addRequest(val, key_pstr, help_pstr, min_allowed, max_allowed, NULL, valueChangedCb);
	}

	/* long int request
	 *
	 * &val KEY HELP [VALIDATOR] [CALLBACK]
	 */
	bool addRequest(long int * val,  SOVA_FLASHSTRING_PTR key_pstr, SOVA_FLASHSTRING_PTR help_pstr,
				bool(*validator)(long int &), MenuRequest_Callback valueChangedCb=NULL);

	/* long uint request */
	bool addRequest(long unsigned int * val,  SOVA_FLASHSTRING_PTR key_pstr, SOVA_FLASHSTRING_PTR help_pstr,
				bool(*validator)(long unsigned int &), MenuRequest_Callback valueChangedCb=NULL);

	bool addRequest(char * val,  SOVA_FLASHSTRING_PTR key_pstr, SOVA_FLASHSTRING_PTR help_pstr,
				bool(*validator)(char &), MenuRequest_Callback valueChangedCb=NULL);
	/* float request */
	bool addRequest(float * val, SOVA_FLASHSTRING_PTR key_pstr, SOVA_FLASHSTRING_PTR help_pstr,
				bool(*validator)(float &), MenuRequest_Callback valueChangedCb=NULL);

	/* String request
	 *
	 * &val KEY HELP MAXLEN [VALIDATOR] [CHANGEDCALLBACK]
	 *
	 */
	bool addRequest(String * val,  SOVA_FLASHSTRING_PTR key_pstr, SOVA_FLASHSTRING_PTR help_pstr,
			uint8_t maxLength,
			bool(*validator)(String &), MenuRequest_Callback valueChangedCb=NULL);

	inline bool addRequest(String * val, SOVA_FLASHSTRING_PTR key_pstr, SOVA_FLASHSTRING_PTR help_pstr,
			uint8_t maxLength,
			MenuRequest_Callback valueChangedCb=NULL) {
		return addRequest(val, key_pstr, help_pstr, maxLength, NULL, valueChangedCb);
	}


	/* bool request, with validator */
	bool addRequest(bool * val,  SOVA_FLASHSTRING_PTR key_pstr, SOVA_FLASHSTRING_PTR help_pstr,
			bool(*validator)(bool &), MenuRequest_Callback valueChangedCb=NULL);

	/* bool request, no validator */
	inline bool addRequest(bool * val,  SOVA_FLASHSTRING_PTR key_pstr, SOVA_FLASHSTRING_PTR help_pstr,
			MenuRequest_Callback valueChangedCb=NULL)
	{
		return addRequest(val, key_pstr, help_pstr, NULL, valueChangedCb);
	}


	/* toggle request -- same as bool, but no param required, just flips */
	bool addToggle(bool* val, SOVA_FLASHSTRING_PTR key_pstr, SOVA_FLASHSTRING_PTR help_pstr,
			MenuRequest_Callback valueChangedCb=NULL);

	inline bool addRequest(long int * val,  SOVA_FLASHSTRING_PTR key_pstr, SOVA_FLASHSTRING_PTR help_pstr, MenuRequest_Callback valueChangedCb=NULL)
	{ return addRequest(val, key_pstr, help_pstr, NULL, valueChangedCb); }
	inline bool addRequest(long unsigned int * val,  SOVA_FLASHSTRING_PTR key_pstr, SOVA_FLASHSTRING_PTR help_pstr, MenuRequest_Callback valueChangedCb=NULL)
	{ return addRequest(val, key_pstr, help_pstr, NULL, valueChangedCb); }
	inline bool addRequest(char * val,  SOVA_FLASHSTRING_PTR key_pstr, SOVA_FLASHSTRING_PTR help_pstr, MenuRequest_Callback valueChangedCb=NULL)
	{ return addRequest(val, key_pstr, help_pstr, NULL, valueChangedCb); }

	inline bool addRequest(float * val, SOVA_FLASHSTRING_PTR key_pstr, SOVA_FLASHSTRING_PTR help_pstr, MenuRequest_Callback valueChangedCb=NULL)
	{ return addRequest(val, key_pstr, help_pstr, NULL, valueChangedCb); }



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
	Menu * subMenu(SOVA_FLASHSTRING_PTR key_str, SOVA_FLASHSTRING_PTR help_str=NULL, uint8_t num_items_hint=SUI_MENU_EXPANDITEMLIST_AMOUNT_DEFAULT);
#endif

	/*
	 * name()
	 * Returns: the menu name (progmem, i.e. SUI_DeclareString-created) string.
	 */
	SOVA_FLASHSTRING_PTR name();
	/*
	 * setName(NAME)
	 * Sets the menu name to (progmem,  i.e. SUI_DeclareString-created) string.
	 */
	void setName(SOVA_FLASHSTRING_PTR namestr);


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
	void returnMessage(SOVA_FLASHSTRING_PTR message) ;


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


#ifndef SOVA_FLASHSTRING_DISABLE
	void returnError(SOVA_FLASHSTRING_PTR errmsg);
#endif

	/*
	 * returnError(ERRORMESSAGE)
	 * Return (prints out) an error message.
	 * This version is useful if you have ERRORMESSAGE in progmem
	 * (e.g. declared with SUI_DeclareString)
	 */



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


	// used internally:
	inline SerialUI * driver() { return sui_driver;}
	void enter();

	inline uint8_t numItems() { return num_menu_items;}
	inline MenuItem::Base * currentItem() { return current_item;}
private:
	SerialUI * sui_driver;
	SOVA_FLASHSTRING_PTR menu_name;
	uint8_t num_menu_items;
	uint8_t max_menu_items;
	// uint8_t expand_list_amount;

	MenuItem::Base ** item_list;
	MenuItem::Base * current_item;

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






	/* *****************  CONSTRUCTION  ************** */

	/*
	 * Menu constructors.
	 * You shouldn't need to use this, normally.  Just call
	 * 	SerialUI::topLevelMenu()
	 * and use Menu::subMenu() to create sub-menus properly.
	 */
	Menu(SerialUI * SUIDriver, SOVA_FLASHSTRING_PTR name, uint8_t num_items_hint=SUI_MENU_EXPANDITEMLIST_AMOUNT_DEFAULT, Menu* parent_menu_ptr=NULL);
	Menu();

	void init(SerialUI * SUIdrv, SOVA_FLASHSTRING_PTR name, uint8_t num_items_hint, Menu * parent);




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
	void unknownCommand(const char * key);




	/*
	 * addMenuItem(MENUITEM)
	 * Adds a menu item to the list, expanding it as necessary.
	 *
	 * Returns boolean true on success, false otherwise.
	 */
	bool addMenuItem(MenuItem::Base * itm);

	/*
	 * expandItemList([BYAMOUNT])
	 * Expands the capacity of our menu item list, by BYAMOUNT
	 * (or, if not passed, by SUI_MENU_EXPANDITEMLIST_AMOUNT_DEFAULT)
	 * slots.
	 *
	 * Returns boolean true on success, false otherwise.
	 */
	bool expandItemList(uint8_t by_amount=SUI_MENU_EXPANDITEMLIST_AMOUNT_DEFAULT);


	/* ******************* MENU ITEM KEY MANIP ******************* */

	/*
	 * newReadKey()
	 * Allocates space for any of the keys in this menu, and reads it in
	 * from the SerialUI input.
	 *
	 * Returns a pointer to a new char* on success (must be freed), NULL otherwise.
	 */
	char * newReadKey();

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
	MenuItem::Base * itemForKey(const char * key);


	/*
	 * printHelpKey(MENUITEM)
	 * Convenience function for pretty-printing the help menu keys.
	 */
	void printHelpKey(MenuItem::Base * menuitem);


	/*
	 * We allow the SerialUI class a peek inside the Menu class, so
	 * it can construct and enter Menus.
	 */
	friend class SerialUI;


	void pingRespond();

	Menu * enterProgramMode();

};



} /* end namespace SUI */

#endif

