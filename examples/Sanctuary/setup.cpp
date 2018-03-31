

/*
 * setup.cpp -- part of the Sanctuary project.
 * Setup of SerialUI and menu system
 *
 * This example was actually generated (by Druid Builder) and 
 * created as part of the builder tutorial series.
 *
 * It includes a few commands, a number of input requests and 
 * submenus... you can see the whole process of creating the 
 * system in the learning section of the devicedruid.com site 
 *  https://devicedruid.com/learn-more/learn-druid-builder/
 *
 * Druid4Arduino, Device Druid, Druid Builder, the builder 
 * code brewery and its wizards, SerialUI and supporting 
 * libraries, as well as the generated parts of this program 
 * are 
 *            Copyright (C) 2013-2017 Pat Deegan 
 * [http://psychogenic.com/ | http://flyingcarsandstuff.com/]
 * and distributed under the terms of their respective licenses.
 * See http://devicedruid.com for details.
 * 
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. 
 * THE ENTIRE RISK AS TO THE QUALITY AND PERFORMANCE OF THE 
 * PROGRAM IS WITH YOU. SHOULD THE PROGRAM PROVE DEFECTIVE, 
 * YOU ASSUME THE COST OF ALL NECESSARY SERVICING, REPAIR OR
 * CORRECTION.
 * 
 * Keep in mind that there is no warranty and you are solely 
 * responsible for the use of all these cool tools.
 * 
 * Play safe, have fun.
 * 
 */


/* we need the SerialUI lib */
#include <SerialUI.h>
#include "SanctuarySettings.h"

/* our project specific types and functions are here */
#include "Sanctuary.h"



/* MySUI
 * Our SerialUI Instance, through which we can send/receive
 * data from users. Actually instantiated here, for global use.
 */
SUI::SerialUI MySUI(6);


/*
 * The container for MyInputs, which
 * holds all the variables for the various inputs.
 * Actually instantiated here, for global use.
 */
MyInputsContainerSt MyInputs;


/* MyTracked
 * A structure to hold the tracked variables, which will 
 * automatically update the druid UI when modified by this 
 * program.
 * Actually instantiated here, for global use.
 */
MyTrackedVarsContainerSt MyTracked;






