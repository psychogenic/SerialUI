/*
 * DeSerializer.cpp
 *
 *  Created on: May 3, 2019
 *      Author: Pat Deegan
 *
 *  Part of the SerialUI project.
 *  Copyright (C) 2019 Pat Deegan, https://psychogenic.com
 *  More information on licensing and usage at 
 *  https://devicedruid.com/
 */


#include "includes/settings/DeSerializer.h"
#include "includes/SerialUIPlatform.h"
#include "includes/SUIGlobals.h"
namespace SerialUI {
namespace Settings {

DeSerializer::DeSerializer(Storage * store, uint16_t maxsize) :
		_storage(store), max_serstate_len(maxsize) {
	/*
	for (uint8_t i=0; i<SERIALIZER_MAX_DEPTH; i++) {
		menuStack[i] = NULL;
	}
	*/
}
bool DeSerializer::restore() {

	SerializedStateByte * buf = new SerializedStateByte[max_serstate_len];
	if (! buf){
		SERIALUI_DEBUG_OUTLN(F("Could not alloc serializer buf"));
		return false;
	}

	if (! _storage->retrieve(buf, max_serstate_len - 1))
	{
		return false;
	}


	bool success = deserializeFrom(buf);
	delete [] buf;
	return success;
}

bool DeSerializer::setValue(Menu::Item::Request::Request* req, bool val) {
	switch (req->requestType()) {
	case Menu::Item::Request::Type::Boolean:
		/* fall-through */
	case Menu::Item::Request::Type::Toggle:
		SERIALUI_DEBUG_OUTLN(F("set bool"));
		req->castAsSubType<Menu::Item::Request::Boolean>()->setValue(val);
		Globals::commChannel()->print(req);
		return true;
	default:
		SERIALUI_DEBUG_OUTLN(F("DeSer: NOT bool"));
		return false;

	}
}

bool DeSerializer::setValue(Menu::Item::Request::Request* req, long int val) {
	switch (req->requestType()) {
	case Menu::Item::Request::Type::BoundedLongInt:
		/* fall-through */
	case Menu::Item::Request::Type::LongInt:
		/* fall-through */
	case Menu::Item::Request::Type::OptionsList:
		SERIALUI_DEBUG_OUTLN(F("set long"));
		req->castAsSubType<Menu::Item::Request::Long>()->setValue(val);
		Globals::commChannel()->print(req);
		return true;
	default:
		SERIALUI_DEBUG_OUTLN(F("DeSer: NOT long"));
		return false;

	}
}

bool DeSerializer::setValue(Menu::Item::Request::Request* req, unsigned long int val) {
	switch (req->requestType()) {
	case Menu::Item::Request::Type::Color:
		SERIALUI_DEBUG_OUT(F("set ulong color "));
		SERIALUI_DEBUG_OUTLN(val);
		req->castAsSubType<Menu::Item::Request::Color>()->setValue(val);
		break;
	case Menu::Item::Request::Type::Event:
		SERIALUI_DEBUG_OUT(F("set ulong event "));
		SERIALUI_DEBUG_OUTLN(val);
		req->castAsSubType<Menu::Item::Request::Event>()->setValue(val);
		break;
	case Menu::Item::Request::Type::DateTime:
		SERIALUI_DEBUG_OUT(F("set ulong datetime "));
		SERIALUI_DEBUG_OUTLN(val);
		req->castAsSubType<Menu::Item::Request::DateTime>()->setValue(val);
		break;
	case Menu::Item::Request::Type::Time:

		SERIALUI_DEBUG_OUT(F("set ulong time "));
		SERIALUI_DEBUG_OUTLN(val);
		req->castAsSubType<Menu::Item::Request::Time>()->setValue(val);
		break;
	case Menu::Item::Request::Type::UnsignedLongInt:
		SERIALUI_DEBUG_OUT(F("set ulong "));
		SERIALUI_DEBUG_OUTLN(val);
		req->castAsSubType<Menu::Item::Request::UnsignedLong>()->setValue(val);
		break;
	default:
		SERIALUI_DEBUG_OUTLN(F("DeSer: NOT ulong"));
		return false;

	}
	Globals::commChannel()->print(req);
	return true;
}
bool DeSerializer::setValue(Menu::Item::Request::Request* req, float val) {
	switch (req->requestType()) {
	case Menu::Item::Request::Type::Float:
		SERIALUI_DEBUG_OUTLN(F("set float"));
		req->castAsSubType<Menu::Item::Request::Float>()->setValue(val);
		Globals::commChannel()->print(req);
		return true;
	default:
		SERIALUI_DEBUG_OUTLN(F("DeSer: NOT float"));
		return false;

	}
}

bool DeSerializer::setValue(Menu::Item::Request::Request* req, char val) {
	switch (req->requestType()) {
	case Menu::Item::Request::Type::Character:
		req->castAsSubType<Menu::Item::Request::Character>()->setValue(val);
		Globals::commChannel()->print(req);
		return true;
	default:
		return false;

	}
}

bool DeSerializer::setValue(Menu::Item::Request::Request* req,
	const char* val) {switch (req->requestType()) {
	case Menu::Item::Request::Type::Passphrase:
		/* fall-through */
	case Menu::Item::Request::Type::String:
		SERIALUI_DEBUG_OUTLN(F("set str"));
		req->castAsSubType<Menu::Item::Request::String>()->setValue(val);
		Globals::commChannel()->print(req);
		return true;
	default:
		SERIALUI_DEBUG_OUTLN(F("DeSer: NOT str"));
		return false;

	}
}

} /* namespace Settings */
} /* namespace SerialUI */

