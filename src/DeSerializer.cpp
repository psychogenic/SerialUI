/*
 * DeSerializer.cpp
 *
 *  Created on: May 3, 2019
 *      Author: malcalypse
 */


#include "includes/settings/DeSerializer.h"
#include "includes/SerialUIPlatform.h"
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
		return true;
	default:
		SERIALUI_DEBUG_OUTLN(F("DeSer: NOT long"));
		return false;

	}
}

bool DeSerializer::setValue(Menu::Item::Request::Request* req, unsigned long int val) {
	switch (req->requestType()) {
	case Menu::Item::Request::Type::UnsignedLongInt:
		req->castAsSubType<Menu::Item::Request::UnsignedLong>()->setValue(val);
		return true;
	default:
		SERIALUI_DEBUG_OUTLN(F("DeSer: NOT ulong"));
		return false;

	}
}
bool DeSerializer::setValue(Menu::Item::Request::Request* req, float val) {
	switch (req->requestType()) {
	case Menu::Item::Request::Type::Float:
		SERIALUI_DEBUG_OUTLN(F("set float"));
		req->castAsSubType<Menu::Item::Request::Float>()->setValue(val);
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
		return true;
	default:
		SERIALUI_DEBUG_OUTLN(F("DeSer: NOT str"));
		return false;

	}
}

} /* namespace Settings */
} /* namespace SerialUI */