bool SetupSerialUI() {

  // basic SUI setup
	MySUI.setGreeting(F(serial_ui_greeting_str));
	// SerialUI acts just like (is actually a facade for)
	// Serial.  Use _it_, rather than Serial, throughout the
	// program.
	// basic setup of SerialUI:
	MySUI.begin(serial_baud_rate); // serial line open/setup
	MySUI.setTimeout(serial_readtimeout_ms);   // timeout for reads (in ms), same as for Serial.
	MySUI.setMaxIdleMs(serial_maxidle_ms);    // timeout for user (in ms)
	// how we are marking the "end-of-line" (\n, by default):
	MySUI.setReadTerminator(serial_input_terminator);
	// project UID -- will be used to remember state in Druid...
	MySUI.setUID(SUI_STR("08a0574074914bed8e8d0c039c85c353N4AD2BFZEU"));

	// have a "heartbeat" function to hook-up
	MySUI.setUserPresenceHeartbeat(CustomHeartbeatCode); 
	// heartbeat_function_period_ms set in main settings header
	MySUI.setUserPresenceHeartbeatPeriod(heartbeat_function_period_ms);
	
	
	
	// Add variable state tracking 
	
	MySUI.trackState(
		SUI_STR("Counter"),
		&(MyTracked.Counter));
	
	MySUI.trackState(
		SUI_STR("Office"),
		&(MyTracked.Office));
	
	MySUI.trackState(
		SUI_STR("Outter"),
		&(MyTracked.Outter));
	
	MySUI.trackState(
		SUI_STR("LowLev"),
		&(MyTracked.LowLev));
	
	MySUI.trackState(
		SUI_STR("TigerFood"),
		&(MyTracked.TigerFood));
	
	
	
	// a few error messages we hopefully won't need
	
	SUI_FLASHSTRING CouldntCreateMenuErr = F("Could not create menu?");
	SUI_FLASHSTRING CouldntAddItemErr = F("Could not add item?");
	
	// get top level menu, to start adding items
	SUI::Menu * topMenu = MySUI.topLevelMenu();
	if (! topMenu ) {
		// well, that can't be good...
		MySUI.returnError(F("Very badness in sEriALui!1"));
		return false;
	}
	
	
	
	/* *** Animals *** */

	
	if( ! topMenu->addCommand(
		SUI_STR("Stock Levels"),
		Animals::doStockLevels,
		SUI_STR("Show stock levels"))) {
		MySUI.returnError(CouldntAddItemErr);
		return false;
	}
	
	if( ! topMenu->addCommand(
		SUI_STR("Feed All"),
		Animals::doFeedAll,
		SUI_STR("Feed all animalz"))) {
		MySUI.returnError(CouldntAddItemErr);
		return false;
	}

  // create all our sub-menus
	SUI::Menu * submen1 = topMenu->subMenu(
		SUI_STR("Tigers"),
		SUI_STR("Rarrr"),
		2);
	if (! submen1 ) { 
		DIE_HORRIBLY(CouldntCreateMenuErr);
	}
	
	SUI::Menu * submen2 = topMenu->subMenu(
		SUI_STR("Monkeys"), NULL, 2);
	if (! submen2 ) { 
		DIE_HORRIBLY(CouldntCreateMenuErr);
	}
	
	SUI::Menu * submen3 = topMenu->subMenu(
		SUI_STR("Fishes"),
		SUI_STR("Fishies"), 3);
	if (! submen3 ) { 
		DIE_HORRIBLY(CouldntCreateMenuErr);
	}
	
	SUI::Menu * submen4 = topMenu->subMenu(
		SUI_STR("Reptiles"),
		SUI_STR("Lizards and such..."), 4);
	if (! submen4 ) { 
		DIE_HORRIBLY(CouldntCreateMenuErr);
	}
	


  // add everything to the right sub-menu

	/* *** Animals -> Tigers *** */

	
	if( ! submen1->addCommand(
		SUI_STR("Empty Litter"),
		Animals::Tigers::doEmptyLitter,
		SUI_STR("Clean it up..."))) {
		MySUI.returnError(CouldntAddItemErr);
		return false;
	}
	
	if( ! submen1->addCommand(
		SUI_STR("Feed"),
		Animals::Tigers::doFeed,
		SUI_STR("Feed the tigers"))) {
		MySUI.returnError(CouldntAddItemErr);
		return false;
	}


	/* *** Animals -> Monkeys *** */

	
	if( ! submen2->addCommand(
		SUI_STR("Feed"),
		Animals::Monkeys::doFeed,
		SUI_STR("Feed the monkeys"))) {
		MySUI.returnError(CouldntAddItemErr);
		return false;
	}
	
	if( ! submen2->addRequest(
		&(MyInputs.WebCam),
		SUI_STR("Web Cam"),
		SUI_STR("Turn off at night..."),
		Animals::Monkeys::WebCamChanged)) {
		MySUI.returnError(CouldntAddItemErr);
		return false;
	}


	/* *** Animals -> Fishes *** */

	
	if( ! submen3->addCommand(
		SUI_STR("Feed"),
		Animals::Fishes::doFeed,
		SUI_STR("feed fish"))) {
		MySUI.returnError(CouldntAddItemErr);
		return false;
	}
	
	if( ! submen3->addRequest(
		&(MyInputs.SaltTanks),
		SUI_STR("Salt Tanks"),
		SUI_STR("degrees C"),
		26,
		32,
		Animals::Fishes::SaltTanksChanged)) {
		MySUI.returnError(CouldntAddItemErr);
		return false;
	}
	
	if( ! submen3->addRequest(
		&(MyInputs.FreshTanks),
		SUI_STR("Fresh Tanks"),
		SUI_STR("Fresh water thermo"),
		Animals::Fishes::FreshTanksIsOk,
		Animals::Fishes::FreshTanksChanged)) {
		MySUI.returnError(CouldntAddItemErr);
		return false;
	}


	/* *** Animals -> Reptiles *** */

	
	if( ! submen4->addCommand(
		SUI_STR("Feed"),
		Animals::Reptiles::doFeed,
		SUI_STR("feed the reptiles"))) {
		MySUI.returnError(CouldntAddItemErr);
		return false;
	}
	
	if( ! submen4->addRequest(
		&(MyInputs.Lighting),
		SUI_STR("Lighting"),
		NULL,
		Animals::Reptiles::LightingChanged)) {
		MySUI.returnError(CouldntAddItemErr);
		return false;
	}
	
	if( ! submen4->addRequest(
		&(MyInputs.Fountains),
		SUI_STR("Fountains"),
		SUI_STR("How much rain"),
		SUI_STR("Tundra"),
		SUI_STR("Misty"),
		SUI_STR("Drizzle"),
		SUI_STR("Tropics"),
		SUI_STR("Stormy"),
		Animals::Reptiles::FountainsChanged)) {
		MySUI.returnError(CouldntAddItemErr);
		return false;
	}
	
	if( ! submen4->addRequest(
		&(MyInputs.Lamp),
		SUI_STR("Lamp"),
		SUI_STR("Heat Lamps"),
		SUI_STR("Lo"),
		SUI_STR("Std"),
		SUI_STR("Hi"),
		Animals::Reptiles::LampChanged)) {
		MySUI.returnError(CouldntAddItemErr);
		return false;
	}

	
	
	
	return true;
	
}

